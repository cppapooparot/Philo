#include "philo.h"

void	print_message(char *str, t_philo *philo)
{
	size_t	time;

	pthread_mutex_lock(philo->write_lock);
	time = get_current_time() - philo->start_time;
	if (!is_dead(philo))
		printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(philo->write_lock);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
	{
		pthread_mutex_unlock(philo->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_message("has taken a fork", philo);
	pthread_mutex_lock(philo->r_fork);
	print_message("has taken a fork", philo);
	print_message("is eating", philo);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	if (!is_dead(philo))
	{
		pthread_mutex_lock(&philo->meal_lock);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->meal_lock);
	}
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

static void	think_routine(t_philo *philo)
{
	size_t	time_to_think;
	size_t	time_since_meal;

	pthread_mutex_lock(&philo->meal_lock);
	time_since_meal = get_current_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	if (philo->time_to_die > time_since_meal + philo->time_to_eat)
		time_to_think = (philo->time_to_die
				- time_since_meal - philo->time_to_eat) / 2;
	else
		time_to_think = 0;
	if (time_to_think == 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	print_message("is thinking", philo);
	ft_usleep(time_to_think);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = philo->start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	while (get_current_time() < philo->start_time)
		continue ;
	if (philo->num_of_philos == 1)
	{
		print_message("has taken a fork", philo);
		ft_usleep(philo->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 1)
		think_routine(philo);
	while (!is_dead(philo))
	{
		eat(philo);
		if (is_dead(philo))
			break ;
		print_message("is sleeping", philo);
		ft_usleep(philo->time_to_sleep);
		think_routine(philo);
	}
	return (NULL);
}
