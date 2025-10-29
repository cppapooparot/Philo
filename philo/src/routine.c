#include "../include/philo.h"

static int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_message("has taken a fork", philo);
	if (is_dead(philo))
	{
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	pthread_mutex_lock(philo->r_fork);
	print_message("has taken a fork", philo);
	if (is_dead(philo))
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	return (1);
}

void	eat(t_philo *philo)
{
	if (!take_forks(philo))
		return ;
	print_message("is eating", philo);
	pthread_mutex_lock(&philo->meal_lock);
	philo->eating = 1;
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

int	check_meal_limit(t_philo *philo)
{
	int	should_stop;

	should_stop = 0;
	if (philo->num_times_to_eat != -1)
	{
		pthread_mutex_lock(&philo->meal_lock);
		if (philo->meals_eaten >= philo->num_times_to_eat)
			should_stop = 1;
		pthread_mutex_unlock(&philo->meal_lock);
	}
	return (should_stop);
}

void	think_routine(t_philo *philo)
{
	size_t	time_to_think;
	size_t	time_since_meal;

	if (is_dead(philo))
		return ;
	pthread_mutex_lock(&philo->meal_lock);
	time_since_meal = get_current_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	if (philo->time_to_die > time_since_meal + philo->time_to_eat)
		time_to_think = (philo->time_to_die - time_since_meal
				- philo->time_to_eat) / 2;
	else
		time_to_think = 0;
	if (time_to_think == 0)
		time_to_think = 1;
	if (time_to_think > 200)
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
		return (handle_one_philo(philo));
	if (philo->id % 2 == 1)
		think_routine(philo);
	philo_routine_cycle(philo);
	return (NULL);
}
