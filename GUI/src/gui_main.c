/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Main - routine
 *
 * Status:
 *
 */

#include <gui_main.h>

/**
 * Global variables
 */

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

/*
 * The girl loves media technology
 */
void apply_css(GtkWidget *widget, GtkStyleProvider *css_s){
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), css_s, G_MAXUINT);
	if(GTK_IS_CONTAINER (widget)){
		gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, css_s);
	}
}

void open_file_c (gpointer data){

}

void drawing_area_c (gpointer data){

}

void configure_dialog_c (gpointer data){

}

void plotter_communication_c (gpointer data){

}

void activate (GtkApplication *app, gpointer data)
{
	widgets *a = (widgets *) data;

	a->css_style = GTK_STYLE_PROVIDER (gtk_css_provider_new ());
	gtk_css_provider_load_from_resource (GTK_CSS_PROVIDER(a->css_style), "/gui_res/res/style.css");


	// create window and set title *****
	a->window = gtk_application_window_new (a->app);
	gtk_window_set_application (GTK_WINDOW (a->window), GTK_APPLICATION (a->app));
	gtk_window_set_position (GTK_WINDOW (a->window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (a->window), "Lucas & Chrisy Plotter");
	gtk_window_set_default_size (GTK_WINDOW (a->window), 800, 500); // width, height


	// layout containers *****
	a->main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add( GTK_CONTAINER (a->window), a->main_box);

	menu_init ((gpointer) a);
	menu_visible ((gpointer) a);


	// css
	apply_css (a->window, a->css_style);

	// show widgets: window, box-layout
	gtk_widget_show (a->window);
	gtk_widget_show (a->main_box);

}

int main (int argc, char **argv)
{
	int status;

	widgets *a = g_malloc(sizeof(widgets));

	a->app = gtk_application_new ("org.gtk.game", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (a->app, "activate", G_CALLBACK (activate), (gpointer) a);
	status = g_application_run (G_APPLICATION (a->app), argc, argv);

	
	// clean up

	g_object_unref (a->app);

	g_free (a);

	return status;
}
