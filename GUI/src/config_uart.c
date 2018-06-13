/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 11.06.2018
 *
 * File description: Laptop to Plotter via UART
 *
 * Status: not working
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

static void set_new_status(widgets *a);
static void create_device_combo_list(GtkComboBoxText *combo);
static void create_baudrate_combo_list(GtkComboBoxText *combo);

gchar dev[][DEVLEN] = { "/dev/ttyUSB0", "/dev/ttyUSB1" };

gint baud_rate[] = { 9600, 19200, 38400, 57600, 115200 };

/**
 * @brief      { function_description }
 *
 * @param[in]  data  The data
 * @param      wid   The wid
 */
void initial_uart_config(gpointer data) {
	widgets *a = (widgets *) data;

	a->uart.selected_device = dev[0];        // /dev/ttyUSB0
	a->uart.device_baudrate = baud_rate[0];  // 9600
	a->uart.status_hwcheck = HWCHECKOFF;
	a->uart.status_swcheck = SWCHECKOFF;
	a->uart.device_databits = 8;
	a->uart.device_stopbits = 1;
	a->uart.idd = 0;
	a->uart.idb = 0;
}

/**
 * @brief      { function_description }
 *
 * @param      uart_dialog       The uart dialog
 * @param[in]  uart_response_id  The uart response identifier
 * @param[in]  data              The data
 */
void uart_response(GtkDialog *uart_dialog, gint uart_response_id, gpointer data) {
	widgets *a = (widgets *) data;

	if (uart_response_id == GTK_RESPONSE_ACCEPT) {
		set_new_status(a);
		g_mutex_lock(&a->uart.uart_configure_mutex);
		a->uart.uart_configured = 1;
		g_cond_signal(&a->uart.uart_configure_cond);
		g_mutex_unlock(&a->uart.uart_configure_mutex);
	}
	gtk_widget_destroy(GTK_WIDGET(a->uart.uart_dialog));
}

/**
 * @brief      { function_description }
 *
 * @param      widget  The widget
 * @param[in]  data    The data
 */
void uart_dialog_cb(gpointer data) {

	widgets *a = (widgets *) data;

	GtkWidget *grid;
	GtkWidget *device_label, *baudrate_label, *parity_label;
	GtkWidget *databits_label, *stopbits_label, *hs_label;
	//GtkWidget *confirm_button, *cancel_button;
	GtkWidget *content_area;
	GtkWidget *ok_button, *cancel_button;
	GtkAdjustment *databits_adjustment, *stopbits_adjustment;

	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;



	a->uart.uart_dialog = gtk_dialog_new_with_buttons("UART Configuration",
			GTK_WINDOW(a->window), flags, "_Cancel", GTK_RESPONSE_REJECT,
			"_OK", GTK_RESPONSE_ACCEPT, NULL);

	//gtk_window_set_default_size (GTK_WINDOW (a->uart.uart_dialog), 480, 100);

	ok_button = gtk_dialog_get_widget_for_response(GTK_DIALOG(a->uart.uart_dialog),
			GTK_RESPONSE_ACCEPT);
	gtk_widget_set_size_request(GTK_WIDGET(ok_button), 230, 30);

	cancel_button = gtk_dialog_get_widget_for_response(
			GTK_DIALOG(a->uart.uart_dialog), GTK_RESPONSE_REJECT);
	gtk_widget_set_size_request(GTK_WIDGET(cancel_button), 230, 30);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(a->uart.uart_dialog));

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_container_add(GTK_CONTAINER(content_area), grid);
	gtk_container_set_border_width(GTK_CONTAINER(content_area), 5);

	device_label = gtk_label_new("Device");
	gtk_grid_attach(GTK_GRID(grid), device_label, 0, 0, 1, 1);
	a->uart.device_combo = gtk_combo_box_text_new_with_entry();
	create_device_combo_list(GTK_COMBO_BOX_TEXT(a->uart.device_combo));
	gtk_combo_box_set_active(GTK_COMBO_BOX(a->uart.device_combo), a->uart.idd);
	gtk_grid_attach(GTK_GRID(grid), a->uart.device_combo, 1, 0, 1, 1);

	baudrate_label = gtk_label_new("Baudrate");
	gtk_grid_attach(GTK_GRID(grid), baudrate_label, 0, 1, 1, 1);
	a->uart.baudrate_combo = gtk_combo_box_text_new_with_entry();
	create_baudrate_combo_list(GTK_COMBO_BOX_TEXT(a->uart.baudrate_combo));
	gtk_combo_box_set_active(GTK_COMBO_BOX(a->uart.baudrate_combo), a->uart.idb);
	gtk_grid_attach(GTK_GRID(grid), a->uart.baudrate_combo, 1, 1, 1, 1);

	parity_label = gtk_label_new("Parity");
	gtk_grid_attach(GTK_GRID(grid), parity_label, 0, 2, 1, 1);
	a->uart.parity_combo = gtk_combo_box_text_new_with_entry();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(a->uart.parity_combo),
			"NONE");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(a->uart.parity_combo),
			"EVEN");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(a->uart.parity_combo),
			"ODD");
	if (a->uart.status_parity == NONE)
		gtk_combo_box_set_active(GTK_COMBO_BOX(a->uart.parity_combo), 0);
	if (a->uart.status_parity == EVEN)
		gtk_combo_box_set_active(GTK_COMBO_BOX(a->uart.parity_combo), 1);
	if (a->uart.status_parity == ODD)
		gtk_combo_box_set_active(GTK_COMBO_BOX(a->uart.parity_combo), 2);
	gtk_grid_attach(GTK_GRID(grid), a->uart.parity_combo, 1, 2, 1, 1);

	databits_label = gtk_label_new("# of Databits");
	gtk_grid_attach(GTK_GRID(grid), databits_label, 2, 0, 1, 1);
	databits_adjustment = gtk_adjustment_new(8, 5, 8, 1, 1, 0);
	a->uart.databits_spinb = gtk_spin_button_new(databits_adjustment, 1, 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(a->uart.databits_spinb),
			a->uart.device_databits);
	gtk_grid_attach(GTK_GRID(grid), a->uart.databits_spinb, 3, 0, 2, 1);

	stopbits_label = gtk_label_new("# of Stopbits");
	gtk_grid_attach(GTK_GRID(grid), stopbits_label, 2, 1, 1, 1);
	stopbits_adjustment = gtk_adjustment_new(1, 1, 2, 1, 1, 0);
	a->uart.stopbits_spinb = gtk_spin_button_new(stopbits_adjustment, 1, 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(a->uart.stopbits_spinb),
			a->uart.device_stopbits);
	gtk_grid_attach(GTK_GRID(grid), a->uart.stopbits_spinb, 3, 1, 2, 1);

	hs_label = gtk_label_new("Handshake");
	gtk_grid_attach(GTK_GRID(grid), hs_label, 2, 2, 1, 1);
	// Hardware
	a->uart.hw_check = gtk_check_button_new_with_label("HW");
	if (a->uart.status_hwcheck == HWCHECKON)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(a->uart.hw_check), TRUE);
	if (a->uart.status_hwcheck == HWCHECKOFF)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(a->uart.hw_check), FALSE);
	gtk_grid_attach(GTK_GRID(grid), a->uart.hw_check, 3, 2, 1, 1);
	// Software
	a->uart.sw_check = gtk_check_button_new_with_label("SW");
	if (a->uart.status_swcheck == SWCHECKON)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(a->uart.sw_check), TRUE);
	if (a->uart.status_swcheck == SWCHECKOFF)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(a->uart.sw_check), FALSE);
	gtk_grid_attach(GTK_GRID(grid), a->uart.sw_check, 4, 2, 1, 1);

	gtk_widget_show_all(a->uart.uart_dialog);
	g_signal_connect(GTK_DIALOG(a->uart.uart_dialog), "response",
			G_CALLBACK(uart_response), a);

}

/**
 * @brief      Sets the new status.
 *
 * @param      wid   The wid
 */
static void set_new_status(widgets *a) {
	gchar *baudrate_value;
	gchar *parity_value;
	gchar *endptr;

	a->uart.selected_device = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(a->uart.device_combo));
	for (gint i = 0; i < sizeof(dev) / DEVLEN * sizeof(gchar); i++) {
		if (g_strcmp0(a->uart.selected_device, dev[i]) == 0)
			a->uart.idd = i;
	}

	baudrate_value = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(a->uart.baudrate_combo));
	a->uart.device_baudrate = g_ascii_strtoll(baudrate_value, &endptr, 10);
	for (gint i = 0; i < sizeof(baud_rate) * sizeof(gint); i++) {
		if (a->uart.device_baudrate == baud_rate[i])
			a->uart.idb = i;
	}

	parity_value = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(a->uart.parity_combo));
	if (g_strcmp0(parity_value, "ODD") == 0)
		a->uart.status_parity = ODD;
	if (g_strcmp0(parity_value, "EVEN") == 0)
		a->uart.status_parity = EVEN;
	if (g_strcmp0(parity_value, "NONE") == 0)
		a->uart.status_parity = NONE;

	a->uart.device_databits = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(a->uart.databits_spinb));
	a->uart.device_stopbits = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(a->uart.stopbits_spinb));

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(a->uart.hw_check)))
		a->uart.status_hwcheck = HWCHECKON;
	else
		a->uart.status_hwcheck = HWCHECKOFF;

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(a->uart.sw_check)))
		a->uart.status_swcheck = SWCHECKON;
	else
		a->uart.status_swcheck = SWCHECKOFF;
}

/**
 * @brief      Creates a device combo list.
 *
 * @param      combo  The combo
 */
static void create_device_combo_list(GtkComboBoxText *combo) {
	gchar devstr[20];

	for (gint i = 0; i < sizeof(dev) / 15 * sizeof(gchar); i++) {
		g_sprintf(devstr, "%s", dev[i]);
		gtk_combo_box_text_append_text(combo, devstr);
	}
}

/**
 * @brief      Creates a baudrate combo list.
 *
 * @param      combo  The combo
 */
static void create_baudrate_combo_list(GtkComboBoxText *combo) {
	gchar bdrstr[10];

	for (gint i = 0; i < sizeof(baud_rate) / sizeof(gint); i++) {
		g_sprintf(bdrstr, "%d", baud_rate[i]);
		gtk_combo_box_text_append_text(combo, bdrstr);
	}
}

