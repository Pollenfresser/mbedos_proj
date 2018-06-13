/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Menu
 *
 * Status: working
 * (application.out:5503): GLib-GObject-CRITICAL **: g_object_unref: assertion 'G_IS_OBJECT (object)' failed
 *  Image does not exist
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

const GActionEntry app_entries[] = {
		{ "open_file", menu_callback_open_file, NULL, NULL, NULL, { 0, 0, 0 } },
		{ "uart_config", menu_callback_uart_config, NULL, NULL, NULL, { 0, 0, 0 } },
		{ "about", menu_callback_about, NULL, NULL, NULL, { 0, 0, 0 } },
		{ "quit", menu_callback_quit, NULL, NULL, NULL, { 0, 0, 0 } } };


void menu_init(gpointer data) {
	widgets *a = (widgets *) data;

	GMenu *menu, *menu_open_file, *menu_uart_config, *menu_about, *menu_quit;

	// keyboard accelerators
	const gchar *accels_open_file[8] = { "<Ctrl>f", NULL };
	const gchar *accels_about[2] = { "F1", NULL };
	const gchar *accels_quit[8] = { "<Ctrl>x", NULL };

	// map entries and actions *****
	g_action_map_add_action_entries(G_ACTION_MAP(a->app), app_entries,
			G_N_ELEMENTS(app_entries), (gpointer) a);

	// create the menu *****
	menu = g_menu_new();

	// create menu open file *****
	menu_open_file = g_menu_new();
	g_menu_append (menu_open_file, "Open File", "app.open_file"); // open file
	g_menu_insert_submenu (menu, 0, "File", G_MENU_MODEL (menu_open_file));
	g_object_unref(menu_open_file);

	// create menu open file *****
	menu_uart_config = g_menu_new();
	g_menu_append (menu_uart_config, "UART", "app.uart_config"); // open file
	g_menu_append_submenu (menu, "Connect", G_MENU_MODEL (menu_uart_config));
	g_object_unref(menu_uart_config);


	// create menu about *****
	menu_about = g_menu_new();
	g_menu_append (menu_about, "About", "app.about"); // about
	g_menu_append_submenu(menu, "About", G_MENU_MODEL (menu_about));
	g_object_unref(menu_about);

	// create menu quit ****
	menu_quit = g_menu_new();
	g_menu_append (menu_quit, "Quit", "app.quit"); // quit
	g_menu_append_submenu(menu, "Quit", G_MENU_MODEL (menu_quit));
	g_object_unref(menu_quit);

	// add menus to menu bar *****
	a->menu.menubar = gtk_menu_bar_new_from_model(G_MENU_MODEL(menu));
	gtk_box_pack_start(GTK_BOX(a->main_box), a->menu.menubar, FALSE, FALSE, 0);

	// connect keyboard accelerators *****
	gtk_application_set_accels_for_action(GTK_APPLICATION(a->app), "app.open_file", accels_open_file);
	gtk_application_set_accels_for_action(GTK_APPLICATION(a->app), "app.about", accels_about);
	gtk_application_set_accels_for_action(GTK_APPLICATION(a->app), "app.quit", accels_quit);
}

void menu_callback_open_file(GSimpleAction *action, GVariant *parameter,
		gpointer data) {
	widgets *a = (widgets *) data;
	main_open_file ((gpointer) a);
}

void menu_callback_uart_config(GSimpleAction *action, GVariant *parameter,
		gpointer data) {
	widgets *a = (widgets *) data;
	main_uart_config ((gpointer) a);
}

void menu_callback_about(GSimpleAction *action, GVariant *parameter,
		gpointer data) {
	widgets *a = (widgets *) data;
	GdkPixbuf *pixbuf;
	GtkWidget *about_dialog;
	const gchar *authors[] = { "Christina Bornberg", "Lucas Ullrich", NULL };

	// Image
	pixbuf = gdk_pixbuf_new_from_file("res/logo.png", NULL) ;

	// Layout
	about_dialog = gtk_about_dialog_new();

	gtk_show_about_dialog(GTK_WINDOW(a->window), "program-name", "mbedos proj",
			"version", "1.0", "copyright", "Copyright \xc2\xa9 EleVariation",
			"comments", "An application by some beginner-developers", "authors",
			authors, "logo", pixbuf,
			NULL);

	g_signal_connect(GTK_DIALOG(about_dialog), "response",
			G_CALLBACK(destroy_dialog), (gpointer) a);

	g_object_unref(pixbuf);
}

void menu_callback_quit(GSimpleAction *action, GVariant *parameter,
		gpointer data) {
	widgets *a = (widgets *) data;
	g_application_quit(G_APPLICATION(a->app));
}
