/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:13:40 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/10 10:01:49 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static bool	is_valid_number(char *param, const char *error)
{
	int				i;
	int				sign;
	unsigned int	result;

	i = 0;
	sign = 1;
	result = 0;
	if (param[i] == '+' || param[i] == '-')
	{
		if (param[i++] == '-')
			sign *= -1;
	}
	while (param[i])
	{
		if (!(param[i] >= '0' && param[i] <= '9'))
		{
			fprintf(stderr, "%s should be a valid number\n", error);
			return (false);
		}
		result = (result * 10) + (param[i++] - '0');
		if ((result * sign > 2147483647) || (result * sign < 0))
		{
			fprintf(stderr, "%s should be between 0 and "
				"INT_MAX (2147483647)\n", error);
			return (false);
		}
	}
	return (true);
}

bool	check_params(int ac, char **av)
{
	int			i;
	const char	*errors[] = {"Number of coders", "Time_to_burnout",
		"Time_to_compile", "Time to debug", "Time to refactor",
		"Number of compiles required", "Dongle cooldown"};

	if (ac != 9)
	{
		fprintf(stderr, "Invalid number of args. Args should be in the following "
			"format: number_of_coders time_to_burnout time_to_compile time_to_debug "
			"time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n");
		return (false);
	}
	if (!strcmp(av[1], "0") || !strcmp(av[1], "-0"))
		fprintf(stderr, "Number of coders should be at least one\n");
	i = 1;
	while (i < 8)
	{
		if (!is_valid_number(av[i], errors[i - 1]))
			return (false);
		i++;
	}
	if (strcmp("edf", av[8]) && strcmp("fifo", av[8]))
	{
		fprintf(stderr, "Scheduler should be either \"edf\" or \"fifo\"\n");
		return (false);
	}
	return (true);
}
