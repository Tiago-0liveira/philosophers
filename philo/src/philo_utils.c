/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:38:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/12 16:41:11 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_philo_state(enum e_philo_state state, t_philo *philo)
{
	size_t	elapsed_time;

	pthread_mutex_lock(philo->table->print_mutex);
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
	pthread_mutex_unlock(philo->table->print_mutex);
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
	i = 0;
	while (i < table->n_philo)
	{
		pthread_join(table->philos[i]->thread_id, NULL);
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

	elapsed_time = get_elapsed_time(philo->table->start_time);
	//TODO: printf("id: %lu, elapsed_time: %lu\n", philo->philo_id, elapsed_time);
	if (elapsed_time > philo->table->time_to_die)
	{
		pthread_mutex_lock(philo->philo_mutex);
		philo->state = DEAD;
		print_philo_state(DEAD, philo);
		pthread_mutex_unlock(philo->philo_mutex);
		return (true);
	}
	return (false);
}

void	start_simulation(t_table *table)
{
	size_t	i;

	i = 0;
	table->forks = malloc(sizeof(t_fork *) * table->n_philo);
	table->philos = malloc(sizeof(t_philo *) * table->n_philo);
	if (!table->forks || !table->philos)
		free_and_exit(table);
	while (i < table->n_philo)
	{
		table->forks[i] = make_fork();
		if (!table->forks[i])
			free_and_exit(table);
		table->philos[i] = make_philo(i, table, table->forks[i]);
		if (!table->philos[i])
			free_and_exit(table);
		i++;
	}
	assign_left_forks(table);
	start_threads(table);
}
