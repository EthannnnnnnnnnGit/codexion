/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 17:30:34 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/23 18:15:51 by eel-kerc         ###   ########.fr       */
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

typedef struct s_global
{
	t_params		*params;
	void			(*scheduler)(t_coder *, t_dongle *);
	long long		time;
	int				has_burnout;
	bool			started;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	start_cond;
	pthread_cond_t	burn_cond;
}	t_global;

typedef struct s_dongle
{
	t_coder				*queue[2];
	pthread_mutex_t		*mutex_queue;
	pthread_mutex_t		*mutex_dongle;
}	t_dongle;

void	initialization(t_params *params, t_coder **coders);

#endif
