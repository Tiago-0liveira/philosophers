/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/29 17:59:12 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_philo *philo, size_t id, t_table *table,
		t_fork *right_fork)
{
	philo->philo_id = id;
	philo->last_eat_time = 0;
	philo->n_eat = 0;
	pthread_mutex_init(&philo->philo_mutex, NULL);
	philo->left_fork = NULL;
	philo->right_fork = right_fork;
	philo->table = table;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if ((philo->philo_id + 1) % 2 == 0)
	{
		//printf("%zu will beg sleep %zums\n", philo->philo_id, philo->table->time_to_eat / 2);
		mysleep(philo->table->time_to_eat / 2);
	}
	pthread_mutex_lock(&philo->table->stop_mutex);
	while (!philo->table->stop)
	{
		pthread_mutex_unlock(&philo->table->stop_mutex);
		if (!take_forks(philo) || is_dead(philo, false) || !philo_eat(philo))
		{
			print_philo_state(DEAD, philo);
			printf("%zu setting true here\n", philo->philo_id);
			is_dead(philo, true);
			//printf("%zu thread ended\n", philo->philo_id);
			return (NULL);
		}
		if (philo->table->n_eat != (size_t) - 1 && philo->table->n_eat == philo->n_eat)
			return (NULL);
		pthread_mutex_lock(&philo->table->stop_mutex);
	}
	pthread_mutex_unlock(&philo->table->stop_mutex);
	//printf("%zu thread ended\n", philo->philo_id);
	return (NULL);
}

bool	take_forks(t_philo *philo)
{
	if ((philo->philo_id + 1) % 2)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	print_philo_state(TOOK_FORK, philo);
	if (philo->table->n_philo == 1)
	{
		mysleep(philo->table->time_to_die);
		return (false);
	}
	/*else if (am_i_dead(philo))
		return (false);*/
	if ((philo->philo_id + 1) % 2)
		pthread_mutex_lock(philo->left_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	//printf("%zu took second fork\n", philo->philo_id);
	print_philo_state(TOOK_FORK, philo);
	return (true);
}

bool	philo_eat(t_philo *philo)
{
	print_philo_state(EATING, philo);
	//printf("%zu tel:%zu\n", philo->philo_id, get_time_millis() - philo->last_eat_time);
	/*if (get_time_millis() - philo->last_eat_time + philo->table->time_to_eat > philo->table->time_to_die)
	{
		printf("%zu will weird sleep for %zums\n", philo->philo_id, philo->table->time_to_die - (get_time_millis() - philo->last_eat_time));
		
		mysleep(philo->table->time_to_die - (get_time_millis() - philo->last_eat_time));
		return (false);
	}*/
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_eat_time = get_time_millis();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->philo_mutex);
	mysleep(philo->table->time_to_eat);
	if (is_dead(philo, false))
		return (false);
	if ((philo->philo_id + 1) % 2)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	print_philo_state(SLEEPING, philo);
	mysleep(philo->table->time_to_sleep);
	//printf("%zu here12\n", philo->philo_id);
	if (am_i_dead(philo))
		return (false);
	print_philo_state(THINKING, philo);
	mysleep((philo->table->time_to_die - (get_time_millis() - philo->last_eat_time)) / 2);
	return (is_dead(philo, false));
}

bool	am_i_dead(t_philo *philo)
{
	size_t	time;
	//pthread_mutex_lock(&philo->philo_mutex);
	if (philo->last_eat_time == 0)
		philo->last_eat_time = get_time_millis();
	time = get_time_millis() - philo->last_eat_time;
	if (is_dead(philo, false) || time >= philo->table->time_to_die)
	{
		printf("		%zu cond %d %d\n", philo->philo_id, time >= philo->table->time_to_die, is_dead(philo, false));
		print_philo_state(DEAD, philo);
		is_dead(philo, true);
		//pthread_mutex_unlock(&philo->philo_mutex);
		return (true);
	}
	//pthread_mutex_unlock(&philo->philo_mutex);
	return (false);
}
