
#include <main.h>




/**
 * @brief      { function_description }
 *
 * @param      buffer  The buffer
 *
 * @return     { description_of_the_return_value }
 */
uint16_t check_numeric(gchar *buffer)
{
  #if DEBUG
  printf("received buffer: %s\n", buffer);
  #endif
  for(uint16_t counter = 0; counter < strlen(buffer); counter++)
  {
    #if DEBUG
    printf("Current buff char: %c\n", (char)buffer[counter]);
    #endif
    if(g_ascii_isdigit(buffer[counter]) == 0)
    {
      #if DEBUG
      printf("check_numeric, returning 0\n");
      #endif
      return 0;
    }
  }
  #if DEBUG
  printf("check_numeric, returning 1\n");
  #endif
  return 1;
}



/**
 * @brief      { function_description }
 *
 * @param      app        The application
 * @param[in]  user_data  The user data
 */
static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *grid;
  GtkWidget *label_output;
  GtkWidget *label_led1, *label_led2;
  GtkWidget *stop_button, *ok_button;
  struct my_widgets *wid = (struct my_widgets *) user_data;

  // Creation of the window
  wid->window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW(wid->window), "PWM-Control");
  gtk_window_set_resizable (GTK_WINDOW(wid->window), TRUE);
  gtk_window_set_position (GTK_WINDOW(wid->window), GTK_WIN_POS_CENTER);

  
  /**
  * Splitting it in parts and creating various entrys
  */
  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_container_add(GTK_CONTAINER(wid->window), grid);

  // labels
  label_output = gtk_label_new("Enter the respective LED's-Dutycycle below");
  gtk_widget_set_size_request (GTK_WIDGET(label_output), 120, 30);
  gtk_grid_attach (GTK_GRID(grid), label_output, 0, 0, 2, 1);

  label_led1 = gtk_label_new("LED1-Duty:");
  gtk_widget_set_size_request (GTK_WIDGET(label_led1), 60, 30);
  gtk_grid_attach (GTK_GRID(grid), label_led1, 0, 1, 1, 1);

  label_led2 = gtk_label_new("LED2-Duty:");
  gtk_widget_set_size_request (GTK_WIDGET(label_led2), 60, 30);
  gtk_grid_attach (GTK_GRID(grid), label_led2, 0, 2, 1, 1);

  wid->label_status = gtk_label_new("Ready to execute!");
  gtk_widget_set_size_request (GTK_WIDGET(wid->label_status), 120, 30);
  gtk_grid_attach (GTK_GRID(grid), wid->label_status, 0, 5, 2, 1);


  // text entrys
  wid->input_entry1 = gtk_entry_new();
  gtk_grid_attach (GTK_GRID(grid), wid->input_entry1, 1, 1, 1, 1);
  gtk_widget_set_size_request (GTK_WIDGET(wid->input_entry1), 60, 30);
  g_signal_connect (wid->input_entry1, "activate", G_CALLBACK(ok_clicked), (gpointer) wid);

  wid->input_entry2 = gtk_entry_new();
  gtk_grid_attach (GTK_GRID(grid), wid->input_entry2, 1, 2, 1, 1);
  gtk_widget_set_size_request (GTK_WIDGET(wid->input_entry2), 60, 30);
  g_signal_connect (wid->input_entry2, "activate", G_CALLBACK(ok_clicked), (gpointer) wid);

  // buttons
  stop_button = gtk_button_new_with_mnemonic("_Stop");
  gtk_grid_attach (GTK_GRID(grid), stop_button, 0, 3, 1, 1);
  gtk_widget_set_size_request (GTK_WIDGET(stop_button), 60, 30);
  g_signal_connect (stop_button, "clicked", G_CALLBACK(stop_clicked), (gpointer) wid);

  ok_button = gtk_button_new_with_mnemonic ("_Ok");
  gtk_grid_attach (GTK_GRID(grid), ok_button, 1, 3, 1, 1);
  gtk_widget_set_size_request (GTK_WIDGET(ok_button), 60, 30);
  g_signal_connect (ok_button, "clicked", G_CALLBACK(ok_clicked), (gpointer) wid);

  wid->uart_button = gtk_button_new_with_mnemonic ("_UART Setup");
  gtk_grid_attach (GTK_GRID(grid), wid->uart_button, 0, 4, 2, 1);
  gtk_widget_set_size_request (GTK_WIDGET(wid->uart_button), 120, 30);
  g_signal_connect (wid->uart_button, "clicked", G_CALLBACK(uart_dialog_cb), (gpointer) wid);

  initial_uart_config (wid);

  
  gtk_widget_show_all (GTK_WIDGET(wid->window));
}


/**
 * @brief      { function_description }
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv
 *
 * @return     { description_of_the_return_value }
 */
int main (int argc, char ** argv)
{
  GThread *uart_com;
  GtkApplication *app;
  int status;

  struct my_widgets *wid = g_malloc(sizeof (struct my_widgets));
  wid->uart_configured = 0;
  wid->uart_ready = 0;

  uart_com = g_thread_new ("UART_COM", (GThreadFunc) uart_thread, wid);

  app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK(activate), (gpointer) wid);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref (app);

  g_free(wid);
  wid = NULL;

  return status;
}