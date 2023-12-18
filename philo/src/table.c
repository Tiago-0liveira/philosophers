/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:02:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/15 16:47:15 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	table_monitor(t_table *table)
{
	while (true)
	{
		if (is_anyone_dead(table))
		{
			printf("someone died\n");
			return ;
		}

		mysleep(BASE_MICROSEC_SLEEP * 2);
	}
}
