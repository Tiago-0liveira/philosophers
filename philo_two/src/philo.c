/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/24 18:50:30 by tiagoliv         ###   ########.fr       */
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
	if (philo->philo_id % 2 == 0)
		mysleep(philo->table->time_to_eat / 10);
	while (!is_dead(philo, false))
	{
		//printf("%zu loop start\n", philo->philo_id);
		if (!take_forks(philo))
		{
			printf("%zu will cancel thread 1\n", philo->philo_id);
			return (NULL);
		}
		//printf("%zu here1\n", philo->philo_id);
		//printf("%zu %zu is eating \n", get_elapsed_time(philo->table->start_time), philo->philo_id);
		mysleep(philo->table->time_to_eat);
		/*if (am_i_dead(philo))
		{
			printf("%zu will cancel thread 2\n", philo->philo_id);
			return (NULL);
		}*/
		//printf("%zu here2\n", philo->philo_id);
		if (philo->table->n_eat != (size_t) - 1 && philo->table->n_eat == philo->n_eat)
		{
			printf("%zu will cancel thread 3\n", philo->philo_id);
			return (NULL);
		}
		//printf("%zu here\n", philo->philo_id);
	}
	return (NULL);
}

bool	take_forks(t_philo *philo)
{
	if (philo->philo_id % 2 && (philo->table->n_philo + 1) % 2)
	{
		//printf("%zu will take first fork %p\n", philo->philo_id, philo->left_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		//printf("%zu will take first fork %p\n", philo->philo_id, philo->right_fork);
		pthread_mutex_lock(philo->right_fork);
	}
	print_philo_state(TOOK_FORK, philo);
	if (philo->table->n_philo == 1)
	{
		mysleep(philo->table->time_to_die);
		return (false);
	}
	/*else if (am_i_dead(philo))
		return (false);*/
	if (philo->philo_id % 2 && (philo->table->n_philo + 1) % 2)
	{
		pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		//printf("%zu will take second fork %p\n", philo->philo_id, philo->left_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	//printf("%zu took second fork\n", philo->philo_id);
	print_philo_state(TOOK_FORK, philo);
	return (true);
}

bool	philo_eat(t_philo *philo)
{
	printf("%zu here\n", philo->thread_id);
	print_philo_state(EATING, philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_eat_time = get_time_millis();
	philo->n_eat++;
	pthread_mutex_unlock(&philo->philo_mutex);
	printf("here\n");
	mysleep(philo->table->time_to_eat);
	if (philo->philo_id % 2 && (philo->table->n_philo + 1) % 2)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	print_philo_state(SLEEPING, philo);
	mysleep(philo->table->time_to_sleep);
	print_philo_state(THINKING, philo);
	return (true);
}

bool	am_i_dead(t_philo *philo)
{
	if (philo->last_eat_time == 0)
		philo->last_eat_time = get_time_millis();
	/*printf("let:%zu|%d\n", get_time_millis() - philo->last_eat_time,
		is_dead(philo, false) || (get_time_millis()
			- philo->last_eat_time) >= philo->table->time_to_die);*/
	if (is_dead(philo, false) || (get_time_millis()
			- philo->last_eat_time) >= philo->table->time_to_die)
	{
		print_philo_state(DEAD, philo);
		is_dead(philo, true);
		return (true);
	}
	return (false);
}
