/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:20:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/15 16:23:05 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_fork	*make_fork(void)
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	if (!fork)
		return (NULL);
	fork->mutex = malloc(sizeof(pthread_mutex_t));
	if (!fork->mutex)
		free_ptr_and_return_null((void **)&fork);
	if (pthread_mutex_init(fork->mutex, NULL) != 0)
		free_ptr_and_return_null((void **)&fork);
	return (fork);
}

void	assign_left_forks(t_table *table)
{
	size_t	i;

	i = 0;
	if (table->n_philo < 2)
		return ;
	table->philos[i++]->left_fork = table->forks[table->n_philo - 1];
	while (i < table->n_philo)
	{
		table->philos[i]->left_fork = table->forks[i - 1];
		i++;
	}
}
