/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:04:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/12 16:46:15 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_table	table;

	if (!check_args(argc, argv))
		return (1); // check subject for error handling
	table_init(&table, argc, argv);
	start_simulation(&table);
	return (0);
}

size_t	get_milis(struct timeval *tv)
{
	return (tv->tv_sec * 1000 + tv->tv_usec / 1000);
}

void	milisleep(size_t	milis)
{
	struct timeval	beg;
	struct timeval	end;

	gettimeofday(&beg, NULL);
	while (1)
	{
		gettimeofday(&end, NULL);
		if ((get_milis(&end) - get_milis(&beg)) >= milis)
			return ;
	}
}

bool	check_args(int argc, char *argv[])
{
	(void)argv;
	if (argc < 4 || argc > 5)
	{
		printf("%s\n", USAGE);
		return (false);
	}
	return (true);
}

void	free_and_exit(t_table *table)
{
	size_t	i;

	i = 0;
	printf("here\n");
	while (i < table->n_philo)
	{
		pthread_join(table->philos[i]->thread_id, NULL);
		i++;
	}
	printf("done\n");
	i = 0;
	while (i < table->n_philo)
	{
		if (table->forks[i]->mutex != NULL)
			pthread_mutex_destroy(table->forks[i]->mutex);
		if (table->philos[i]->philo_mutex != NULL)
			pthread_mutex_destroy(table->philos[i]->philo_mutex);
		if (table->forks[i])
			free(table->forks[i]);
		if (table->philos[i])
			free(table->philos[i]);
		i++;
	}
	printf("done2\n");
	free(table->forks);
	free(table->philos);
	exit(1);
}
