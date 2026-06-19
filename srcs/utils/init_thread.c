/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:10:42 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/19 15:24:45 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/coders.h"
#include "../../includes/parsing.h"
#include "../../includes/simulation.h"
#include "../../includes/utils.h"
#include <sys/time.h>

static void	link_dongle(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i + 1])
	{
		coders[i + 1]->second_dongle = coders[i]->first_dongle;
		i++;
	}
	coders[0]->second_dongle = coders[i]->first_dongle;
}

static void	init_coder(int i, t_coder *coder, t_global *global)
{
	coder->id = i + 1;
	coder->nb_compiled = 0;
	coder->global = global;
	coder->first_dongle = malloc(sizeof(t_dongle));
	coder->coder = malloc(sizeof(pthread_t));
	memset(coder->first_dongle->queue, 0, 2);
	coder->first_dongle->mutex_dongle = malloc(sizeof(pthread_mutex_t));
	coder->first_dongle->mutex_queue = malloc(sizeof(pthread_mutex_t));
	if (!coder->first_dongle)
		return ;
	if (pthread_mutex_init(coder->first_dongle->mutex_dongle, NULL))
		return ;
	if (pthread_mutex_init(coder->first_dongle->mutex_queue, NULL))
		return ;
	if (pthread_create(coder->coder, NULL, simulation, coder))
		return ;
}

static void	init_global(t_global *global, t_params	*params, pthread_cond_t *start)
{
	pthread_cond_t	burn_cond;

	global->params = params;
	global->start_cond = start;
	global->has_burnout = 0;
	global->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!strcmp(params->scheduler, "fifo"))
		global->scheduler = fifo;
	else
		global->scheduler = edf;
	if (pthread_cond_init(&burn_cond, NULL))
		return ;
	if (pthread_mutex_init(global->print_mutex, NULL))
		return ;
	global->burn_cond = &burn_cond;
}

void	initialization(t_params *params, t_coder **coders, pthread_t *monitor)
{
	t_global	*global;
	pthread_cond_t	start_cond;
	int			i;

	monitor = malloc(sizeof(pthread_t));
	if (!monitor)
		return ;
	*coders = malloc(sizeof(t_coder *) * params->nb_of_coders + 1);
	if (!coders)
		return ;
	global = malloc(sizeof(t_global));
	if (!global)
		return ;
	if (pthread_cond_init(&start_cond, NULL))
		return ;
	i = 0;
	init_global(global, params, &start_cond);
	while (i < params->nb_of_coders)
	{
		coders[i] = malloc(sizeof(t_coder));
		init_coder(i, coders[i], global);
		i++;
	}
	coders[i] = NULL;
	link_dongle(coders);
	global->time = get_time(0);
	pthread_cond_broadcast(&start_cond);
}
