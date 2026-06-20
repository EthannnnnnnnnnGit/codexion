/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethan <ethan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/20 09:24:32 by ethan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "utils.h"
#include "simulation.h"

void	get_burnout_time(t_coder *coder, struct timespec *time)
{
	struct timeval	tv;
	int				burnout;

	burnout = (coder->global->params->time_burnout -
		(get_time(coder->global->time) - coder->last_compiled));
	// printf("%i", burnout);
	if (gettimeofday(&tv, NULL) == -1 || burnout <= 0)
	{
		time->tv_nsec = 0;
		time->tv_sec = 0;
		return ;
	}
	time->tv_sec = tv.tv_sec;
	time->tv_nsec = (tv.tv_usec * 1000) + (burnout * 1000000);
	if (time->tv_nsec >= 1000000000) {
 		time->tv_sec += 1;
    	time->tv_nsec -= 1000000000;
	}
}


unsigned int	get_time(unsigned int start)
{
	struct timeval	tv;
	unsigned int	time;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (!start)
		return (time);
	return (time - start);
}

void	compiling(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i is compiling\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	coder->nb_compiled++;
	usleep(1000 * coder->global->params->time_compile);
	pthread_mutex_unlock(coder->second_dongle->mutex_dongle);
	coder->second_dongle->queue[0] = coder->second_dongle->queue[1];
	coder->second_dongle->queue[1] = 0;
	pthread_mutex_unlock(coder->first_dongle->mutex_dongle);
	coder->first_dongle->queue[0] = coder->first_dongle->queue[1];
	coder->first_dongle->queue[1] = 0;
	coder->last_compiled = get_time(coder->global->time);
}

void	debugging(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i debugging\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_debug);
}

void	refactoring(t_coder *coder)
{
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i is refactoring\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_refactor);
}


int	take_dongle(t_coder *coder)
{
	struct timespec	time_burnout;
	
	coder->global->scheduler(coder, coder->first_dongle);
	pthread_mutex_lock(coder->first_dongle->mutex_dongle);
	while (coder != coder->first_dongle->queue[0])
	{
		get_burnout_time(coder, &time_burnout);
		pthread_cond_timedwait(coder->global->burn_cond, 
			coder->first_dongle->mutex_dongle, &time_burnout);
		printf("%i", get_time(coder->global->time) - coder->last_compiled);
		if (get_time(coder->global->time) - coder->last_compiled >= coder->global->params->time_burnout)
			return (1);
	}
	pthread_mutex_lock(coder->global->print_mutex);
	printf("%i %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	coder->global->scheduler(coder, coder->second_dongle);
	pthread_mutex_lock(coder->second_dongle->mutex_dongle);
	while (coder != coder->second_dongle->queue[0])
	{
		get_burnout_time(coder, &time_burnout);
		pthread_cond_timedwait(coder->global->burn_cond, 
			coder->second_dongle->mutex_dongle, &time_burnout);
		if (get_time(coder->global->time) - coder->last_compiled >= coder->global->params->time_burnout)
			return (1);
	}
	pthread_mutex_lock(coder->global->print_mutex);	
	printf("%i %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(coder->global->print_mutex);
	return (0);
}


void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(coder->global->print_mutex);
	pthread_cond_wait(coder->global->start_cond, coder->global->print_mutex);
	pthread_mutex_unlock(coder->global->print_mutex);
	while (coder->global->params->nb_compiles > coder->nb_compiled)
	{
		if (take_dongle(coder))
			return (NULL);
		compiling(coder);
		debugging(coder);
		refactoring(coder);
	}
	return (NULL);
}
