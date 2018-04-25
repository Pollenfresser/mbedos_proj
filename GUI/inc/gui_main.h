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


/**
* Global variables
*/

// all widgets together
typedef struct {
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *main_box;
	GtkStyleProvider *css_style;

} widgets;

#endif /* GUI_MAIN_H */
