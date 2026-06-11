/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:10:42 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/11 10:31:11 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/coders.h"
#include "../includes/params.h"


void	lunch_coders(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		pthread_join(coders[i], NULL);
		i++;
	}
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
		coders[i]->id = i + 1;
		coders[i]->compile = params->time_compile;
		coders[i]->debug = params->time_debug;
		coders[i]->refactor = params->time_refactor;
		coders[i]->compiles_required = params->nb_compiles;
		coders[i]->nb_compiled = 0;
		if (pthread_create(coders[i]->coder, NULL, init_coders, NULL))
			return ;
		i++;
	}
	coders[i] = (void *)0;
	lunch_coders(coders);
}
