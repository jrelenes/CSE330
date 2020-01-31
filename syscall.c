#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
	printf("Say hello");

	long int ret_status = syscall(398); // the number of the syscall in the system-call table

	if(ret_status == 0)
	{
		printf("System call executed correctly. Use dmesg to check the syscall output.\n");
	}	
	else
	{
		printf("System call did not execute as expected\n");
	}
	
	return 0;
}
