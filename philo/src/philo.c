/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/31 17:11:45 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_philo *philo, size_t id, t_fork *right_fork)
{
	philo->philo_id = ++id;
	philo->last_eat_time = 0;
	philo->n_eat = 0;
	pthread_mutex_init(&philo->philo_mutex, NULL);
	philo->left_fork = NULL;
	philo->right_fork = right_fork;
	philo->eating = 0;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_philo_state(THINKING, philo);
	if ((philo->philo_id) % 2 == 0)
	{
		mysleep(table()->time_to_eat / 2);
	}
	pthread_mutex_lock(&table()->stop_mutex);
	while (!table()->stop)
	{
		pthread_mutex_unlock(&table()->stop_mutex);
		if (!take_forks(philo) || !philo_eat(philo))
		{
			report_dead_philo(philo);
			return (NULL);
		}
		if (table()->n_eat != (size_t) - 1 && table()->n_eat == philo->n_eat)
			return (NULL);
		pthread_mutex_lock(&table()->stop_mutex);
	}
	pthread_mutex_unlock(&table()->stop_mutex);
	return (NULL);
}

bool	take_forks(t_philo *philo)
{
	if ((philo->philo_id) % 2)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->eating = 1;
	pthread_mutex_unlock(&philo->philo_mutex);
	print_philo_state(TOOK_FORK, philo);
	if (table()->n_philo == 1)
	{
		mysleep(table()->time_to_die);
		return (false);
	}
	if ((philo->philo_id) % 2)
		pthread_mutex_lock(philo->left_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	print_philo_state(TOOK_FORK, philo);
	return (!is_dead(false));
}

bool	philo_eat(t_philo *philo)
{
	//long	s;

	print_philo_state(EATING, philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_eat_time = get_time_millis();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->philo_mutex);
	mysleep(table()->time_to_eat);
	if (is_dead(false))
		return (false);
	unlock_forks(philo);
	print_philo_state(SLEEPING, philo);
	mysleep(table()->time_to_sleep);
	if (am_i_dead(philo))
		return (false);
	print_philo_state(THINKING, philo);
	/*if ((table()->n_philo) % 2)
		s = table()->time_to_die - table()->time_to_eat - table()->time_to_sleep - 50;
	else
		s = table()->time_to_die - (table()->time_to_eat * 2) - table()->time_to_sleep - 50;
	if (s > (long) table()->time_to_eat || s < 0)
	{
		printf("%zu %zu|%d|%d\n", philo->philo_id, s, s > (long) table()->time_to_eat, s < 0);	
		s = 0;
	}
	printf("%zu will think for %zu\n", philo->philo_id, s);
	mysleep(s);*/
	return (!is_dead(false));
}

bool	am_i_dead(t_philo *philo)
{
	size_t	time;

	pthread_mutex_lock(&philo->philo_mutex);
	time = get_time_millis() - philo->last_eat_time;
	if (is_dead(false) || time > table()->time_to_die)
	{
		print_philo_state(DEAD, philo);
		pthread_mutex_unlock(&philo->philo_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	return (false);
}
