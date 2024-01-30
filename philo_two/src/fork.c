/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:20:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/30 15:56:31 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_left_forks(t_table *table)
{
	size_t	i;

	i = 0;
	if (table->n_philo < 2)
		return ;
	table->philos[i++].left_fork = &table->forks[table->n_philo - 1];
	while (i < table->n_philo)
	{
		table->philos[i].left_fork = &table->forks[i - 1];
		i++;
	}
}

void	unlock_forks(t_philo *philo)
{
	if ((philo->philo_id + 1) % 2)
	{
		pthread_mutex_unlock(philo->right_fork);
		if (philo->left_fork)
			pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		if (philo->left_fork)
			pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	pthread_mutex_lock(&philo->philo_mutex);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
}