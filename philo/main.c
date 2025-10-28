#include "philo.h"

int	main(int argc, char **argv)
{
	t_program		program;
	pthread_mutex_t	*forks;

	if (!check_args(argc, argv))
		return (1);
	printf("Arguments OK ✅\n");
	init_program(&program, argv);
	forks = malloc(sizeof(pthread_mutex_t) * ft_atol(argv[1]));
	init_forks(forks, ft_atol(argv[1]));
	init_philos(&program, forks, argv);
	if (!start_simulation(&program))
	{
		printf("Error: Failed to start simulation\n");
		cleanup(&program, forks);
		return (1);
	}
	cleanup(&program, forks);
	return (0);
}
