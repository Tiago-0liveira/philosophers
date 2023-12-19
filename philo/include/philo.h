/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:03:14 by tiagoliv          #+#    #+#             */
/*   Updated: 2023/12/19 00:16:01 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define USAGE "Usage: ./philo <n_philo> <time_to_die> \
<time_to_eat> <time_to_sleep> [<number_of_times_each_philosopher_must_eat>]"

# define K * 1000
# define BASE_MICROSEC_SLEEP 50

# define PHILO_FORMAT_THINKING "%lu %d is thinking\n"
# define PHILO_FORMAT_EATING "%lu %d is eating\n"
# define PHILO_FORMAT_SLEEPING "%lu %d is sleeping\n"
# define PHILO_FORMAT_DIED "%lu %d died\n"
# define PHILO_FORMAT_TOOK_FORK "%lu %d has taken a fork\n"

enum e_philo_state {
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	TOOK_FORK
};

enum e_fork_state {
	FREE,
	USED
};

typedef struct s_fork {
	pthread_mutex_t		*mutex;
	//enum e_fork_state	state;
}	t_fork;

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

typedef struct s_table {
	size_t			n_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	size_t			n_eat;
	t_fork			**forks;
	t_philo			**philos; // maybe unnecessary
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print_mutex;
}	t_table;

typedef struct s_philo {
	pthread_t			thread_id;
	size_t				philo_id;
	size_t				eat_count;
	size_t				action_time_elapsed;
	size_t				last_eat_time;
	enum e_philo_state	state;
	pthread_mutex_t		*philo_mutex; //maybe unnecessary
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_table				*table;
}	t_philo;

// main.c
bool	check_args(int argc, char *argv[]);
void	load_args(t_table *table, int argc, char *argv[]);
void	start_simulation(t_table *table);
void	free_and_exit(t_table *table);

// philo.c
t_philo	*make_philo(size_t id, t_table *table, t_fork *right_fork);
void	philo_routine(void *arg);
void	philo_eat(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_sleep(t_philo *philo);

// fork.c
t_fork	*make_fork(void);
void	assign_left_forks(t_table *table);

// utils.c
size_t	get_time_millis(void);
void	mysleep(size_t millis);
void	*free_ptr_and_return_null(void **ptr);
size_t	get_elapsed_time(size_t start);
int		ft_atoi(const char *nptr);

// philo_utils.c
void	print_philo_state(enum e_philo_state state, t_philo *philo);
void	start_threads(t_table *table);
bool	is_anyone_dead(t_table *table);
// table.c

size_t	get_milis(struct timeval *tv);
void	milisleep(size_t	milis);

#endif
