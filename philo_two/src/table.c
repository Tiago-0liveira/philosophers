/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:02:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/24 16:37:50 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	validate_args(int argc, char **argv)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < (size_t) argc)
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

void	table_init(t_table *table, int argc, char *argv[])
{
	if (!validate_args(argc - 1, &argv[1]))
		exit(1);
	table->stop = false;
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->n_eat = -1;
	if (argc == 6)
	{
		table->n_eat = ft_atoi(argv[5]);
		if (table->n_eat == 0)
			exit(1);
	}
	if (table->n_philo == 0)
		exit(1);
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->stop_mutex, NULL);
}
