/* gettouch.c  Version 2.2T 3/14/15 Ripdraw Tutorial application used to dump the event message from 1RU GPU 
 * 
 * gettouch() deciphers the raw event data from a touch and dumps it on the console screen 
 *
 *
 */

#include "Ripdraw.h"

#define MID_TOP 100
#define MID_BOTTOM 150


RdByte new_red;
RdByte new_green;
RdByte new_blue;

int gettouch(RdInterface* rd_interface) {
	RdEvent* events;
	RdUWord count;
	RdByte* data;

	RdByte* dumpdata;
	RdByte* pointsData;
	RdByte* free_pointsData;
	RdUWord pointsDataLen;

	int touchPoints;
	int GestureId;
	int xPos, yPos, swapPos;
	int eventflag;
	int i, j;
	int ret;

	
	/* 
	 * Poll for an event, return if none 
	 * 
	 * EventMessage returns a pointer to arrary of "events" messages
	 * The lenght of this array is equal to "count 
	 *
	 */
	count = 0;
	ret = RdGetEventData(rd_interface, &events, &count);
	if (ret != 0) return ret;

	/* process all events */
	if (count > 0) 
	{
		printf("\n\n**** EventMessage with %d Event.data packets ****", count);
		// process all events
		for (i = 0; i < count; i++) 
		{
			//process only touch event 
			if (events[i].eventType == 1) 
			{
				//load data with pointer to Touch EventMessage Data Frame
				data = events[i].data;

				// Read first 2 bytes of Event.data which is the Packetsize of PacketData, not including Label size and Label string
				pointsDataLen = *((RdUWord*) data);

				// print the sequence of the event data and the total number of event
				printf("\n%d/%d",i+1,count);
				
				// bump pointer to point PacketData
				data += 2;
				
				// load dumpdata pointer for printing data
				dumpdata = data;

				// dump the PackData which has length of pointsDataLen (Packetsize)
				printf("\tPkData ");
				dump_buffer(dumpdata,pointsDataLen);

				/*
				 *  allocate enough memory to hold only the PacketData and copy over
				 *  we do this incase you need to transfer contents of the PacketData somewhere
				 *  the event.dat will eventually be release back into memory by the C library
				 *
				 */
				pointsData = (RdByte*) malloc(pointsDataLen);
				free_pointsData = pointsData; /* save pointer for freeing memory */

				memcpy(pointsData, data, pointsDataLen);

				/*
				 *  get Gesture ID from the PacketData and dump to console 
				 *  First byte of the PacketData, mask
				 */
				GestureId= pointsData[0]; 
				printf("\tGesture %d", GestureId);
				
				// get total number of points from the Packet Data
				touchPoints = pointsData[1]; 
				printf("\tPoints %d ", touchPoints);

				// bump pointer of Packet Data to first Touch Point data
				pointsData += 2;

				//read all points one by one
				for (j = 0; j < touchPoints; j++) 
				{
					eventflag = ((pointsData[0] & 0xC0) >> 6);
					xPos = ((pointsData[0] & 0x0F) << 8) | (pointsData[1]);
					yPos = ((pointsData[2] & 0x0F) << 8) | (pointsData[3]);


					/* current firmware bug in controller reverses x,y postion in Putdown event, fix in software and label "Putup*" to show patch) */
					if (eventflag == 01) 
					{
						swapPos = xPos;
						xPos = yPos;
						yPos = swapPos;

						/* set layer 1 background for layer 1 based on new_red, new_green, and new blue, leave alpha fixed at 0xff */
						ret =RdSetLayerBackColor(rd_interface, 1, RdCreateColor(new_red, new_green, new_blue,0xff));
						if (ret != 0) return ret;
					}

					printf(" %d -> X=%d Y=%d Evt=%d ", j+1, xPos, yPos,eventflag);

					if (eventflag == 00) printf (" Putdown");
					if (eventflag == 02) printf (" Contact");
					if (eventflag == 01) printf (" Putup* ");
					if (eventflag == 03) printf (" UNKNOWN");

					/* check where finger is horizontally */
					if (yPos < MID_TOP)
					{
						/* normalize x position and use as red background value */
						new_red = xPos/2;
						printf (" New Red %d",new_red);
					}


					if ((yPos >= MID_TOP) && (yPos <= MID_BOTTOM))
					{
						/* normalize x position and use as green background value */
						new_green = xPos/2;
						printf (" New green %d",new_green);
					}

					if (yPos > MID_BOTTOM )
					{
						/* normalize x position and use as blue background value */
						new_blue = xPos/2;
						printf (" New blue %d",new_blue);
					}
					
					// index over to next TouchPoint data set
					pointsData += 4;
				}
			}
			//free that event.data.
			RdFreeData(events[i].data);
			// free buffer that we used
			free(free_pointsData);
			fflush(stdout);		
		}
		//free all events structures.
		if (events != NULL) 
		{
			RdFreeData(events);
		}
	}
	return ret;
}
