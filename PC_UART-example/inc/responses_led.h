#ifndef _RESPONSES_PWM_
#define _RESPONSES_PWM_

#include <main.h>

void error_dialog_cb (GtkDialog *err_dialog, gint err_response_id, gpointer data);
void ok_clicked (GtkWidget *widget, gpointer data);
void stop_clicked (GtkWidget *widget, gpointer data);

#endif