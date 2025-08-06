#include "philo.h"

void    print_message(char *str, t_philo *philo)
{
    size_t  time;

    pthread_mutex_lock(philo->write_lock);
    time = get_current_time() - philo->start_time;
    if (!is_dead(philo))
        printf("%zu %d %s\n", time, philo->id, str);
    pthread_mutex_unlock(philo->write_lock);
}

int is_dead(t_philo *philo)
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

static void eat(t_philo *philo)
{
    if (philo->num_of_philos == 1)
    {
        pthread_mutex_lock(philo->l_fork);
        print_message("has taken a fork", philo);
        ft_usleep(philo->time_to_die + 10); // Wait a bit longer than time_to_die
        pthread_mutex_unlock(philo->l_fork);
        return;
    }

    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->r_fork);
        print_message("has taken a fork", philo);
        pthread_mutex_lock(philo->l_fork);
        print_message("has taken a fork", philo);
    }
    else
    {
        pthread_mutex_lock(philo->l_fork);
        print_message("has taken a fork", philo);
        pthread_mutex_lock(philo->r_fork);
        print_message("has taken a fork", philo);
    }

    philo->eating = 1;
    print_message("is eating", philo);

    pthread_mutex_lock(philo->meal_lock);
    philo->last_meal = get_current_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->meal_lock);

    ft_usleep(philo->time_to_eat);
    philo->eating = 0;

    pthread_mutex_unlock(philo->r_fork);
    pthread_mutex_unlock(philo->l_fork);
}

void    *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 == 0)
        ft_usleep(1);
    
    // Special case for single philosopher
    if (philo->num_of_philos == 1)
    {
        pthread_mutex_lock(philo->l_fork);
        print_message("has taken a fork", philo);
        pthread_mutex_unlock(philo->l_fork);
        // Just return and let monitor handle death detection
        return (NULL);
    }
    
    while (!is_dead(philo))
    {
        eat(philo);
        if (is_dead(philo))
            break;
        
        print_message("is sleeping", philo);
        ft_usleep(philo->time_to_sleep);

        print_message("is thinking", philo);
    }
    return (NULL);
}

