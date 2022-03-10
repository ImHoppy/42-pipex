#include "pipex_bonus.h"

void child(int *i)
{
	printf("in child function before fork with %d %d\n", getpid(), *i);
	*i = fork();
	if (*i == 0)
	{
		printf("in child function with %d %d\n", getpid(), *i);
		exit(1);
	}
}

int main()
{
	int i = -1;
	printf("in main function with %d %d\n", getpid(), i);
	child(&i);
	printf("end main function with %d %d\n", getpid(), i);
}