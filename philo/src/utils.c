/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 17:07:12 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/15 16:45:39 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time_millis(void)
{
	struct timeval	time;
	size_t			millis;

	gettimeofday(&time, NULL);
	millis = time.tv_sec K + time.tv_usec / 1000;
	return (millis);
}

void	mysleep(size_t millis)
{
	size_t	start;

	start = get_time_millis();
	while (get_time_millis() - start < millis)
		usleep(BASE_MICROSEC_SLEEP);
}

void	*free_ptr_and_return_null(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

size_t	get_elapsed_time(size_t start)
{
	return (get_time_millis() - start);
}

int	ft_atoi(const char *nptr)
{
	int		r;
	int		s;
	char	*d;

	d = (char *) nptr;
	r = 0;
	s = 1;
	while (*d == ' ' || *d == '\n' || *d == '\t' || *d == '\v'
		|| *d == '\f' || *d == '\r')
		d++;
	if (*d == '-' || *d == '+')
	{
		if (*d == '-')
			s = -1;
		d++;
	}
	while (*d >= '0' && *d <= '9')
	{
		r = r * 10 + *d - '0';
		d++;
	}
	return (r * s);
}
