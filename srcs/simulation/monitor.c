/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:33:01 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/11 17:40:00 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "simulation.h"

static void	wait_coders(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders[0].global->params->nb_of_coders)
	{
		pthread_join(*coders[i].coder, NULL);
		i++;
	}
}

void	alarm_burnout(t_coder *coders, int i)
{
	pthread_mutex_unlock(&coders[i].mutex_compile);
	pthread_mutex_lock(&coders[0].global->burn_mutex);
	coders[0].global->has_burnout = true;
	pthread_mutex_unlock(&coders[0].global->burn_mutex);
	pthread_mutex_lock(&coders[i].global->print_mutex);
	printf("%lli %i burned out\n",
			get_time(coders[i].global->time), coders[i].id);
	wait_coders(coders);
	pthread_mutex_unlock(&coders[i].global->print_mutex);
}

static int	detect_burnout(t_coder *coders)
{
	int	finished;
	int	i;

	i = 0;
	finished = 0;
	while (i < coders[0].global->params->nb_of_coders)
	{
		pthread_mutex_lock(&coders[i].mutex_compile);
		if (get_time(coders[i].global->time) - coders[i].last_compiled
			> coders[i].global->params->time_burnout)
		{
			alarm_burnout(coders, i);
			return (1);
		}
		if (coders[i].nb_compiled >= coders[i].global->params->nb_compiles)
			finished++;
		pthread_mutex_unlock(&coders[i].mutex_compile);
		i++;
	}
	if (finished == i)
	{
		wait_coders(coders);
		return (1);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_coder		*coders;

	coders = (t_coder *)arg;
	pthread_mutex_lock(&coders[0].global->start_mutex);
	if (!coders[0].global->started)
		pthread_cond_wait(&coders[0].global->start_cond,
			&coders[0].global->start_mutex);
	pthread_mutex_unlock(&coders[0].global->start_mutex);
	while (1)
	{
		if (detect_burnout(coders))
		{
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
