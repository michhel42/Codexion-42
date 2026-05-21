/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:04:50 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:12:45 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define _XOPEN_SOURCE 500
# define SHEDULER_FIFO 1
# define SHEDULER_EDF 0
# define COMPILE "is compiling"
# define DEBUG "is debugging"
# define REFACTOR "is refactoring"
# define DONGLE "has taken a dongle"
# define BURN_OUT "burned out"
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_heap_node
{
	long long	priority;
	int			coder_id;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*data;
	int			capacity;
	int			size;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long long		last_released_time;
	t_heap			queue;
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread_id;
	int				id;
	int				nb_compiles;
	long long		last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_data	*global_data;
}	t_coder;

typedef struct s_data
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				num_compiles_required;
	long long		dongle_cooldown;
	int				sheduler_type;
	int				simulation_running;
	long long		start;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor_id;
}	t_data;

// heap.c
void		heapify_down(t_heap *heap, int i);
t_heap_node	heap_pop(t_heap *heap);
void		heap_push(t_heap *heap, long long priority, int coder_id);
void		heap_remove_coder(t_heap *heap, int coder_id);

// utils.c
void		print_log(t_data *data, int id, char *msg);
void		swap_nodes(t_heap_node *a, t_heap_node *b);
int			ft_atoi(const char *nptr);
int			ft_strcmp(const char *s1, const char *s2);

// dongle_manager.c
int			acquire_dongle(t_coder *coder, t_dongle *dongle);
void		release_dongles(t_coder *coder);
void		release_dongle(t_dongle *dongle, int coder_id);

// routine.c
void		*coder_routine(void *arg);

// monitor.c
void		*monitor_routine(void *arg);
int			is_simulation_over(t_data *data);

// cleaner.c
void		free_all(t_data *data);

// checker.c
int			parser_checker(int ac, char **av);

// init.c
int			init_data(t_data *data, char **av, int ac);
void		init_coders_start(t_data *data);

// timer.c
int			is_cooldown_over(t_dongle *dongle, long long duration);
void		ft_usleep(long long time_in_ms);
long long	get_time_ms(void);

#endif