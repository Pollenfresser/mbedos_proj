done by: Lucas L , Chrisy C
done: + ~ -


# Pre-Work, GUI
* Pick a file format -
* install a suitable software -
* organize some example files (with just 2 lines) -
* simple file parser; extracts the relevant information like X/Y coordinates; drawn or only moved -
* command-line program outputs information on STDOUT -
* Test your application -
* structure code for flexibility -


# PC GUI application

* pick one of the menu demos and adjsut the menus to your needs -
* file open functionality -
* drawing area: connect a "draw" and a "configure_event" signal (see lines 179 and 182 in cd_app.c -
* drawing area: copy cd_configure_event() and cd_draw_callback() callbacks -
* drawing area: replace the switch-case statement in the latter callback function -
* add cairo_move_to(), cairo_line_to() and cairo_stroke() functions for every line that shall be drawn -
* rectangles: cairo_rectangle() followed by cairo_stroke() - 
* configure dialog (obtain settings for serial interface, if using it) -
* Using the USB interface with libUSB this step can be omitted - you may either hardcode the USB settings or provide a simple dialog therefore -
* add the code for libUSB or serial interface. The latter should already be in-place from task #1 or task #3 -




# XMC RTOS application
* implementing joystick -
* transfer bare-bone project template -

