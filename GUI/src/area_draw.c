/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 06.06.2018
 *
 * File description: Drawing Area
 *
 * Status: beginning
 *
 * librsvg - is a library to render SVG files using cairo.
 * https://developer.gnome.org/rsvg/2.42/rsvg-RsvgHandle.html#RsvgHandle
 * https://developer.gnome.org/rsvg/2.42/rsvg-Using-RSVG-with-cairo.html
 *
 * The preferred way to render an already-loaded RsvgHandle is to use rsvg_handle_render_cairo()
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

static void event_draw (GtkWidget *widget, GdkEvent *event, gpointer data);
static gboolean event_config (GtkWidget *widget, GdkEventConfigure *event, gpointer data);

void draw_area_init(gpointer data) {
#if DEBUG
	g_print("Draw Area Init, ");
#endif
	widgets *a = (widgets *) data;
	a->draw.area = gtk_drawing_area_new();
	gtk_widget_set_size_request (a->draw.area, 100, 400);
	gtk_box_pack_start (GTK_BOX(a->main_box), a->draw.area, FALSE, FALSE, 0);
	// DRAW EVENT
	g_signal_connect (a->draw.area, "draw", G_CALLBACK (event_draw), (gpointer) a);
	// CONFIGURE EVENT
	g_signal_connect (a->draw.area, "configure_event", G_CALLBACK (event_config), (gpointer) a);
}

static void event_draw (GtkWidget *widget, GdkEvent *event, gpointer data){

	widgets *a = (widgets *) data;

	// obtain the size of the drawing area
	a->draw.xsize = gtk_widget_get_allocated_width (a->draw.area);
	a->draw.ysize = gtk_widget_get_allocated_height (a->draw.area);

	cairo_set_line_width (a->draw.cr, 0.005);
	// draw a white filled rectangle
	cairo_set_source_rgb (a->draw.cr, 1.0, 1.0, 1.0);
	cairo_rectangle (a->draw.cr, 0, 0, a->draw.xsize, a->draw.ysize);
	cairo_fill (a->draw.cr);
	// set up a transform so that (0,0) to (1,1) maps to (0, 0) to (width, height)
	cairo_translate (a->draw.cr, 0, 0);
	cairo_scale (a->draw.cr, a->draw.xsize, a->draw.ysize);
	// invoke a drawing function depending on the value of the state variable
	draw_image ((gpointer)a);

}

static gboolean event_config (GtkWidget *widget, GdkEventConfigure *event, gpointer data){

	widgets *a = (widgets *) data;

	GtkAllocation allocation;
	static cairo_surface_t *surface = NULL;

	gtk_widget_get_allocation (widget, &allocation);
	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
						     CAIRO_CONTENT_COLOR,
						     allocation.width,
						     allocation.height);
	a->draw.cr = cairo_create (surface);
	cairo_destroy (a->draw.cr);
	return TRUE;
}


void draw_image (gpointer data){

	widgets *a = (widgets *) data;
	cairo_t* cr = a->draw.cr;

	// draw face - blue circle (R G B)
	cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
	cairo_arc (cr, 0.5, 0.5, 0.45, 0, 2 * 3.1);
	cairo_stroke (cr);

}

