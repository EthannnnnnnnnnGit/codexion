/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 10:16:30 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/20 10:31:53 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "utils.h"

int	compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->burn_mutex);
	if (coder->global->has_burnout)
	{
		pthread_mutex_unlock(&coder->global->burn_mutex);
		pthread_mutex_unlock(&coder->second_dongle->mutex_dongle);
		pthread_mutex_unlock(&coder->first_dongle->mutex_dongle);
		return (1);
	}
	pthread_mutex_unlock(&coder->global->burn_mutex);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is compiling\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	pthread_mutex_lock(&coder->mutex_compile);
	coder->last_compiled = get_time(coder->global->time);
	pthread_mutex_unlock(&coder->mutex_compile);
	usleep(1000 * coder->global->params->time_compile);
	pthread_mutex_lock(&coder->mutex_compile);
	coder->nb_compiled++;
	pthread_mutex_unlock(&coder->mutex_compile);
	release_dongle(coder->second_dongle);
	release_dongle(coder->first_dongle);
	return (0);
}

int	debugging(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->burn_mutex);
	if (coder->global->has_burnout)
	{
		pthread_mutex_unlock(&coder->global->burn_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->global->burn_mutex);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i debugging\n", get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_debug);
	return (0);
}

int	refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->burn_mutex);
	if (coder->global->has_burnout)
	{
		pthread_mutex_unlock(&coder->global->burn_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->global->burn_mutex);
	pthread_mutex_lock(&coder->global->print_mutex);
	printf("%lli %i is refactoring\n",
		get_time(coder->global->time), coder->id);
	pthread_mutex_unlock(&coder->global->print_mutex);
	usleep(1000 * coder->global->params->time_refactor);
	return (0);
}
