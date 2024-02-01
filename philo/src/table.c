/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:02:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/31 23:56:17 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	validate_args(int argc, char **argv)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < (size_t)argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

bool	valid_int(size_t t)
{
	return (t > 0 && t <= 2147483647);
}

void	table_init(t_table *table, int argc, char *argv[])
{
	if (!validate_args(argc - 1, &argv[1]))
		exit(1);
	table->stop = false;
	table->has_printed_dead = false;
	table->n_philo = ft_atoi(argv[1]);
	table->time2die = ft_atoi(argv[2]);
	table->time2eat = ft_atoi(argv[3]);
	table->time2sleep = ft_atoi(argv[4]);
	table->n_eat = -1;
	if (argc == 6)
	{
		table->n_eat = ft_atoi(argv[5]);
		if (!valid_int(table->n_eat))
			exit(1);
	}
	if (!valid_int(table->n_philo) || !valid_int(table->time2die)
		|| !valid_int(table->time2eat) || !valid_int(table->time2die))
		exit(1);
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->stop_mutex, NULL);
}

t_table	*table(void)
{
	static t_table	*t;

	if (!t)
	{
		t = malloc(sizeof(t_table));
		if (!t)
			exit(1);
	}
	return (t);
}

bool	everyone_has_eaten(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->philos[i].philo_mutex);
		if (table->philos[i].n_eat < table->n_eat)
		{
			pthread_mutex_unlock(&table->philos[i].philo_mutex);
			return (false);
		}
		pthread_mutex_unlock(&table->philos[i].philo_mutex);
		i++;
	}
	return (true);
}
