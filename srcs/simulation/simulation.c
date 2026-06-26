/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/26 17:01:31 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "utils.h"
#include "simulation.h"

void	waking_time(t_coder *coder, struct timespec *time, int dongle_cooldown)
{
	struct timeval	tv;
	long long		burnout;
	int				cooldown;

	burnout = (coder->global->params->time_burnout
			- (get_time(coder->global->time) - coder->last_compiled));
	dongle_cooldown = dongle_cooldown - get_time(coder->global->time);
	if (burnout <= 0)
		burnout = 0;
	if (dongle_cooldown <= 0)
		dongle_cooldown = 0;
	if (burnout < dongle_cooldown)
		cooldown = burnout;
	else
		cooldown = dongle_cooldown;
	if (gettimeofday(&tv, NULL) == -1)
	{
		time->tv_nsec = 0;
		time->tv_sec = 0;
		return ;
	}
	time->tv_sec = tv.tv_sec;
	time->tv_nsec = (tv.tv_usec * 1000) + (cooldown * 1000000);
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

int	compiling(t_coder *coder)
{
	if (coder->global->has_burnout)
		return (1);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is compiling\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	coder->nb_compiled++;
	usleep(1000 * coder->global->params->time_compile);
	pthread_mutex_unlock(&coder->second_dongle->mutex_dongle);
	pthread_mutex_lock(&coder->second_dongle->mutex_queue);
	coder->second_dongle->queue[0] = coder->second_dongle->queue[1];
	coder->second_dongle->queue[1] = NULL;
	pthread_mutex_unlock(&coder->second_dongle->mutex_queue);
	pthread_cond_broadcast(&coder->second_dongle->dongle_cond);
	pthread_mutex_unlock(&coder->first_dongle->mutex_dongle);
	pthread_mutex_lock(&coder->first_dongle->mutex_queue);
	coder->first_dongle->queue[0] = coder->first_dongle->queue[1];
	coder->first_dongle->queue[1] = NULL;
	pthread_mutex_unlock(&coder->first_dongle->mutex_queue);
	coder->last_compiled = get_time(coder->global->time);
	pthread_cond_broadcast(&coder->first_dongle->dongle_cond);
	return (0);
}

int	debugging(t_coder *coder)
{
	if (coder->global->has_burnout)
		return (1);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i debugging\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_debug);
	return (0);
}

int	refactoring(t_coder *coder)
{
	if (coder->global->has_burnout)
		return (1);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is refactoring\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_refactor);
	return (0);
}

int	taking(t_coder *coder, t_dongle *dongle)
{
	struct timespec	waking;

	coder->global->scheduler(coder, dongle);
	pthread_mutex_lock(&dongle->mutex_queue);
	while (coder != dongle->queue[0] || get_time(coder->global->time) < dongle->cooldown)
	{
		waking_time(coder, &waking, dongle->cooldown);
		pthread_cond_timedwait(&dongle->dongle_cond,
			&dongle->mutex_queue, &waking);
		if (get_time(coder->global->time) - coder->last_compiled >= coder->global->params->time_burnout)
		{
			pthread_mutex_unlock(&dongle->mutex_queue);
			return (1);
		}
	}
	if (coder->global->has_burnout)
		return (1);
	pthread_mutex_unlock(&dongle->mutex_queue);
	pthread_mutex_lock(&dongle->mutex_dongle);
	pthread_mutex_lock(&coder->global->print_mutex);
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
		{
			pthread_mutex_unlock(&coder->first_dongle->mutex_dongle);
			return (1);
		}
	}
	else
	{
		if (taking(coder, coder->second_dongle))
			return (1);
		if (taking(coder, coder->first_dongle))
		{
			pthread_mutex_unlock(&coder->second_dongle->mutex_dongle);
			return (1);
		}
	}
	printf("%lli %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	printf("%lli %i has taken a dongle\n", get_time(coder->global->time), coder->id);
	coder->first_dongle->cooldown = get_time(coder->global->time) + coder->global->params->time_compile + coder->global->params->dongle_cooldown;
	coder->second_dongle->cooldown = get_time(coder->global->time) + coder->global->params->time_compile + coder->global->params->dongle_cooldown;
	return (0);
}

void	*simulation(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->global->start_mutex);
	if (!coder->global->started)
		pthread_cond_wait(&coder->global->start_cond, &coder->global->start_mutex);
	pthread_mutex_unlock(&coder->global->start_mutex);
	coder->last_compiled = get_time(coder->global->time);
	while (coder->global->params->nb_compiles > coder->nb_compiled)
	{
		if (take_dongle(coder))
			return (NULL);
		if (compiling(coder))
			return (NULL);
		if (debugging(coder))
			return (NULL);
		if (refactoring(coder))
			return (NULL);
	}
	return (NULL);
}
