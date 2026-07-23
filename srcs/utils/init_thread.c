/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:10:42 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/23 17:39:10 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include "parsing.h"
#include "simulation.h"
#include "utils.h"
#include <sys/time.h>

static int	failed_free(t_coder *coder, const char *error, int advancement)
{
	free(coder->first_dongle);
	free(coder->coder);
	fprintf(stderr, "Failed to %s", error);
	if (advancement > 0)
		pthread_mutex_destroy(&coder->first_dongle->mutex_cooldown);
	if (advancement > 1)
		pthread_mutex_destroy(&coder->first_dongle->mutex_dongle);
	if (advancement > 2)
		pthread_mutex_destroy(&coder->first_dongle->mutex_queue);
	if (advancement > 3)
		pthread_mutex_destroy(&coder->mutex_compile);
	if (advancement > 4)
		pthread_cond_destroy(&coder->first_dongle->dongle_cond);
	return (0);
}

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

static int	init_coder(int i, t_coder *coder, t_global *global)
{
	coder->id = i + 1;
	coder->nb_compiled = 0;
	coder->global = global;
	coder->last_compiled = 0;
	coder->first_dongle = malloc(sizeof(t_dongle));
	coder->coder = malloc(sizeof(pthread_t));
	coder->first_dongle->queue[0] = NULL;
	coder->first_dongle->queue[1] = NULL;
	coder->first_dongle->cooldown = 0;
	if (pthread_mutex_init(&coder->first_dongle->mutex_cooldown, NULL))
		return (failed_free(coder, "initialize dongle mutex.", 0));
	if (pthread_mutex_init(&coder->first_dongle->mutex_dongle, NULL))
		return (failed_free(coder, "initialize dongle mutex.", 1));
	if (pthread_mutex_init(&coder->first_dongle->mutex_queue, NULL))
		return (failed_free(coder, "initialize dongle mutex.", 2));
	if (pthread_mutex_init(&coder->mutex_compile, NULL))
		return (failed_free(coder, "initialize coder mutex.", 3));
	if (pthread_cond_init(&coder->first_dongle->dongle_cond, NULL))
		return (failed_free(coder, "initialize dongle cond variable.", 4));
	if (pthread_create(coder->coder, NULL, simulation, coder))
		return (failed_free(coder, "create coder.", 5));
	return (1);
}

static int	free_global(t_global *global, int advancement)
{
	if (advancement > 0)
		pthread_cond_destroy(&global->start_cond);
	if (advancement > 1)
		pthread_mutex_destroy(&global->print_mutex);
	if (advancement > 2)
		pthread_mutex_destroy(&global->start_mutex);
	free(global);
	return (0);
}

static int	init_global(t_global *global, t_params	*params)
{
	global->params = params;
	global->has_burnout = 0;
	global->time = 0;
	global->started = false;
	global->ended = false;
	if (!strcmp(params->scheduler, "fifo"))
		global->scheduler = fifo;
	else
		global->scheduler = edf;
	if (pthread_cond_init(&global->start_cond, NULL))
		return free_global(global, 0);
	if (pthread_mutex_init(&global->print_mutex, NULL))
		return free_global(global, 1);
	if (pthread_mutex_init(&global->start_mutex, NULL))
		return free_global(global, 2);
	if (pthread_mutex_init(&global->end_mutex, NULL))
		return free_global(global, 3);
	global->time = get_time(0);
	return (1);
}

int	initialization(t_params *params, t_coder *coders, pthread_t *monitor)
{
	t_global		*global;
	int				i;

	global = malloc(sizeof(t_global));
	if (!global)
		return (-1);
	i = 0;
	if (!init_global(global, params))
		return (-1);
	while (i < params->nb_of_coders)
	{
		if (!init_coder(i, &coders[i], global))
			return (i + 1);
		i++;
	}
	if (pthread_create(monitor, NULL, monitoring, coders))
		return (i + 1);
	link_dongle(coders);
	global->time = get_time(0);
	pthread_mutex_lock(&global->start_mutex);
	global->started = true;
	pthread_cond_broadcast(&global->start_cond);
	pthread_mutex_unlock(&global->start_mutex);
	return (0);
}
