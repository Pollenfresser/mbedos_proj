#ifndef _MAIN_
#define _MAIN_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>

#include <responses_led.h>
#include <uart_dialog.h>
#include <uart_com.h>
#include <rs232.h>

#define DEBUG 1

#define DEVLEN 15

enum parity {NONE, EVEN, ODD};
enum hwcheck {HWCHECKON, HWCHECKOFF};
enum swcheck {SWCHECKON, SWCHECKOFF};



extern gchar dev[][DEVLEN];

extern gint baud_rate[];

struct my_widgets {
  GtkWidget *window;
  GtkWidget *label_status;
  GtkWidget *input_entry1;
  GtkWidget *input_entry2;
  GtkWidget *uart_button;

  // UART specific
  GtkWidget *uart_dialog;
  GtkWidget *device_combo;
  GtkWidget *baudrate_combo;
  GtkWidget *parity_combo;
  GtkWidget *databits_spinb;
  GtkWidget *stopbits_spinb;
  GtkWidget *hw_check;
  GtkWidget *sw_check;
  gchar *selected_device;
  gint device_baudrate;
  gint device_databits;
  gint device_stopbits;
  enum parity status_parity;
  enum hwcheck status_hwcheck;
  enum swcheck status_swcheck;
  gboolean inital;
  gint idd;
  gint idb;

  gint uart_configured;
  GMutex uart_configure_mutex;
  GCond uart_configure_cond;
  gint uart_ready;

  gchar *uart_data;
};

uint16_t check_numeric(gchar *buffer);


#endif