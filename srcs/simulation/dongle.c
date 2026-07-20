/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 11:05:55 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/20 11:15:20 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "utils.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_unlock(&dongle->mutex_dongle);
	pthread_mutex_lock(&dongle->mutex_queue);
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = NULL;
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->mutex_queue);
}

int	taking(t_coder *coder, t_dongle *dongle)
{
	struct timespec	waking;

	coder->global->scheduler(coder, dongle);
	pthread_mutex_lock(&dongle->mutex_queue);
	pthread_mutex_lock(&dongle->mutex_cooldown);
	while (coder != dongle->queue[0] || get_time(coder->global->time)
		< dongle->cooldown || coder->first_dongle == coder->second_dongle)
	{
		waking_time(coder, &waking, dongle->cooldown);
		pthread_mutex_unlock(&dongle->mutex_cooldown);
		pthread_cond_timedwait(&dongle->dongle_cond,
			&dongle->mutex_queue, &waking);
		pthread_mutex_lock(&coder->mutex_compile);
		if (get_time(coder->global->time) - coder->last_compiled
			>= coder->global->params->time_burnout)
		{
			pthread_mutex_unlock(&coder->mutex_compile);
			pthread_mutex_unlock(&dongle->mutex_queue);
			return (1);
		}
		pthread_mutex_unlock(&coder->mutex_compile);
		pthread_mutex_lock(&dongle->mutex_cooldown);
	}
	pthread_mutex_unlock(&dongle->mutex_cooldown);
	pthread_mutex_unlock(&dongle->mutex_queue);
	pthread_mutex_lock(&coder->global->burn_mutex);
	if (coder->global->has_burnout)
	{
		pthread_mutex_unlock(&coder->global->burn_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->global->burn_mutex);
	pthread_mutex_lock(&dongle->mutex_dongle);
	return (0);
}

void	manage_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i has taken a dongle\n",
		get_time(coder->global->time), coder->id);
	printf("%lli %i has taken a dongle\n",
		get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	pthread_mutex_lock(&coder->first_dongle->mutex_cooldown);
	coder->first_dongle->cooldown = (get_time(coder->global->time)
			+ coder->global->params->time_compile
			+ coder->global->params->dongle_cooldown);
	pthread_mutex_unlock(&coder->first_dongle->mutex_cooldown);
	pthread_mutex_lock(&coder->second_dongle->mutex_cooldown);
	coder->second_dongle->cooldown = (get_time(coder->global->time)
			+ coder->global->params->time_compile
			+ coder->global->params->dongle_cooldown);
	pthread_mutex_unlock(&coder->second_dongle->mutex_cooldown);
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
	manage_dongle(coder);
	return (0);
}
