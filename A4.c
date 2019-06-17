/* Assignment 4
 * Nikolay Nikolov
 */

/* Header files */
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>

/* Constant Values */
#define MEM_SIZE 0x20000
#define LEDCTL 0xE00
#define LED_ZERO_ON  0x0E 
#define LED_ZERO_OFF 0x0F 
#define LED_ONE_ON (0x0E << 8 )
#define LED_ONE_OFF (0x0F << 8 )
#define LED_TWO_OFF (0x0F << 16)
#define LED_TWO_ON (0x0E << 16 )
#define LED_THREE_OFF (0x0F << 24 )
#define MEM "/dev/mem"

/* Global Variables */
static int dev_mem_fd;
volatile void* base_addr;
static uint32_t* led_addr;
static uint32_t* GPRC;
static uint32_t* Receive_Enable;

int main (int argc, char* argv [])
{
	int i;
	int j;
	off_t OFF_SET=0xF1200000;
	int packets;


	printf("The MEM size in hex is : 0x%x \n",MEM_SIZE);
	/* open the memory map */
	dev_mem_fd = open(MEM, O_RDWR);// File Descriptor
	printf("Fd is %d \n",dev_mem_fd);

	/* do the mmap */
	base_addr = mmap (NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
			dev_mem_fd, OFF_SET);

	printf("The base_addr is 0x%p \n",base_addr);
	/* Error Checking */
	if (base_addr == MAP_FAILED)
	{
		printf("mmap failure \n");
		exit(EXIT_FAILURE);

	}

	//printf("Base address % \n",base_addr);
	/* Create the correct led ctl address */
	led_addr = (uint32_t*) (base_addr + 0xE00);

	/* Receive Control Register */
	Receive_Enable = (uint32_t*)(base_addr + 0x00100 );

	/*Enable the receives*/
	*Receive_Enable = 0x01;

	/* Print the led_addr */
	printf("Led is 0x%x \n", *led_addr);

	/* The GPRC Register */
	GPRC = (uint32_t*)(base_addr + 0x04074);

	/* Initial Value for the LEDCTL */
	int ledctrl = *led_addr;

	/* Write to the LEDCTL registers */
	printf("Write to the LED to turn on LED0 and LED2\n");

	/* Turn the LED in a patter */
	*led_addr = LED_ZERO_ON | LED_ONE_OFF | LED_TWO_ON | LED_THREE_OFF ;

	printf("Led value is 0X%X\n", *led_addr);
	sleep(2);

	printf("Write to turn off all LED \n");
	*led_addr = LED_ZERO_OFF | LED_ONE_OFF | LED_TWO_OFF | LED_THREE_OFF ;
	sleep(2);

	printf("Led value is 0X%X\n", *led_addr);
	printf("Blink LED's \n");
	int values[4] = {0x0f0f0f0E , 0x0f0f0e0f, 0x0f0e0f0f, 0x0e0f0f0f};
	for (i = 0; i < 5 ;i++)
	{
		for (j=0;j<4 ;j++)
		{
			*led_addr = values[j];
			sleep(1);
			printf(".");
			fflush(stdout);
		}

	}
	printf("Done with LEDs!\n");
	printf("Check the packets \n");
	sleep(1);
	packets =*GPRC;
	printf("Packets received %d\n",packets);

	/* Restore the value for LED */
	*led_addr = ledctrl ;// Restore to initial value

	/* Clean up */
	munmap((void*)base_addr,MEM_SIZE);
	return 0;

}


