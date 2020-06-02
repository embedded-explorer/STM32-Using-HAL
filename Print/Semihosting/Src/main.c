#include <stdio.h>


extern void initialise_monitor_handles(void);

int main()
{
	initialise_monitor_handles();
	printf("Application is running\n");

	return 0;
}
