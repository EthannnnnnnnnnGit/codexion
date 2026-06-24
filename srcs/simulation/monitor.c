/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:33:01 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/24 10:47:29 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "simulation.h"

void	stop_coders(t_coder *coders)
{
	return ;
}

static int	detect_burnout(t_coder *coders)
{
	static int	finished = 0;
	int			i;

	i = 0;
	while (&coders[i])
	{
		if (get_time(coders[i].global->time) - coders[i].last_compiled > coders[i].global->params->time_burnout)
		{
			pthread_mutex_lock(&coders[i].global->print_mutex);
			printf("%lli %i has burnout\n", get_time(coders[i].global->time), coders[i].id);
			stop_coders(coders);
			pthread_mutex_unlock(&coders[i].global->print_mutex);
			return ;
		}
	}
	if (finished == i + 1)
		stop_coders(coders);
}

void	monitoring(void *arg)
{
	t_coder		*coders;

	coders = (t_coder *)arg;
	while (1)
	{
		pthread_cond_wait(&coders[0].global->has_burnout);
		if (simulation_stopped(coders))
			return ;
	}
}