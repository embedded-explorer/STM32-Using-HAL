#include <stdio.h>

//Debug Exception and Monitor Control Register base address
#define DEMCR        			*((volatile uint32_t*) 0xE000EDFCU )

/* ITM register addresses */
#define ITM_STIMULUS_PORT0   	*((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN          	*((volatile uint32_t*) 0xE0000E00 )

void ITM_SendChar(char ch);

int main()
{
	printf("Application is running\n");

	return 0;
}

// Overwrite write function to send data to ITM
int _write(int file, char *ptr, int len)
{
	int i;
	for (i=0; i<len; i++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}

void ITM_SendChar(char ch)
{
	//Enable TRCENA
	DEMCR |= ( 1 << 24);

	//enable stimulus port 0
	ITM_TRACE_EN |= ( 1 << 0);

	// read FIFO status in bit [0]:
	while(!(ITM_STIMULUS_PORT0 & 1));

	//Write to ITM stimulus port0
	ITM_STIMULUS_PORT0 = ch;
}
