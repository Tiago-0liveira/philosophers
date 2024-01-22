/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/22 21:24:31 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_philo *philo, size_t id, t_table *table,
		t_fork *right_fork)
{
	philo->philo_id = id;
	philo->last_eat_time = 0;
	philo->n_eat = 0;
	philo->state = THINKING;
	pthread_mutex_init(&philo->philo_mutex, NULL);
	philo->left_fork = NULL;
	philo->right_fork = right_fork;
	philo->table = table;
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	monitor_thread;

	philo = (t_philo *)arg;
	if ((philo->philo_id + 1) % 2 == 0)
		mysleep(philo->table->time_to_eat / 10);
	while (!is_dead(philo, false))
	{
		pthread_create(&monitor_thread, NULL, check_philo_death, philo);
		take_forks(philo);
		philo_eat(philo);
		pthread_detach(monitor_thread);
		if (philo->table->n_eat != (size_t) - 1)
		{
			pthread_mutex_lock(&philo->table->dead_mutex);
			if (philo->table->n_eat == philo->n_eat)
			{
				pthread_mutex_unlock(&philo->table->dead_mutex);
				philo->state = FINNISHED;
				return (NULL);
			}
			pthread_mutex_unlock(&philo->table->dead_mutex);
		}
	}
	return (NULL);
}

bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_philo_state(TOOK_FORK, philo);
	if (philo->table->n_philo == 1)
	{
		mysleep(philo->table->time_to_die * 2);
		return (false);
	}
	pthread_mutex_lock(philo->left_fork);
	print_philo_state(TOOK_FORK, philo);
	return (true);
}

bool	philo_eat(t_philo *philo)
{
	if (philo->state == DEAD)
		return (false);
	print_philo_state(EATING, philo);
	pthread_mutex_lock(&philo->table->eat_mutex);
	philo->last_eat_time = get_time_millis();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->table->eat_mutex);
	mysleep(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	print_philo_state(SLEEPING, philo);
	mysleep(philo->table->time_to_sleep);
	print_philo_state(THINKING, philo);
	return (true);
}

void	*check_philo_death(void *arg)
{
	t_philo	*philo;

	philo = arg;
	mysleep(philo->table->time_to_die + 1);
	if (philo->state == FINNISHED)
		return (NULL);
	pthread_mutex_lock(&philo->table->stop_mutex);
	if (!is_dead(philo, false) && (get_time_millis() - philo->last_eat_time)
		>= philo->table->time_to_die)
	{
		print_philo_state(DEAD, philo);
		pthread_mutex_unlock(&philo->table->stop_mutex);
		is_dead(philo, true);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->table->stop_mutex);
	return (NULL);
}
