/* mbed Example Program
 * Copyright (c) 2006-2014 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************* Includes */
#include <main.h>
#include <SeggerRTT.h>

/******************************************************************** Globals */
PwmOut      pen (PWM_CCU4);
SPI         expander (SPI_MOSI, SPI_MISO, SPI_CLK);
DigitalOut  io_cs (IO_CS);
DigitalOut  adc_cs (ADC_CS);
DigitalIn   endstop_x_right   (ENDSTOP_1);
DigitalIn   endstop_x_left    (ENDSTOP_2);
DigitalIn   endstop_y_top     (ENDSTOP_3);
DigitalIn   endstop_y_bottom  (ENDSTOP_4);
DigitalOut  io_reset          (IO_RESET);
Serial      pc                (P0_1, P0_0);    // UART1 -> Tx = P0.1, Rx = P0.0
DigitalOut  led1(LED1);
DigitalOut  led2(LED2);


SeggerRTT gdb;


typedef struct {
  char dir_m1;
  char step_m1;
  char dir_m2;
  char step_m2;
} expander_mail_t;

typedef struct {
  int size_x;
  int size_y;
} plotter_mail_t;

typedef struct {
  char received[31];
} pc_mail_t;

Mail <expander_mail_t, 4> expander_mail;
Mail <plotter_mail_t, 4> plotter_mail;
Mail <pc_mail_t, 31> pc_mail;


/******************************************************************** Threads */
Thread thread_pos_plotter;
Thread thread_com_plotter;
Thread thread_com_pc;


/****************************************************************** Functions */

/**
 * @brief      { function_description }
 */
#if DEBUG_ENDPOINTS
void endpoint_indicators (void)
{
  while(1)
    led1.write(endstop_y_bottom.read());
}
#endif

/**
 * @brief      { function_description }
 */
void spi_init ()
{
  io_cs.write (1);
  expander.format (8, 0);
  expander.frequency (10000000); // 10 MHz
}


/**
 * @brief      { function_description }
 *
 * @param[in]  buffer  The buffer
 */
void expander_port (char buffer)
{
  io_cs = 0;
  expander.write (MCP23S08_ADDR);
  expander.write (MCP23S08_PORT);
  expander.write (buffer);
  io_cs = 1;
}


/**
 * @brief      { function_description }
 */
void modules_init ()
{
  spi_init();
  io_reset.write(0);
  wait_us(1);
  io_reset.write(1);

  adc_cs.write (1);

  // IOCON: SEQOP = 1, HAEN = 0
  io_cs = 0;
  expander.write (MCP23S08_ADDR);
  expander.write (MCP23S08_IOCON);
  expander.write (0x28);
  io_cs = 1;

  // IODIR: Joystick buttons as input, rest is output
  io_cs = 0;
  expander.write (MCP23S08_ADDR);
  expander.write (MCP23S08_IODIR);
  expander.write (0x00);
  io_cs = 1;
}



/**
 * @brief      { function_description }
 */
void com_plotter ()
{
  //char send_buffer[3] = {0};
  char buffer = 0;
  //char dummy[3] = {0};
  while (1)
  {
    #if DEBUG_PRIORITY
    gdb.printf("com\n");
    #endif
    osEvent evt = expander_mail.get();
    if (evt.status == osEventMail)
    {
      expander_mail_t * mail = (expander_mail_t *) evt.value.p;
      //send_buffer[0] = MCP23S08_ADDR;
      //send_buffer[1] = MCP23S08_PORT;
      //send_buffer[2] = (mail->dir_m1 << 0) | (mail->step_m1 << 1) | (mail->dir_m2 << 2) | (mail->step_m2 << 3);
      buffer = (mail->dir_m1 << 0) | (mail->step_m1 << 1) | (mail->dir_m2 << 2) | (mail->step_m2 << 3);

      #if DEBUG_COM_PLOT
      pc.printf("\tsetting... %x,%x,%x\n", MCP23S08_ADDR, MCP23S08_PORT, buffer);
      io_cs = 0;
      expander.write (MCP23S08_ADDR_READ);
      expander.write (MCP23S08_PORT);
      int ret = expander.write (0xAA);
      io_cs = 1;
      pc.printf("\treturn: %x\n", char(ret));
      __NOP();
      #endif
      //expander.write (send_buffer, strlen (send_buffer), dummy, strlen (send_buffer));
      expander_port (buffer);

      // Setting output back to 0 to finish stepping
      //send_buffer[0] = MCP23S08_ADDR;
      //send_buffer[1] = MCP23S08_PORT;
      //send_buffer[2] = (mail->dir_m1 << 0) | (0 << 1) | (mail->dir_m2 << 2) | (0 << 3);
      buffer = (mail->dir_m1 << 0) | (0 << 1) | (mail->dir_m2 << 2) | (0 << 3);
      #if DEBUG_COM_PLOT
      pc.printf("\tresetting... %x,%x,%x\n", MCP23S08_ADDR, MCP23S08_PORT, buffer);
      io_cs = 0;
      expander.write (MCP23S08_ADDR_READ);
      expander.write (MCP23S08_PORT);
      ret = expander.write (0xAA);
      io_cs = 1;
      pc.printf("\treturn: %x\n", char(ret));
      __NOP();
      #endif
      expander_port (buffer);
      expander_mail.free(mail);
    }
    thread_com_plotter.yield();
  }
}



/**
 * @brief      { function_description }
 */
void pos_init_plotter()
{
  expander_mail_t * mail = expander_mail.alloc();
  // initialize position to right bottom
  pc.printf("Starting to initialize position!\n");
  while (endstop_x_left.read() == 1 || endstop_y_bottom.read() == 1)
  {
    #if DEBUG_COM_PLOT
    pc.printf("running...\n");
    pc.printf("endstop_x_left: %x\n", endstop_x_left.read());
    pc.printf("endstop_y_bottom: %x\n", endstop_y_bottom.read());
    #endif
    #if DEBUG_PRIORITY
    gdb.printf("pos\n");
    #endif

    if (endstop_x_left.read() == 1)
    {
      // move stepper right
      mail->dir_m1 = DIR_LEFT_nRIGHT;
      mail->step_m1 = 1;
    }
    else
    {
      mail->step_m1 = 0;
    }
    if (endstop_y_bottom.read() == 1)
    {
      // move stepper down
      mail->dir_m2 = DIR_UP_nDOWN;
      mail->step_m2 = 1;
    }
    else
    {
      mail->step_m2 = 0;
    }

    expander_mail.put(mail);
    osThreadYield();

  }

  int size_x = 0;
  int size_y = 0;
  while (endstop_x_right.read() == 1 || endstop_y_top.read() == 1)
  {
    if (endstop_x_right.read() == 1)
    {
      // move stepper right
      mail->dir_m1 = !DIR_LEFT_nRIGHT;
      mail->step_m1 = 1;
      size_x++;
    }
    else
    {
      mail->step_m1 = 0;
    }
    if (endstop_y_top.read() == 1)
    {
      // move stepper down
      mail->dir_m2 = !DIR_UP_nDOWN;
      mail->step_m2 = 1;
      size_y++;
    }
    else
    {
      mail->step_m2 = 0;
    }

    expander_mail.put(mail);
    osThreadYield();
  }

  plotter_mail_t *mail_p = plotter_mail.alloc();
  mail_p->size_x = size_x;
  mail_p->size_y = size_y;
  plotter_mail.put(mail_p);

  pc.printf("size_x: %d\n", size_x);
  pc.printf("size_y: %d\n", size_y);
}


void callback_rx ()
{
  static int cnt = 0;
  static bool receiving = FALSE;
  int buff = pc.getc();
  if (buff == '$')
  {
    cnt = 0;
    receiving = TRUE;
    pc_mail_t * mail = pc_mail.alloc();
  }
  if (receiving == TRUE)
  {
    mail->received[cnt] = buff;
    cnt++;
    if (buff == '#')
    {
      receiving = FALSE;
      pc_mail.put(mail);
    }
  }
}


void com_pc ()
{
  pc.printf("$ready#");
  pc.attach(&callback_rx);
  while (1)
  {
    osEvent evt = pc_mail.get();
    if (evt.status == osEventMail)
    {
      pc_mail_t * mail = (pc_mail_t *) evt.value.p;
      
    }
  }
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
int main (void)
{
  pc.printf("Starting programm\n");
  #if DEBUG_ENDPOINTS
  endpoint_indicators();
  #endif
  modules_init();
  thread_com_plotter.start (com_plotter);
  thread_com_plotter.set_priority (osPriorityRealtime);
  osThreadSetPriority(osThreadGetId(), osPriorityIdle);
  pos_init_plotter();

  led2 = !led2;

  thread_com_pc.start (com_pc);

	while (1) {
		
	}
}
/*EOF*/
