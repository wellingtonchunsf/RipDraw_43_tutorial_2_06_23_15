/* dump_buffer.c Version 2.0T 11/1/14 Ripdraw Tutorial
 *
 * dump_buffer dumps a buffer to the console
 *
 */

void dump_buffer(char *buffer, int buffer_size)
{
#include <stdio.h>
	int c =0;;
	for (c=0;c<buffer_size;c++)
	{
     		printf("%.2X ", (int)buffer[c]);

     		// put an extra space between every 4 bytes
     		if (c % 4 == 3)
     		{
         		printf(" ");
     		}

     		// Display 16 bytes per line
     		if (c % 16 == 15)
     		{
			printf("\n");
         		//printf("Index %d  %x \n",c+1,c+1);
     		}
	}
	// Add an extra line feed for good measure
	//printf("\n");
}
