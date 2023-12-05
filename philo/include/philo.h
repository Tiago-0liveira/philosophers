/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:03:14 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/05 15:47:04 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

# define USAGE "Usage: ./philo <n_philo> <time_to_die> \
<time_to_eat> <time_to_sleep> [<number_of_times_each_philosopher_must_eat>]"

enum e_philo_state {
	THINKING,
	EATING,
	SLEEPING,
	DEAD
};

typedef struct s_philo {
	pthread_t			thread_id;
	size_t				eat_count;
	size_t				action_time_elapsed;
	enum e_philo_state	state;
	pthread_mutex_t		*philo_mutex;
}	t_philo;

typedef struct s_table {
	size_t				n_philo;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				start_millis;
	t_philo				*philos;
}	t_table;

size_t	get_milis(struct timeval *tv);
void	milisleep(size_t	milis);

#endif
