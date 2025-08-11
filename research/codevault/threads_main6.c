/* 	The Problem:
 * 	Create 10 threads, each taking a unique prime from the primes array and print it on the screen.
 * */

/* Proposal at the end of the video was to do the same in a simple way. Tip: the approach is related to the fact that we are passing addresses.  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int	prime_array[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void	*routine(void *arg)
{
	// sleep(1);
	int	index = *(int*)arg;
	printf("%d ", prime_array[index]);
	free(arg);
}

int	main()
{
	pthread_t	th[10];
	int	i;

	for (i = 0; i < 10; i++)
	{
		int	*a = malloc(sizeof(int));
		*a = i;

		if (pthread_create(&th[i], NULL, &routine, a) != 0)
		{
			perror("Failed to create thread");
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("Failed to join thread");
		}
	}

	return (0);
}
