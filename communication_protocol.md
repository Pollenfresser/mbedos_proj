# Communication Protocol
## General rules
* XMC inidicates as soon as the initialization is ready by sending "$ready#"
* Data may only be sent after the XMC is ready
* The GUI reads SVG files, accepted elements are
  * Lines
  * Rectangles
  * Cricles

## GUI
Transmission may start after receiving the "$ready#" message.
For each possible elemnet the element name shall be transmitted first, followed
by either 3 or 4 paramteres indicating the size of the object, each seperated by a ":".

The communication shall start with a "$" and end with a "#". Each consecutive
element shall only be transmitted if the XMC has finished the previous one, indicated by another "$ready#" message.
* Example
  * SVG file:  rect x="50" y="200" width="1000" height="500"
  * output: $rect:50:200:1000:500#

## XMC
At startup the XMC evaluates the size of the drawing area, after finishing this
step a "$ready#" message is sent to the pc. With receiving the element it is
broken into the single elements and checked for its completeness and the size of
the elements. If the size of an element exceeds the drawing are a "$err:size#"
message is sent to the pc.

If a message is valid the XMC draws the object, after completion another "$ready#"
message is sent to the pc, indicating that the XMC is able to draw the next element.

## Manual Mode
The XMC shall be controllable via the GUI interface. The following commands are used:

* Pen
  * Up: $pen:up#
  * Down: $pen:down#
* Movement, always in strait lines
  * $move:x-target:y-target# (Example: $move:50:350#)
