/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/17 15:01:53 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "utils.h"
#include "simulation.h"

int		get_time(int start)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (get_time(start));
	if (!start)
		return (tv.tv_sec);
	return (start - tv.tv_sec);
}

void	compiling(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i is compiling\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	coder->nb_compiled++;
	usleep(1000 * coder->global->compile);
	pthread_mutex_unlock(coder->second_dongle->mutex_dongle);
	coder->second_dongle->queue[0] = coder->second_dongle->queue[1];
	coder->second_dongle->queue[1] = 0;
	pthread_mutex_unlock(coder->first_dongle->mutex_dongle);
	coder->first_dongle->queue[0] = coder->first_dongle->queue[1];
	coder->first_dongle->queue[1] = 0;
}

void	debugging(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i debugging\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	usleep(1000 * coder->global->debug);
}

void	refactoring(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i is refactoring\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	usleep(1000 * coder->global->refactor);
}

void	take_dongle(t_coder *coder)
 {
	coder->global->scheduler(coder, coder->first_dongle);
	while (coder != coder->first_dongle->queue[0])
		continue ;
	pthread_mutex_lock(coder->first_dongle->mutex_dongle);
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	coder->global->scheduler(coder, coder->second_dongle);
	while (coder != coder->second_dongle->queue[0])
		continue ;
	pthread_mutex_lock(coder->second_dongle->mutex_dongle);
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
}

void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("here");
	pthread_cond_wait(coder->global->start_cond, coder->global->print_mutex);
	printf("here");
	while (coder->global->compiles_required > coder->nb_compiled)
	{
		take_dongle(coder);
		compiling(coder);
		debugging(coder);
		refactoring(coder);
	}
	return (NULL);
}
