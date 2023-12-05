/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:04:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/05 15:47:26 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	//check_args(argc, argv);
	(void)argc;
	(void)argv;
	milisleep(1000);
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
