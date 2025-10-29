#include "../include/philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

void	cleanup(t_program *program, pthread_mutex_t *forks)
{
	int	i;
	int	num_philos;

	i = 0;
	num_philos = program->philos[0].num_of_philos;
	while (i < num_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&program->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&program->dead_lock);
	pthread_mutex_destroy(&program->meal_lock);
	pthread_mutex_destroy(&program->write_lock);
	free(program->philos);
	free(forks);
}

void	print_message(char *str, t_philo *philo)
{
	size_t	time;

	pthread_mutex_lock(philo->dead_lock);
	pthread_mutex_lock(philo->write_lock);
	time = get_current_time() - philo->start_time;
	if (*philo->dead == 0)
		printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(philo->write_lock);
	pthread_mutex_unlock(philo->dead_lock);
}
