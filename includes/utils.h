/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 17:30:34 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/30 15:34:57 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include "parsing.h"
# include "coders.h"
# include <stdbool.h>

typedef struct s_dongle	t_dongle;
typedef struct s_global	t_global;
typedef struct s_coder t_coder;

typedef struct s_global
{
	t_params		*params;
	void			(*scheduler)(t_coder *, t_dongle *);
	long long		time;
	int				has_burnout;
	bool			started;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	burn_mutex;
	pthread_cond_t	start_cond;
}	t_global;

typedef struct s_dongle
{
	int					cooldown;
	t_coder				*queue[2];
	pthread_cond_t		dongle_cond;
	pthread_mutex_t		mutex_cooldown;
	pthread_mutex_t		mutex_queue;
	pthread_mutex_t		mutex_dongle;
}	t_dongle;

typedef struct s_coder
{
	unsigned int	id;
	long long		last_compiled;
	pthread_mutex_t	mutex_compile;
	unsigned int	nb_compiled;
	t_dongle		*first_dongle;
	t_dongle		*second_dongle;
	pthread_t		*coder;
	t_global		*global;
}	t_coder;

void	initialization(t_params *params, t_coder *coders, pthread_t *monitor);

#endif
