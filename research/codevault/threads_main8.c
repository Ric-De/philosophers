#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t	mutex;

void	*routine(void *arg)
{
	// Approach using trylock -> It's an imediate verification if it's lock / unlock
	
/*	if (pthread_mutex_trylock(&mutex) == 0)
	{
		printf("Got lock\n");
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		printf("Didn't get lock\n");
	}
*/
	//Approach using lock -> always wait lock to unlock to go to next thread

	pthread_mutex_lock(&mutex);
	printf("Got lock\n");
	sleep(1);
	pthread_mutex_unlock(&mutex);
}

int	main()
{
	pthread_t	th[4];
	pthread_mutex_init(&mutex, NULL);

	for (int i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread");
		}
	}
	for (int  i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("Failed to join thread");
		}
	}
	return (0);
}
