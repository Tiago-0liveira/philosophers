/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:20:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/22 16:15:28 by tiagoliv         ###   ########.fr       */
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
