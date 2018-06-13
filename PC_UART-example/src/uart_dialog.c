#include <uart_dialog.h>

static void set_new_status (struct my_widgets *wid);
static void create_device_combo_list (GtkComboBoxText *combo);
static void create_baudrate_combo_list (GtkComboBoxText *combo); 



gchar dev[][DEVLEN] =  {"/dev/ttyUSB0",
                        "/dev/ttyUSB1"};

gint baud_rate[] = {9600,
                    19200,
                    38400,
                    57600,
                    115200};


/**
 * @brief      { function_description }
 *
 * @param[in]  data  The data
 * @param      wid   The wid
 */
void initial_uart_config (gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  wid->selected_device = dev[0];        // /dev/ttyUSB0
  wid->device_baudrate = baud_rate[0];  // 9600
  wid->status_hwcheck = HWCHECKOFF;
  wid->status_swcheck = SWCHECKOFF;
  wid->device_databits = 8;
  wid->device_stopbits = 1;
  wid->idd = 0;
  wid->idb = 0;
}


/**
 * @brief      { function_description }
 *
 * @param      uart_dialog       The uart dialog
 * @param[in]  uart_response_id  The uart response identifier
 * @param[in]  data              The data
 */
void uart_response (GtkDialog *uart_dialog, gint uart_response_id, gpointer data)
{
  struct my_widgets *wid = (struct my_widgets *) data;

  if (uart_response_id == GTK_RESPONSE_ACCEPT)
  {
    set_new_status (wid);
    g_mutex_lock (&wid->uart_configure_mutex);
    wid->uart_configured = 1;
    g_cond_signal (&wid->uart_configure_cond);
    g_mutex_unlock (&wid->uart_configure_mutex);
    gtk_widget_destroy (GTK_WIDGET (wid->uart_button));
  }
  gtk_widget_destroy (GTK_WIDGET (wid->uart_dialog));
}


/**
 * @brief      { function_description }
 *
 * @param      widget  The widget
 * @param[in]  data    The data
 */
void uart_dialog_cb (GtkWidget *widget, gpointer data)
{
  GtkWidget *grid;
  GtkWidget *device_label, *baudrate_label, *parity_label;
  GtkWidget *databits_label, *stopbits_label, *hs_label;
  //GtkWidget *confirm_button, *cancel_button;
  GtkWidget *content_area;
  GtkWidget *ok_button, *cancel_button;
  GtkAdjustment *databits_adjustment, *stopbits_adjustment;

  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

  struct my_widgets *wid = (struct my_widgets *) data;

  wid->uart_dialog = gtk_dialog_new_with_buttons ("UART Configuration",
                                                  GTK_WINDOW (wid->window),
                                                  flags,
                                                  "_Cancel",
                                                  GTK_RESPONSE_REJECT,
                                                  "_OK",
                                                  GTK_RESPONSE_ACCEPT,
                                                  NULL);

  //gtk_window_set_default_size (GTK_WINDOW (wid->uart_dialog), 480, 100);

  ok_button = gtk_dialog_get_widget_for_response (GTK_DIALOG (wid->uart_dialog), GTK_RESPONSE_ACCEPT);
  gtk_widget_set_size_request (GTK_WIDGET(ok_button), 230, 30);

  cancel_button = gtk_dialog_get_widget_for_response (GTK_DIALOG (wid->uart_dialog), GTK_RESPONSE_REJECT);
  gtk_widget_set_size_request (GTK_WIDGET(cancel_button), 230, 30);

  content_area = gtk_dialog_get_content_area (GTK_DIALOG(wid->uart_dialog));

  grid = gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
  gtk_grid_set_column_spacing (GTK_GRID (grid), 5);
  gtk_container_add (GTK_CONTAINER(content_area), grid);
  gtk_container_set_border_width (GTK_CONTAINER (content_area), 5);


  device_label = gtk_label_new ("Device");
  gtk_grid_attach (GTK_GRID (grid), device_label, 0, 0, 1, 1);
  wid->device_combo = gtk_combo_box_text_new_with_entry();
  create_device_combo_list (GTK_COMBO_BOX_TEXT (wid->device_combo));
  gtk_combo_box_set_active (GTK_COMBO_BOX (wid->device_combo), wid->idd);
  gtk_grid_attach (GTK_GRID (grid), wid->device_combo, 1, 0, 1, 1);

  baudrate_label = gtk_label_new ("Baudrate");
  gtk_grid_attach (GTK_GRID(grid), baudrate_label, 0, 1, 1, 1);
  wid->baudrate_combo = gtk_combo_box_text_new_with_entry();
  create_baudrate_combo_list (GTK_COMBO_BOX_TEXT (wid->baudrate_combo));
  gtk_combo_box_set_active (GTK_COMBO_BOX (wid->baudrate_combo), wid->idb);
  gtk_grid_attach (GTK_GRID (grid), wid->baudrate_combo, 1, 1, 1, 1);

  parity_label = gtk_label_new ("Parity");
  gtk_grid_attach (GTK_GRID(grid), parity_label, 0, 2, 1, 1);
  wid->parity_combo = gtk_combo_box_text_new_with_entry();
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (wid->parity_combo), "NONE");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (wid->parity_combo), "EVEN");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (wid->parity_combo), "ODD");
  if (wid->status_parity == NONE)
    gtk_combo_box_set_active (GTK_COMBO_BOX (wid->parity_combo), 0);
  if (wid->status_parity == EVEN)
    gtk_combo_box_set_active (GTK_COMBO_BOX (wid->parity_combo), 1);
  if (wid->status_parity == ODD)
    gtk_combo_box_set_active (GTK_COMBO_BOX (wid->parity_combo), 2);
  gtk_grid_attach (GTK_GRID (grid), wid->parity_combo, 1, 2, 1, 1);


  databits_label = gtk_label_new ("# of Databits");
  gtk_grid_attach (GTK_GRID (grid), databits_label, 2, 0, 1, 1);
  databits_adjustment = gtk_adjustment_new (8, 5, 8, 1, 1, 0);
  wid->databits_spinb = gtk_spin_button_new (databits_adjustment, 1, 0);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (wid->databits_spinb), wid->device_databits);
  gtk_grid_attach (GTK_GRID (grid), wid->databits_spinb, 3, 0, 2, 1);

  stopbits_label = gtk_label_new ("# of Stopbits");
  gtk_grid_attach (GTK_GRID (grid), stopbits_label, 2, 1, 1, 1);
  stopbits_adjustment = gtk_adjustment_new (1, 1, 2, 1, 1, 0);
  wid->stopbits_spinb = gtk_spin_button_new (stopbits_adjustment, 1, 0);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (wid->stopbits_spinb), wid->device_stopbits);
  gtk_grid_attach (GTK_GRID (grid), wid->stopbits_spinb, 3, 1, 2, 1);


  hs_label = gtk_label_new ("Handshake");
  gtk_grid_attach (GTK_GRID (grid), hs_label, 2, 2, 1, 1);
  // Hardware
  wid->hw_check = gtk_check_button_new_with_label ("HW");
  if (wid->status_hwcheck == HWCHECKON)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wid->hw_check), TRUE);
  if (wid->status_hwcheck == HWCHECKOFF)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wid->hw_check), FALSE);
  gtk_grid_attach (GTK_GRID (grid), wid->hw_check, 3, 2, 1, 1);
  // Software
  wid->sw_check = gtk_check_button_new_with_label ("SW");
  if (wid->status_swcheck == SWCHECKON)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wid->sw_check), TRUE);
  if (wid->status_swcheck == SWCHECKOFF)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wid->sw_check), FALSE);
  gtk_grid_attach (GTK_GRID (grid), wid->sw_check, 4, 2, 1, 1);


  gtk_widget_show_all (wid->uart_dialog);
  g_signal_connect (GTK_DIALOG (wid->uart_dialog), "response", G_CALLBACK (uart_response), wid);

}


/**
 * @brief      Sets the new status.
 *
 * @param      wid   The wid
 */
static void set_new_status (struct my_widgets *wid)
{
  gchar *baudrate_value;
  gchar *parity_value;
  gchar *endptr;

  wid->selected_device = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (wid->device_combo));
  for (gint i = 0; i < sizeof (dev) / DEVLEN * sizeof (gchar); i++)
  {
    if (g_strcmp0 (wid->selected_device, dev[i]) == 0)
      wid->idd = i;
  }

  baudrate_value = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (wid->baudrate_combo));
  wid->device_baudrate = g_ascii_strtoll (baudrate_value, &endptr, 10);
  for (gint i = 0; i < sizeof (baud_rate) * sizeof (gint); i++)
  {
    if (wid->device_baudrate == baud_rate[i])
      wid->idb = i;
  }

  parity_value = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (wid->parity_combo));
  if (g_strcmp0 (parity_value, "ODD") == 0)
    wid->status_parity = ODD;
  if (g_strcmp0 (parity_value, "EVEN") == 0)
    wid->status_parity = EVEN;
  if (g_strcmp0 (parity_value, "NONE") == 0)
    wid->status_parity = NONE;

  wid->device_databits = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (wid->databits_spinb));
  wid->device_stopbits = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (wid->stopbits_spinb));

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid->hw_check)))
    wid->status_hwcheck = HWCHECKON;
  else
    wid->status_hwcheck = HWCHECKOFF;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid->sw_check)))
    wid->status_swcheck = SWCHECKON;
  else
    wid->status_swcheck = SWCHECKOFF;
}


/**
 * @brief      Creates a device combo list.
 *
 * @param      combo  The combo
 */
static void create_device_combo_list (GtkComboBoxText *combo)
{
  gchar devstr[20];

  for (gint i = 0; i < sizeof (dev) / 15 * sizeof (gchar); i++)
  {
    g_sprintf (devstr, "%s", dev[i]);
    gtk_combo_box_text_append_text (combo, devstr);
  }
}


/**
 * @brief      Creates a baudrate combo list.
 *
 * @param      combo  The combo
 */
static void create_baudrate_combo_list (GtkComboBoxText *combo)
{
  gchar bdrstr[10];

  for (gint i = 0; i < sizeof (baud_rate) / sizeof (gint); i++)
  {
    g_sprintf (bdrstr, "%d", baud_rate[i]);
    gtk_combo_box_text_append_text (combo, bdrstr);
  }
}