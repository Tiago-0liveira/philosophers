/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:04:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/04 23:50:53 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	check_args(argc, argv);

	return (0);
}

bool	check_args(int argc, char *argv[])
{
	if (argc < 4 || argc > 5)
	{
		printf("%s\n", USAGE);
		return (false);
	}
}
