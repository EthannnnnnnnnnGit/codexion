/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 10:16:30 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/23 17:07:02 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"
#include "utils.h"

int	safe_print(t_coder *coder, const char *task)
{
	pthread_mutex_lock(&coder->global->print_mutex);
	pthread_mutex_lock(&coder->global->end_mutex);
	if (coder->global->has_burnout)
	{
		pthread_mutex_unlock(&coder->global->end_mutex);
		pthread_mutex_unlock(&coder->global->print_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->global->end_mutex);
	printf("%lli %i is %s\n", get_time(coder->global->time), coder->id, task);
	pthread_mutex_unlock(&coder->global->print_mutex);
	return (1);
}

int	compiling(t_coder *coder)
{
	if (!safe_print(coder, "compiling"))
	{
		pthread_mutex_unlock(&coder->second_dongle->mutex_dongle);
		pthread_mutex_unlock(&coder->first_dongle->mutex_dongle);
		return (1);
	}
	pthread_mutex_lock(&coder->mutex_compile);
	coder->last_compiled = get_time(coder->global->time);
	pthread_mutex_unlock(&coder->mutex_compile);
	usleep(1000 * coder->global->params->time_compile);
	pthread_mutex_lock(&coder->mutex_compile);
	coder->nb_compiled++;
	pthread_mutex_unlock(&coder->mutex_compile);
	release_dongle(coder, coder->second_dongle);
	release_dongle(coder, coder->first_dongle);
	return (0);
}

int	debugging(t_coder *coder)
{
	if (!safe_print(coder, "debugging"))
		return (1);
	usleep(1000 * coder->global->params->time_debug);
	return (0);
}

int	refactoring(t_coder *coder)
{
	if (!safe_print(coder, "refactoring"))
		return (1);
	usleep(1000 * coder->global->params->time_refactor);
	return (0);
}
