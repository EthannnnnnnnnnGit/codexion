/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/23 17:36:02 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "utils.h"
#include "simulation.h"

void	wait_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->start_mutex);
	if (!coder->global->started)
		pthread_cond_wait(&coder->global->start_cond,
			&coder->global->start_mutex);
	pthread_mutex_unlock(&coder->global->start_mutex);
	pthread_mutex_lock(&coder->mutex_compile);
	coder->last_compiled = get_time(coder->global->time);
	if (coder->id % 2 == 0)
		usleep(1000);
	pthread_mutex_unlock(&coder->mutex_compile);
}

void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_start(coder);
	pthread_mutex_lock(&coder->global->end_mutex);
	while (!coder->global->ended)
	{
		pthread_mutex_unlock(&coder->global->end_mutex);
		if (take_dongle(coder))
			return (NULL);
		if (compiling(coder))
			return (NULL);
		if (debugging(coder))
			return (NULL);
		if (refactoring(coder))
			return (NULL);
		pthread_mutex_lock(&coder->global->end_mutex);
	}
	pthread_mutex_unlock(&coder->global->end_mutex);
	return (NULL);
}
