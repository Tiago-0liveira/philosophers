/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:02:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/16 17:32:03 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	table_monitor(void *arg)
{
	t_table *table;

	table = (t_table *)arg;
	pthread_mutex_lock(table->mutex);
	while (table->running)
	{
		pthread_mutex_unlock(table->mutex);
		if (is_anyone_dead(table))
		{
			printf("someone died\n");
			return ;
		}
		mysleep(BASE_MICROSEC_SLEEP);
		pthread_mutex_lock(table->mutex);
	}
	pthread_mutex_unlock(table->mutex);
}

void	table_init(t_table *table, int argc, char *argv[])
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->running = true;
	if (argc == 6)
		table->n_eat = ft_atoi(argv[5]);
	else
		table->n_eat = -1;
	if (table->n_philo < 1 || table->time_to_die < 1 || table->time_to_eat < 1
		|| table->time_to_sleep < 1 || table->n_eat < 1)
	{
		printf("%s\n", USAGE);
		exit(1);
	}
	table->print_mutex = malloc(sizeof(pthread_mutex_t));
	table->mutex = malloc(sizeof(pthread_mutex_t));
	if (!table->print_mutex || pthread_mutex_init(table->print_mutex, NULL) != 0 || !table->mutex
		|| pthread_mutex_init(table->mutex, NULL) != 0)
	{
		printf("Error: pthread mutex init failed!\n");
		exit(1);
	}
}
