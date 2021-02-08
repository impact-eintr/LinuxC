
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
 
int main()
{
	printf("Valid priority range for SCHED_OTHER: %d - %d\n",
		sched_get_priority_min(SCHED_OTHER),
		sched_get_priority_max(SCHED_OTHER));
	printf("Valid priority range for SCHED_FIFO: %d - %d\n",
		sched_get_priority_min(SCHED_FIFO),
		sched_get_priority_max(SCHED_FIFO));
	printf("Valid priority range for SCHED_RR: %d - %d\n",
		sched_get_priority_min(SCHED_RR),
		sched_get_priority_max(SCHED_RR));
	
	return 0;		
}



