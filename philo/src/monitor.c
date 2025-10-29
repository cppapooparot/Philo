#include "../include/philo.h"

static int	check_death(t_philo *philos, int num_philos)
{
	int		i;
	size_t	time;

	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		if (get_current_time() - philos[i].last_meal >= philos[i].time_to_die
			&& philos[i].eating == 0)
		{
			pthread_mutex_lock(philos[i].dead_lock);
			*philos[i].dead = 1;
			pthread_mutex_lock(philos[i].write_lock);
			time = get_current_time() - philos[i].start_time;
			printf("%zu %d died\n", time, philos[i].id);
			pthread_mutex_unlock(philos[i].write_lock);
			pthread_mutex_unlock(philos[i].dead_lock);
			pthread_mutex_unlock(&philos[i].meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	return (0);
}

static int	check_meals_eaten(t_philo *philos, int num_philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (-1);
	while (i < num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	if (finished_eating == num_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos[0].dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

static int	create_threads(t_program *program)
{
	int	i;
	int	num_philos;

	i = 0;
	num_philos = program->philos[0].num_of_philos;
	while (i < num_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, philo_routine,
				&program->philos[i]) != 0)
			return (0);
		i++;
	}
	while (get_current_time() < program->start_time)
		continue ;
	return (1);
}

static void	monitor_and_join(t_program *program, int num_philos)
{
	int	i;
	int	should_stop;

	should_stop = 0;
	while (!should_stop)
	{
		if (check_death(program->philos, num_philos))
			break ;
		if (check_meals_eaten(program->philos, num_philos) > 0)
			break ;
		pthread_mutex_lock(&program->dead_lock);
		should_stop = program->dead_flag;
		pthread_mutex_unlock(&program->dead_lock);
		usleep(1000);
	}
	i = 0;
	while (i < num_philos)
	{
		pthread_join(program->philos[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_program *program)
{
	int	num_philos;

	if (!create_threads(program))
		return (0);
	num_philos = program->philos[0].num_of_philos;
	monitor_and_join(program, num_philos);
	return (1);
}
