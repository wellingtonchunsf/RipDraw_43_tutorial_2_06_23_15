 /* 43_touch.c  Version 3.0T 6/23/15 RipDraw Tutorial Application that dumps touch screen data to system console
 * 
 * makefile modified for current RipDraw C directory structure
 * removed RdInitialize() for RipdrawLinuxSerialInit()  
 * change #include ripdraw.h to #include Ripdraw.h  for case sensitivity for linux file system/tools
 *
 * Tested with RipDraw C Library 
 * Version:  3.00 Date of Release May 01, 2015
 * makefile modified for current RipDraw C directory structure
 * removed RdInitialize() for RipdrawLinuxSerialInit()  
 *
 *
 * Application  
 * 
 * The event message packet is displayed on the console screen
 * The purpose of this application is to show how to read, decipher and test the touch screen event pakage
 * This application is also useful for the user to correlate physical actions on the touch screen to event messages
 *
 * No images or fonts are used in this tutorial.  The display will be just display the background color.
 *
 *
 */
#include <stdio.h>
#include "Ripdraw.h"

#define RD_TRUE 1
#define RD_FALSE 0
#define STATUS_OK 0 

#define SLEEPTIME 1	/* amount of time to sleep before polling again */ 

int main(int argc, char **argv)
{

	int ret;

	RdInterface* rd_interface;
	RdId layer;

	/*
	 * Initialize the Ripdraw library
	 * Connect the system port to a Ripdraw display interface handle
	 *
	 * The Ripdraw display interface handle is used direct all Ripdraw commands to the specific Ripdraw display
	 *
	 * For the Beagle board, the com port shows up as "/dev/ttyACMO"
	 * For windows platforms it may be COM10
	 * ("\\\\.\\COM10");
	 */

	/* malloc space for rd_interface object */
	rd_interface = malloc(sizeof(RdInterface));
	if (rd_interface == NULL) return; /* if handle is NULL, the malloc failed */

	memset(rd_interface,0,sizeof(RdInterface)); 

    	ret = RipdrawLinuxSerialInit(rd_interface, "/dev/ttyACM0");
    	if (ret != RdErrorSuccess)
    	{
        	printf("Failed with error code : %x", ret);
        	return ret;
    	}

	/* Issue reset to Ripdraw display
	* if you are single stepping, the screen will go blank
	*/
	ret = RdReset(rd_interface);
	if (ret !=  STATUS_OK ) return ret;

	/*
	 * Set background color for this layer
	 *
	 * Rd_Color() is short function that packs the red, green, blue and alpha values into a structure
	 * color values  RGB= 0xFF is white,  RGB = 0x00 is black 
	 *
	 */

	#define	RED_BACK	0xff
	#define	GREEN_BACK	0x00
	#define	BLUE_BACK	0x00	
	#define ALPHA		0xff	


	/* set background color and enable layer 1 */
	layer = 1;
	ret = RdSetLayerEnable(rd_interface,layer, RD_TRUE);
	if (ret != STATUS_OK) return ret;

	ret =RdSetLayerBackColor(rd_interface, layer, RdCreateColor(RED_BACK, GREEN_BACK, BLUE_BACK,ALPHA));
	if (ret != STATUS_OK) return ret;

	printf("\nRipdraw Tutorial 2 Touch Screen Dump");
	printf("\nRipdraw Screen will remain blank");
	printf("\nPutDown X,Y swap bug correct by software");
	fflush(stdout);
	/* get touch and dump forever */
	while(1) 
	{
		ret = gettouch(rd_interface);
		if (ret != STATUS_OK) return ret;
		sleep(SLEEPTIME);  /* sleep before repolling */

	}

	/* close off the interface */
	ret = RdInterfaceClose(rd_interface);
	return 0;
}
