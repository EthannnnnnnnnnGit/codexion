/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 08:49:16 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/17 15:00:58 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"
#include "../includes/params.h"
#include "../includes/coders.h"
#include "../includes/utils.h"

void	join_coder(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		pthread_join(*coders[i]->coder, NULL);
	}
}


int	main(int ac, char **av)
{
	t_coder		*coder;
	pthread_t	monitor;
	t_params	params;

	if (!check_params(ac, av))
		return (0);
	get_params(&av[1], &params);
	initialization(&params, &coder, &monitor);
	// join_coder(&coder);
}
