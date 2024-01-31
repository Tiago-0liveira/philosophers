/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:03:14 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/01/31 16:36:38 by tiagoliv         ###   ########.fr       */
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

# define BASE_MICROSEC_SLEEP 50

# define PHILO_FORMAT_THINKING "%lu %lu is thinking\n"
# define PHILO_FORMAT_EATING "%lu %lu is eating\n"
# define PHILO_FORMAT_SLEEPING "%lu %lu is sleeping\n"
# define PHILO_FORMAT_DIED "%lu %lu died\n"
# define PHILO_FORMAT_TOOK_FORK "%lu %lu has taken a fork\n"

enum e_philo_state {
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	TOOK_FORK,
	FINNISHED
};

typedef pthread_mutex_t	t_fork;

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

typedef struct s_table {
	size_t			n_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	size_t			n_eat;
	t_fork			*forks;
	t_philo			*philos;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_t		hunger_thread;
	bool			stop;
	bool			has_printed_dead;
}	t_table;

typedef struct s_philo {
	pthread_t			thread_id;
	size_t				philo_id;
	size_t				last_eat_time;
	pthread_mutex_t		philo_mutex;
	size_t				n_eat;
	t_fork				*left_fork;
	t_fork				*right_fork;
	bool				eating;
}	t_philo;

// main.c
void	start_simulation(t_table *table);
void	free_and_exit(t_table *table);

// philo.c
void	init_philo(t_philo *philo, size_t id, t_fork *right_fork);
void	*philo_routine(void *arg);
bool	take_forks(t_philo *philo);
bool	philo_eat(t_philo *philo);
bool	am_i_dead(t_philo *philo);

// fork.c
void	assign_left_forks(t_table *table);
void	unlock_forks(t_philo *philo);
void	report_dead_philo(t_philo *philo);

// utils.c
size_t	get_time_millis(void);
void	mysleep(size_t millis);
size_t	get_elapsed_time(size_t start);
int		ft_atoi(const char *nptr);

// philo_utils.c
void	print_philo_state(enum e_philo_state state, t_philo *philo);
void	start_threads(t_table *table);
void	start_simulation(t_table *table);
bool	is_dead(bool dead_flag);
void	*check_death(void *arg);

// table.c
bool	valid_int(size_t t);
void	table_init(t_table *table, int argc, char *argv[]);
t_table	*table(void);

#endif
