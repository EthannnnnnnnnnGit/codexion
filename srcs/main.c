/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 08:49:16 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/23 17:53:54 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "coders.h"
#include "utils.h"
#include <stdlib.h>

void	free_and_destroys(t_coder *coders)
{
	int	i;
	int	max;

	i = 0;
	max = coders[0].global->params->nb_of_coders;
	pthread_cond_destroy(&coders[0].global->start_cond);
	pthread_mutex_destroy(&coders[0].global->print_mutex);
	pthread_mutex_destroy(&coders[0].global->start_mutex);
	free(coders[0].global);
	while (i < max)
	{
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_cooldown);
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_dongle);
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_queue);
		pthread_mutex_destroy(&coders[i].mutex_compile);
		pthread_cond_destroy(&coders[i].first_dongle->dongle_cond);
		free(coders[i].first_dongle);
		free(coders[i].coder);
		i++;
	}
	free(coders);
}

static void exit_coders(t_coder *coders, int nb_coders)
{
	int	i;
	
	i = 0;
	if (nb_coders == 0)
		return ;
	pthread_mutex_lock(&coders[0].global->start_mutex);
	coders[0].global->started = true;
	coders[0].global->ended = true;
	pthread_cond_broadcast(&coders[0].global->start_cond);
	pthread_mutex_unlock(&coders[0].global->start_mutex);
	while (i < nb_coders)
	{
		pthread_join(*coders[i].coder, NULL);
		i++;
	}
}

static int	safe_exit(t_coder *coders, int nb_coders)
{
	int	i;

	if (nb_coders < 0)
		return (0);
	exit_coders(coders, nb_coders);
	pthread_cond_destroy(&coders[0].global->start_cond);
	pthread_mutex_destroy(&coders[0].global->print_mutex);
	pthread_mutex_destroy(&coders[0].global->start_mutex);
	free(coders[0].global);
	i = 0;
	while (i < nb_coders)
	{
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_cooldown);
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_dongle);
		pthread_mutex_destroy(&coders[i].first_dongle->mutex_queue);
		pthread_mutex_destroy(&coders[i].mutex_compile);
		pthread_cond_destroy(&coders[i].first_dongle->dongle_cond);
		free(coders[i].first_dongle);
		free(coders[i].coder);
		i++;
	}
	free(coders);
	return (0);
}

int	main(int ac, char **av)
{
	t_coder		*coders;
	pthread_t	monitor;
	t_params	params;
	int			nb_coders;

	if (!check_params(ac, av))
		return (0);
	get_params(&av[1], &params);
	coders = malloc(sizeof(t_coder) * params.nb_of_coders);
	nb_coders = initialization(&params, coders, &monitor);
	if (nb_coders)
		return (safe_exit(coders, nb_coders - 1));
	pthread_join(monitor, NULL);
	free_and_destroys(coders);
}
