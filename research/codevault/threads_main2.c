#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int	mails = 0;

void	*routine()
{
	/* This amount creates a race condition between the two threads. The OS can pause one or another between the calcs and fuck with the results */
	for (int i = 0; i < 1000000; i++)
	{
		mails++;
		// read mails
		// increment
		// write mails
	}
}

int	main()
{
	pthread_t	p1;
	pthread_t	p2;

	if (pthread_create(&p1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&p2, NULL, &routine, NULL))
		return (2);
	if (pthread_join(p1, NULL))
		return (3);
	if (pthread_join(p2, NULL))
		return (4);

	printf("Number of mails: %d\n", mails);
	return (0);
}

