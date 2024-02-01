/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/01 01:14:14 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_philo *philo, size_t id, t_fork *right_fork)
{
	philo->philo_id = ++id;
	philo->last_eat_time = 0;
	philo->has_left_table = false;
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
	if (philo->philo_id % 2 == 0)
	{
		mysleep(table()->time2eat / 2);
	}
	while (!is_dead(false))
	{
		if (!take_forks(philo) || !philo_eat(philo))
		{
			report_dead_philo(philo);
			return (NULL);
		}
		if (table()->n_eat != (size_t) - 1 && table()->n_eat == philo->n_eat)
		{
			pthread_mutex_lock(&philo->philo_mutex);
			philo->has_left_table = true;
			pthread_mutex_unlock(&philo->philo_mutex);
			return (NULL);
		}
	}
	return (NULL);
}

bool	take_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
		pthread_mutex_lock(philo->left_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->eating = 1;
	pthread_mutex_unlock(&philo->philo_mutex);
	print_philo_state(TOOK_FORK, philo);
	if (table()->n_philo == 1)
	{
		mysleep(table()->time2die);
		return (false);
	}
	if (philo->philo_id % 2 == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	print_philo_state(TOOK_FORK, philo);
	return (!is_dead(false));
}

bool	philo_eat(t_philo *philo)
{
	long	s;

	print_philo_state(EATING, philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_eat_time = get_time_millis();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->philo_mutex);
	mysleep(table()->time2eat);
	if (is_dead(false))
		return (false);
	unlock_forks(philo);
	print_philo_state(SLEEPING, philo);
	mysleep(table()->time2sleep);
	print_philo_state(THINKING, philo);
	if (table()->n_philo % 2 == 0)
		s = table()->time2die - table()->time2eat - table()->time2sleep;
	else
		s = table()->time2die - (table()->time2eat * 2) - table()->time2sleep;
	if (s > (long)table()->time2eat || s < 0 || table()->n_philo >= 15)
		s = 0;
	mysleep(s);
	return (!is_dead(false));
}

bool	am_i_dead(t_philo *philo)
{
	size_t	time;

	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->has_left_table)
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return (false);
	}
	time = get_time_millis() - philo->last_eat_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	if (time > table()->time2die)
	{
		print_philo_state(DEAD, philo);
		return (true);
	}
	return (false);
}
