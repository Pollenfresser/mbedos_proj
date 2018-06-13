#include <responses_led.h>


/**
 * @brief      { function_description }
 *
 * @param      widget       The widget
 * @param[in]  response_id  The response identifier
 * @param[in]  data         The data
 */
void error_response_cb (GtkDialog *err_dialog, gint err_response_id, gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  gtk_label_set_text (GTK_LABEL(wid->label_status), "Execution stopped!");
  gtk_widget_destroy (GTK_WIDGET(err_dialog));
}


/**
 * @brief      { function_description }
 *
 * @param      widget  The widget
 * @param[in]  data    The data
 */
void ok_clicked (GtkWidget *widget, gpointer data)
{
  GtkWidget *error_message;
  gchar *buffer_led1, *buffer_led2;
  struct my_widgets *wid = (struct my_widgets *) data;

  #if DEBUG
  printf("entered ok_clicked\n");
  #endif

  buffer_led1 = (gchar *) gtk_entry_get_text (GTK_ENTRY(wid->input_entry1));
  buffer_led2 = (gchar *) gtk_entry_get_text (GTK_ENTRY(wid->input_entry2));
  if(check_numeric(buffer_led1) == 0 || check_numeric(buffer_led2) == 0 ||
      wid->uart_ready == 0)
  {
    //***** Error
    #if DEBUG
    printf("Non numeric entry!\n");
    #endif
    error_message = gtk_message_dialog_new(GTK_WINDOW(wid->window),
                                            GTK_DIALOG_MODAL,
                                            GTK_MESSAGE_WARNING,
                                            GTK_BUTTONS_OK,
                                            "At least one entry was non numeric or UART was not connected!\nStopping execution");
    gtk_widget_show_all (error_message);
    g_signal_connect(GTK_DIALOG(error_message), "response", G_CALLBACK(error_response_cb), (gpointer) wid);

    wid->uart_data = g_malloc0 (strlen ("#LEDx:0$\0") * sizeof (gchar));
    g_stpcpy (wid->uart_data, "#LEDx:0$\0");
    return;
  }
  else
  {
    gint64 buff = 0;
    gchar *endptr;
    //***** Successfull, transmit data
    if (strlen (buffer_led1) != 0)
    {
      // TODO conversion to int, check size, put string together
      buff = g_ascii_strtoll (buffer_led1, &endptr, 10);
      if (buff > 100)
        buff = 100;
      else if (buff < 0)
        buff = 0;

      while(wid->uart_data != NULL);
      wid->uart_data = g_malloc0 (strlen ("#LED1:xxx$\0") * sizeof (gchar));
      g_sprintf (wid->uart_data, "#LED1:%d$", (int) buff);

    }
    if (strlen (buffer_led2) != 0)
    {
      buff = g_ascii_strtoll (buffer_led2, &endptr, 10);
      if (buff > 100)
        buff = 100;
      else if (buff < 0)
        buff = 0;

      while(wid->uart_data != NULL);
      wid->uart_data = g_malloc0 (strlen ("#LED2:xxx$\0") * sizeof (gchar));
      g_sprintf (wid->uart_data, "#LED2:%d$", (int) buff);
    }
    gtk_label_set_text (GTK_LABEL(wid->label_status), "Executing!");
  }
}



/**
 * @brief      { function_description }
 *
 * @param      widget  The widget
 * @param[in]  data    The data
 */
void stop_clicked (GtkWidget *widget, gpointer data)
{
  GtkStyleProvider *provider;
  struct my_widgets *wid = (struct my_widgets *) data;

  gtk_entry_set_text (GTK_ENTRY(wid->input_entry1), "");
  gtk_entry_set_text (GTK_ENTRY(wid->input_entry2), "");
  gtk_label_set_text (GTK_LABEL(wid->label_status), "Execution stopped!");

  wid->uart_data = g_malloc0 (strlen ("#LEDx:0$\0") * sizeof (gchar));
  g_stpcpy (wid->uart_data, "#LEDx:0$\0");
}