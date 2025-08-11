#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* The threads shared the same variable x. Therefore the value of x will be the same on both cases. My doubt is about that this happens due to the fact that x is shared as a global variable. I don't know, outside the other functions and this contribute to this result.  */

int	x = 2;

void    *routine()
{
	x += 5;
	sleep(2);
	printf("Value of x: %d\n", x);
}

void	*routine1()
{
	// x += 5;
	sleep(2);
	printf("Value of x: %d\n", x);
}

int	main()
{
	pthread_t	t1;
	pthread_t	t2;

	if (pthread_create(&t1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&t2, NULL, &routine1, NULL))
		return (2);
	if (pthread_join(t1, NULL))
		return (3);
	if (pthread_join(t2, NULL))
		return (4);

	return (0);
}
