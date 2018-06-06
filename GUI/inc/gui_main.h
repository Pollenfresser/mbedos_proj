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

/**
* Defines
*/
#define DEBUG 0

/**
* Prototypes
*/

// MENU
void menu_init(gpointer data);
void menu_visible(gpointer data);
void destroy_dialog(GtkDialog *dialog, gint response_id, gpointer data);
void menu_callback_open_file(GSimpleAction *action, GVariant *parameter, gpointer data);
void menu_callback_about(GSimpleAction *action, GVariant *parameter, gpointer data);
void menu_callback_quit(GSimpleAction *action, GVariant *parameter,	gpointer data);

// OPEN FILE
void file_selection(gpointer data);


/**
* Global variables
*/

typedef struct {
	GtkWidget *menubar;
} gui_menu_s;

typedef struct {
	GtkWidget *sub_box;

} open_file_s;
// all widgets together
typedef struct {
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *main_box;
	GtkStyleProvider *css_style;
	gui_menu_s menu;
	open_file_s file;
} widgets;

#endif /* GUI_MAIN_H */
