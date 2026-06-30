/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 08:49:16 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/30 16:05:53 by eel-kerc         ###   ########.fr       */
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

int	main(int ac, char **av)
{
	t_coder		*coders;
	pthread_t	monitor;
	t_params	params;

	if (!check_params(ac, av))
		return (0);
	get_params(&av[1], &params);
	coders = malloc(sizeof(t_coder) * params.nb_of_coders);
	initialization(&params, coders, &monitor);
	pthread_join(monitor, NULL);
	free_and_destroys(coders);
}
