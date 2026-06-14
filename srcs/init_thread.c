/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethan <ethan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:10:42 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/12 23:43:23 by ethan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/coders.h"
#include "../includes/params.h"
#include "../includes/simulation.h"


static void	lunch_coders(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		pthread_join(coders[i], NULL);
		i++;
	}
}

static void init_coders(int i, t_coder *coders, t_params *params)
{
	coders->id = i + 1;
	coders->compile = params->time_compile;
	coders->debug = params->time_debug;
	coders->refactor = params->time_refactor;
	coders->compiles_required = params->nb_compiles;
	coders->nb_compiled = 0;
}

t_coder	init_coders(t_params *params)
{
	t_coder	**coders;
	t_coder	*monitor;
	int		i;
	
	monitor = malloc(sizeof(t_coder));
	if (!monitor)
		return ;
	coders = malloc(sizeof(t_coder) * params->nb_of_coders);
	if (!coders)
		return ;
	i = 0;
	while (i < params->nb_of_coders)
	{
		init_coders(i, coders[i], params);
		if (pthread_create(coders[i]->coder, NULL, simulation, NULL))
			return ;
		i++;
	}
	coders[i] = (void *)0;
	lunch_coders(coders);
}
