#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void	*roll_dice()
{
	int	value = (rand() % 6) + 1;
	int	*result = malloc(sizeof(int));
	*result = value;
	// printf("%d\n", value);
	printf("Thread result: %p\n", result);
	return (void*) result;
}

int	main()
{
	int	*res;
	srand(time(NULL));
	pthread_t	th[4];
	int	i;

	for (i = 0; i < 4; i++)
	{
		if (pthread_create(th + i, NULL, &roll_dice, NULL) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		printf("Thread %d has started\n", i);
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], (void**) &res) != 0)
		{
			return (2);
		}
		printf("Result th[i]: %d, %d\n", i, *res);
		printf("Thread %d has finished execution\n", i);
	}

	// printf("Main res: %p\n", res);
	// printf("Result: %d\n", *res);
	free(res);

	return (0);
}
