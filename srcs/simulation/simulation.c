/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/24 09:51:00 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "utils.h"
#include "simulation.h"

void	get_burnout_time(t_coder *coder, struct timespec *time)
{
	struct timeval	tv;
	long long		burnout;

	burnout = (coder->global->params->time_burnout
			- (get_time(coder->global->time) - coder->last_compiled));
	if (burnout <= 0)
		burnout = 0;
	if (gettimeofday(&tv, NULL) == -1)
	{
		time->tv_nsec = 0;
		time->tv_sec = 0;
		return ;
	}
	time->tv_sec = tv.tv_sec;
	time->tv_nsec = (tv.tv_usec * 1000) + (burnout * 1000000);
	if (time->tv_nsec >= 1000000000)
	{
		time->tv_sec++;
		time->tv_nsec -= 1000000000;
	}
}

long long	get_time(long long start)
{
	struct timeval	tv;
	long long		time;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (!start)
		return (time);
	return (time - start);
}

void	compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is compiling\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
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
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i debugging\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_debug);
}

void	refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is refactoring\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_refactor);
}

int	taking(t_coder *coder, t_dongle *dongle)
{
	struct timespec	time_burnout;

	coder->global->scheduler(coder, dongle);
	pthread_mutex_lock(dongle->mutex_queue);
	while (coder != dongle->queue[0])
	{
		get_burnout_time(coder, &time_burnout);
		pthread_cond_timedwait(&coder->global->burn_cond,
			dongle->mutex_queue, &time_burnout);
		if (get_time(coder->global->time) - coder->last_compiled >= coder->global->params->time_burnout)
		{
			pthread_mutex_unlock(dongle->mutex_queue);
			return (1);
		}
	}
	pthread_mutex_unlock(dongle->mutex_queue);
	pthread_mutex_lock(dongle->mutex_dongle);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	return (0);
}

int	take_dongle(t_coder *coder)
{
	if (coder->id % 2)
	{
		if (taking(coder, coder->first_dongle))
			return (1);
		if (taking(coder, coder->second_dongle))
			return (1);
	}
	else
	{
		if (taking(coder, coder->second_dongle))
			return (1);
		if (taking(coder, coder->first_dongle))
			return (1);
	}
	return (0);
}

void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->global->print_mutex);
	if (!coder->global->started)
		pthread_cond_wait(&coder->global->start_cond, &coder->global->print_mutex);
	pthread_mutex_unlock(&coder->global->print_mutex);
	coder->last_compiled = get_time(coder->global->time);
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
