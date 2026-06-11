/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 08:36:52 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/11 08:49:24 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/params.h"


static int	get_number(char *s)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (s[i] == '+' || s[i] == "-")
		i++;
	while (s[i])
	{
		res = (res * 10) + (s[i]- '0');
		i++; 
	}
	return (res);
}

void	get_params(char	**av, t_params params)
{
	params.nb_of_coders = get_number(av[0]);
	params.time_burnout = get_number(av[1]);
	params.time_compile = get_number(av[2]);
	params.time_debug = get_number(av[3]);
	params.time_refactor = get_number(av[4]);
	params.nb_compiles = get_number(av[5]);
	params.dongle_cooldown = get_number(av[6]);
	params.scheduler = av[7];
}