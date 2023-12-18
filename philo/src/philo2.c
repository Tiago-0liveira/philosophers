/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:04:49 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/15 16:27:16 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*make_philo(size_t id, t_table *table, t_fork *right_fork)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->philo_id = id;
	philo->eat_count = 0;
	philo->action_time_elapsed = 0;
	philo->last_eat_time = 0;
	philo->state = THINKING;
	philo->philo_mutex = malloc(sizeof(pthread_mutex_t));
	if (!philo->philo_mutex)
		return (free_ptr_and_return_null(&philo));
	if (pthread_mutex_init(philo->philo_mutex, NULL) != 0)
		return (free_ptr_and_return_null(&philo));
	philo->left_fork = NULL;
	philo->right_fork = right_fork;
	philo->table = table;
	return (philo);
}

void	philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->state != DEAD || !is_anyone_dead(philo->table))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
}

void	philo_eat(t_philo *philo)
{
	if (is_anyone_dead(philo->table))
		return (free_and_exit(philo->table));
	pthread_mutex_lock(philo->left_fork->mutex);
	pthread_mutex_lock(philo->right_fork->mutex);
	phtread_mutex_lokc(philo->philo_mutex);
	philo->state = EATING;
	print_philo_state(TOOK_FORK, philo);
	print_philo_state(EATING, philo);
	mysleep(philo->table->time_to_eat);
	philo->last_eat_time = get_time_millis();
	philo->eat_count++;
	phtread_mutex_unlock(philo->left_fork->mutex);
	phtread_mutex_unlock(philo->right_fork->mutex);
	phtread_mutex_unlock(philo->philo_mutex);
}

void	philo_think(t_philo *philo)
{
	if (is_anyone_dead(philo->table))
		return (free_and_exit(philo->table));
	phtread_mutex_lock(philo->philo_mutex);
	philo->state = THINKING;
	print_philo_state(philo->state, philo);
	phtread_mutex_unlock(philo->philo_mutex);
	mysleep(philo->table->time_to_sleep);
}

void	philo_sleep(t_philo *philo)
{
	if (is_anyone_dead(philo->table))
		return (free_and_exit(philo->table));
	phtread_mutex_lock(philo->philo_mutex);
	philo->state = SLEEPING;
	print_philo_state(philo->state, philo);
	phtread_mutex_unlock(philo->philo_mutex);
	mysleep(philo->table->time_to_sleep);
}
