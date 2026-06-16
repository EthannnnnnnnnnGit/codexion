/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 17:30:34 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/16 17:36:25 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <pthread.h>
# include <string.h>

typedef struct s_global
{
	int				burnout;
	int				compile;
	int				debug;
	int				refactor;
	int				compiles_required;
	void			(*scheduler)(t_coder *, t_dongle *);
	int				time;
	pthread_mutex_t	*print_mutex;
}	t_global;

typedef struct s_dongle
{
	t_coder				*queue[2];
	pthread_mutex_t	*mutex_queue;
	pthread_mutex_t	*mutex_dongle;
}	t_dongle;

#endif
