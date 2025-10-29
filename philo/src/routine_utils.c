/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsahakya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:45:53 by nsahakya          #+#    #+#             */
/*   Updated: 2025/10/29 17:45:54 by nsahakya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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

void	*handle_one_philo(t_philo *philo)
{
	print_message("has taken a fork", philo);
	ft_usleep(philo->time_to_die);
	return (NULL);
}

void	philo_routine_cycle(t_philo *philo)
{
	while (!is_dead(philo))
	{
		eat(philo);
		if (is_dead(philo) || check_meal_limit(philo))
			break ;
		print_message("is sleeping", philo);
		ft_usleep(philo->time_to_sleep);
		if (is_dead(philo) || check_meal_limit(philo))
			break ;
		think_routine(philo);
	}
}
