/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Headerfile of the Graphical User Interface
 *
 * Status: 
 */
#ifndef GUI_MAIN_H
#define GUI_MAIN_H

/**
* Included Files
*/

// GUI
#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/**
* Defines
*/
#define DEBUG 1

/**
* Prototypes
*/

// MAIN
void destroy_dialog(GtkDialog *dialog, gint response_id, gpointer data);
void main_gui_menu (gpointer data);
void main_open_file (gpointer data);
void main_drawing_area (gpointer data);
void main_uart_config (gpointer data);
void main_plotter_communication (gpointer data);


// MENU
void menu_init(gpointer data);
void menu_callback_open_file(GSimpleAction *action, GVariant *parameter, gpointer data);
void menu_callback_uart_config(GSimpleAction *action, GVariant *parameter, gpointer data);
void menu_callback_about(GSimpleAction *action, GVariant *parameter, gpointer data);
void menu_callback_quit(GSimpleAction *action, GVariant *parameter,	gpointer data);

// OPEN FILE
void file_selection(gpointer data);
void file_open(gpointer data);
void file_read(gpointer data);

// AREA DRAW
void draw_area_init(gpointer data);
void draw_image (gpointer data);

// UART
void uart_dialog (gpointer data);

/**
* Global variables
*/



typedef struct {
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *dialog;
} uart_s;

typedef struct {
	GtkWidget *area;
	gint xsize;
	gint ysize;
	cairo_t* cr;
} gui_drawing_s;

typedef struct {
	GtkWidget *menubar;
} gui_menu_s;

typedef struct {
	GtkWidget *sub_box;
	gchar *filename;
} open_file_s;
// all widgets together
typedef struct {
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *main_box;
	GtkStyleProvider *css_style;
	gui_menu_s menu;
	open_file_s file;
	gui_drawing_s draw;
	uart_s uart;
} widgets;

#endif /* GUI_MAIN_H */
