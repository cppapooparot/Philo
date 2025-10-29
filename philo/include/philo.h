/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsahakya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:46:44 by nsahakya          #+#    #+#             */
/*   Updated: 2025/10/29 17:46:46 by nsahakya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_program	t_program;

typedef struct s_philo
{
	pthread_t				thread;
	int						id;
	int						eating;
	int						meals_eaten;
	size_t					last_meal;
	size_t					time_to_die;
	size_t					time_to_eat;
	size_t					time_to_sleep;
	size_t					start_time;
	int						num_of_philos;
	int						num_times_to_eat;
	int						*dead;
	pthread_mutex_t			*r_fork;
	pthread_mutex_t			*l_fork;
	pthread_mutex_t			*write_lock;
	pthread_mutex_t			*dead_lock;
	pthread_mutex_t			meal_lock;
}							t_philo;

typedef struct s_program
{
	int						dead_flag;
	size_t					start_time;
	pthread_mutex_t			dead_lock;
	pthread_mutex_t			meal_lock;
	pthread_mutex_t			write_lock;
	t_philo					*philos;
}							t_program;

int							check_args(int argc, char **argv);
int							ft_usleep(size_t milliseconds);
size_t						get_current_time(void);
long						ft_atol(const char *str);

void						init_program(t_program *program, char **argv);
void						init_forks(pthread_mutex_t *forks, int philo_num);
void						init_philos(t_program *program,
								pthread_mutex_t *forks, char **argv);

void						*philo_routine(void *arg);
void						print_message(char *str, t_philo *philo);
int							is_dead(t_philo *philo);

int							start_simulation(t_program *program);
void						cleanup(t_program *program, pthread_mutex_t *forks);

void						*handle_one_philo(t_philo *philo);
void						philo_routine_cycle(t_philo *philo);
int							check_meal_limit(t_philo *philo);
void						eat(t_philo *philo);
void						think_routine(t_philo *philo);
#endif
