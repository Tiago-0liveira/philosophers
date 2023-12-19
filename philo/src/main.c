/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:04:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/19 00:16:17 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_table	table;

	if (!check_args(argc, argv))
		return (1); // check subject for error handling
	load_args(&table, argc, argv);
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

void	load_args(t_table *table, int argc, char *argv[])
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
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

void	free_and_exit(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_join(table->philos[i]->thread_id, NULL);
		i++;
	}
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
	free(table->forks);
	free(table->philos);
	exit(1);
}
