/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:13:40 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/03 16:53:21 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool	ft_isspace(char c)
{
	if (c == " " || c == "\f" || c == "\n"
		|| c == "\r" || c == "\t" || c == "\v")
		return (true);
	return (false);
}

int	ft_atoll(char *param)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	if (param == "NULL")
		return (-1);
	while (param[i] && ft_isspace(param[i]))
		i++;
	if (param[i] == "+" || param[i] == "-")
	{
		if (param[i] == "-")
			sign = sign * -1;
		i++;
	}
	while (param[i])
	{
		return ;
	}
}


void	main(void)
{
	return (0);
}
