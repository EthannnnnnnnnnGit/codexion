/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:33:01 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/26 16:50:43 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "simulation.h"

void	wait_coders(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders[0].global->params->nb_of_coders)
	{
		pthread_join(coders[i].coder, NULL);
		i++;
	}
}

static int	detect_burnout(t_coder *coders)
{
	int	finished;
	int			i;

	i = 0;
	finished = 0;
	while (i < coders[0].global->params->nb_of_coders)
	{
		if (get_time(coders[i].global->time) - coders[i].last_compiled > coders[i].global->params->time_burnout)
		{
			pthread_mutex_lock(&coders[i].global->print_mutex);
			printf("%lli %i has burnout\n", get_time(coders[i].global->time), coders[i].id);
			coders[0].global->has_burnout = true;
			wait_coders(coders);
			printf("here");
			pthread_mutex_unlock(&coders[i].global->print_mutex);
			return (1);
		}
		if (coders[i].nb_compiled >= coders[i].global->params->nb_compiles)
			finished++;
		i++;
	}
	if (finished == i + 1)
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
	while (1)
	{
		if (detect_burnout(coders))
			return (NULL);
	}
	return (NULL);
}
