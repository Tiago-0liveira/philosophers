/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 15:38:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/30 17:11:35 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_philo_state(enum e_philo_state state, t_philo *philo)
{
	size_t	elapsed_time;

	if (is_dead(false))
		return ;
	pthread_mutex_lock(&table()->print_mutex);
	elapsed_time = get_time_millis() - table()->start_time;
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
	pthread_mutex_unlock(&table()->print_mutex);
}

void	start_threads(t_table *table)
{
	size_t		i;

	table->start_time = get_time_millis();
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->philos[i].philo_mutex);
		table->philos[i].last_eat_time = table->start_time;
		pthread_mutex_unlock(&table->philos[i].philo_mutex);
		if (pthread_create(&table->philos[i].thread_id, NULL,
				philo_routine, &table->philos[i]) != 0)
		{
			printf("Error: could not create thread\n");
			return ;
		}
		i++;
	}
	//pthread_create(&table->hunger_thread, NULL, check_death, table);
}

void	start_simulation(t_table *table)
{
	size_t	i;

	i = 0;
	table->forks = malloc(sizeof(t_fork) * table->n_philo);
	table->philos = malloc(sizeof(t_philo) * table->n_philo);
	if (!table->forks || !table->philos)
		free_and_exit(table);
	while (i < table->n_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		init_philo(&table->philos[i], i, &table->forks[i]);
		i++;
	}
	assign_left_forks(table);
	start_threads(table);
}

bool	is_dead(bool dead_flag)
{
	pthread_mutex_lock(&table()->stop_mutex);
	if (dead_flag)
		table()->stop = true;
	if (table()->stop)
	{
		pthread_mutex_unlock(&table()->stop_mutex);
		return (true);
	}
	pthread_mutex_unlock(&table()->stop_mutex);
	return (false);
}

void	*check_death(void *arg)
{
	t_table	*table;
	size_t	phi;

	table = arg;
	phi = 0;
	while (is_dead(false))
	{
		if (phi == table->n_philo)
		{
			mysleep(1);
			phi = 0;
		}
		pthread_mutex_lock(&table->philos[phi].philo_mutex);
		if (am_i_dead(&table->philos[phi]))
		{
			pthread_mutex_unlock(&table->philos[phi].philo_mutex);
			print_philo_state(DEAD, &table->philos[phi]);
			printf("dingding %zu died\n", table->philos[phi].philo_id);
			is_dead(true);
			return (NULL);
		}
		pthread_mutex_unlock(&table->philos[phi].philo_mutex);
		phi++;
	}
	printf("ended monitor check\n");
	return (NULL);
}
