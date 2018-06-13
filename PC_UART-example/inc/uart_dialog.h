#ifndef _UART_DIALOG_
#define _UART_DIALOG_

#include <main.h>

void initial_uart_config (gpointer data);
void uart_response (GtkDialog *uart_dialog, gint uart_response_id, gpointer data);
void uart_dialog_cb (GtkWidget *widget, gpointer data);


#endif