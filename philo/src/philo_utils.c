/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:38:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/15 16:46:56 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_philo_state(enum e_philo_state state, t_philo *philo)
{
	size_t	elapsed_time;

	phtread_mutex_lock(philo->table->print_mutex);
	elapsed_time = get_elapsed_time(philo->table->start_time);
	if (state == THINKING)
		printf(PHILO_FORMAT_THINKING, elapsed_time, philo->philo_id);
	else if (state == EATING)
		printf(PHILO_FORMAT_EATING, elapsed_time, philo->philo_id);
	else if (state == SLEEPING)
		printf(PHILO_FORMAT_SLEEPING, elapsed_time, philo->philo_id);
	else if (state == DEAD)
		printf(PHILO_FORMAT_DIED, elapsed_time, philo->philo_id);
	else if (state == TOOK_FORK)
		printf(PHILO_FORMAT_TOOK_FORK, elapsed_time, philo->philo_id);
	else
		printf("Error: invalid state\n");
	phtread_mutex_unlock(philo->table->print_mutex);
}

void	start_threads(t_table *table)
{
	size_t	i;

	table->start_time = get_time_millis();
	i = 0;
	while (i < table->n_philo)
	{
		if (pthread_create(&table->philos[i]->thread_id, NULL,
				(void *)philo_routine, table->philos[i]) != 0)
		{
			printf("Error: could not create thread\n");
			return ;
		}
		i++;
	}
}

bool	is_anyone_dead(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->n_philo)
	{
		if (table->philos[i]->state == DEAD)
			return (true);
		i++;
	}
	return (false);
}

bool	check_last_meal_time(t_philo *philo)
{
	size_t	elapsed_time;

	elapsed_time = get_elapsed_time(philo->last_eat_time);
	if (elapsed_time > philo->table->time_to_die)
	{
		phtread_mutex_lock(philo->philo_mutex);
		philo->state = DEAD;
		print_philo_state(DEAD, philo);
		phtread_mutex_unlock(philo->philo_mutex);
		return (true);
	}
	return (false);
}
