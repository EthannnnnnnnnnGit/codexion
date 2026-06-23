/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 08:49:16 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/23 15:28:20 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "coders.h"
#include "utils.h"
#include <stdlib.h>

void	join_coder(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		pthread_join(*coders[i]->coder, NULL);
		i++;
	}
}

void	free_and_destroys(t_coder **coders)
{
	int	i;

	i = 0;
	pthread_cond_destroy(&coders[0]->global->start_cond);
	pthread_cond_destroy(&coders[0]->global->burn_cond);
	pthread_mutex_destroy(&coders[0]->global->print_mutex);
	free(coders[0]->global);
	while (coders[i])
	{
		pthread_mutex_destroy(coders[i]->first_dongle->mutex_dongle);
		pthread_mutex_destroy(coders[i]->first_dongle->mutex_queue);
		free(coders[i]->first_dongle->mutex_dongle);
		free(coders[i]->first_dongle->mutex_queue);
		free(coders[i]->first_dongle);
		free(coders[i]->coder);
		free(coders[i]);
		i++;
	}
	// free(coders);
	// free(monitor);
}

int	main(int ac, char **av)
{
	t_coder		*coders;
	// pthread_t	monitor;
	t_params	params;

	if (!check_params(ac, av))
		return (0);
	get_params(&av[1], &params);
	initialization(&params, &coders);
	join_coder(&coders);
	free_and_destroys(&coders);
}
