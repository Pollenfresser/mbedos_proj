#include <uart_com.h>

static int uart_initialization (gpointer data);
static gboolean uart_send (gpointer data);


/**
 * @brief      { function_description }
 *
 * @param[in]  data  The data
 *
 * @return     { description_of_the_return_value }
 */
gpointer uart_thread (gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  if (!uart_initialization (wid))
    g_error("Serial device could not be opened!\n");

  g_timeout_add (500, (GSourceFunc) uart_send, (gpointer) wid);
}



/**
 * @brief      { function_description }
 *
 * @param[in]  data  The data
 *
 * @return     { description_of_the_return_value }
 */
static gboolean uart_send (gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  if (wid->uart_data != NULL)
  {
    RS232_cputs ((int) wid->idd, (char *) wid->uart_data);
    g_free (wid->uart_data);
    wid->uart_data = NULL;
  }

  return TRUE;
}


/**
 * @brief      { function_description }
 *
 * @param[in]  data  The data
 *
 * @return     { description_of_the_return_value }
 */
static int uart_initialization (gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  g_mutex_lock (&wid->uart_configure_mutex);
  while (!wid->uart_configured)
    g_cond_wait (&wid->uart_configure_cond, &wid->uart_configure_mutex);
  g_mutex_unlock (&wid->uart_configure_mutex);

  #if DEBUG
  printf("Device: %s\n", dev[wid->idd]);
  printf("Baudrate: %d\n", baud_rate[wid->idb]);
  printf("Databits: %d\n", wid->device_databits);
  printf("Stopbits: %d\n", wid->device_stopbits);
  printf("Parity: %d\n", wid->status_parity);
  #endif

  if (RS232_OpenComport(wid))
  {
    printf("Could not open comport!\n");
    return 0;
  }
  else
  {
    printf("Comport open!\n");
    wid->uart_ready = 1;
    return 1;
  }
}