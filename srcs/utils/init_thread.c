/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:10:42 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/26 15:46:06 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/coders.h"
#include "../../includes/parsing.h"
#include "../../includes/simulation.h"
#include "../../includes/utils.h"
#include <sys/time.h>

static void	link_dongle(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders[0].global->params->nb_of_coders - 1)
	{
		coders[i + 1].second_dongle = coders[i].first_dongle;
		i++;
	}
	coders[0].second_dongle = coders[i].first_dongle;
}

static void	init_coder(int i, t_coder *coder, t_global *global)
{
	coder->id = i + 1;
	coder->nb_compiled = 0;
	coder->global = global;
	coder->last_compiled = 0;
	coder->first_dongle = malloc(sizeof(t_dongle));
	coder->first_dongle->queue[0] = NULL;
	coder->first_dongle->queue[1] = NULL;
	coder->first_dongle->cooldown = 0;
	if (pthread_mutex_init(&coder->first_dongle->mutex_dongle, NULL))
		return ;
	if (pthread_mutex_init(&coder->first_dongle->mutex_queue, NULL))
		return ;
	if (pthread_cond_init(&coder->first_dongle->dongle_cond, NULL))
		return ;
	if (pthread_create(&coder->coder, NULL, simulation, coder))
		return ;
}

static void	init_global(t_global *global, t_params	*params)
{
	global->params = params;
	global->has_burnout = 0;
	global->time = 0;
	global->started = false;
	if (!strcmp(params->scheduler, "fifo"))
		global->scheduler = fifo;
	else
		global->scheduler = edf;
	if (pthread_cond_init(&global->burn_cond, NULL))
		return ;
	if (pthread_cond_init(&global->start_cond, NULL))
		return ;
	if (pthread_mutex_init(&global->print_mutex, NULL))
		return ;
	if (pthread_mutex_init(&global->start_mutex, NULL))
		return ;
	global->time = get_time(0);
}

void	initialization(t_params *params, t_coder *coders, pthread_t *monitor)
{
	t_global		*global;
	int				i;

	global = malloc(sizeof(t_global));
	if (!global)
		return ;
	i = 0;
	init_global(global, params);
	while (i < params->nb_of_coders)
	{
		init_coder(i, &coders[i], global);
		i++;
	}
	link_dongle(coders);
	global->time = get_time(0);
	pthread_mutex_lock(&global->start_mutex);
	global->started = true;
	pthread_mutex_unlock(&global->start_mutex);
	pthread_cond_broadcast(&global->start_cond);
	if (pthread_create(monitor, NULL, monitoring, coders))
		return ;
}
