#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int	mails = 0;
// int	lock = 0;	// We can reproduce the mutex with a variable but use the mutex
pthread_mutex_t	mutex;

void	*routine()
{
	/* This amount creates a race condition between the two threads. The OS can pause one or another between the calcs and fuck with the results */
	for (int i = 0; i < 1000000; i++)
	{
		/* if (lock == 1)
		{
			// wait until the lock is 0
		}
		lock = 1; */
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);		
		/* lock = 0; */
		// read mails
		// increment
		// write mails
	}
}

int	main()
{
	pthread_t	p1;
	pthread_t	p2;
	pthread_mutex_init(&mutex, NULL);

	if (pthread_create(&p1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&p2, NULL, &routine, NULL))
		return (2);
	if (pthread_join(p1, NULL))
		return (3);
	if (pthread_join(p2, NULL))
		return (4);
	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %d\n", mails);
	return (0);
}

