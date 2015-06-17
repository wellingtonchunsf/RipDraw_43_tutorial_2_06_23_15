43_touch.c  Version 3.0T 6/23/15 RipDraw Tutorial Application that dumps touch screen data to system console

Run on Beagle Bone Black Host computer using USB port to RipDraw

Tested with Ripdraw C Library 
Version:  3.00 Date of Release May 01, 2015

 makefile modified for current RipDraw C directory structure
 removed RdInitialize() for RipdrawLinuxSerialInit() 

The code was generated using the gcc version 4.8.1, Yagarto tool chain and Eclipses IDE.

The demo program is distributed in the following directory structure

application directory
   app				source files
   clib				RipDraw C Libraries
   fonts			font files
   imagelists			imagelists files
   images			image files
   object 			object file directory

A makefile is included in the distribution.
An executable is included, but his hardwired to /dev/tty/ACM0 and will work without changes if your USB is the same.

