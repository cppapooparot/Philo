#include "philo.h"

void    init_program(t_program *program, char **argv)
{
    program->dead_flag = 0;
    program->philos = malloc(sizeof(t_philo) * ft_atol(argv[1]));
    pthread_mutex_init(&program->dead_lock, NULL);
    pthread_mutex_init(&program->meal_lock, NULL);
    pthread_mutex_init(&program->write_lock, NULL);
}

void    init_forks(pthread_mutex_t *forks, int philo_num)
{
    int i;

    i = 0;
    while (i < philo_num)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
}

void    init_philos(t_program *program, pthread_mutex_t *forks, char **argv)
{
    int i;

    i = 0;
    while (i < ft_atol(argv[1]))
    {
        program->philos[i].id = i + 1;
        program->philos[i].eating = 0;
        program->philos[i].meals_eaten = 0;
        program->philos[i].last_meal = get_current_time();
        program->philos[i].time_to_die = ft_atol(argv[2]);
        program->philos[i].time_to_eat = ft_atol(argv[3]);
        program->philos[i].time_to_sleep = ft_atol(argv[4]);
        program->philos[i].start_time = get_current_time();
        program->philos[i].num_of_philos = ft_atol(argv[1]);
        program->philos[i].num_times_to_eat = -1;
        if (argv[5])
            program->philos[i].num_times_to_eat = ft_atol(argv[5]);
        program->philos[i].dead = &program->dead_flag;
        program->philos[i].l_fork = &forks[i];
        program->philos[i].r_fork = &forks[(i + 1) % ft_atol(argv[1])];
        program->philos[i].write_lock = &program->write_lock;
        program->philos[i].dead_lock = &program->dead_lock;
        program->philos[i].meal_lock = &program->meal_lock;
        i++;
    }
}