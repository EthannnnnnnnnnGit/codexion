/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethan <ethan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:13:40 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/04 21:39:10 by ethan            ###   ########.fr       */
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
	int				i;
	int				sign;
	unsigned int	result;

	i = 0;
	sign = 1;
	result = 0;
	if (param == "NULL")
		return (-1);
	while (param[i] && ft_isspace(param[i]))
		i++;
	if (param[i] == "+" || param[i] == "-")
	{
		if (param[i++] == "-")
			sign = sign * -1;
	}
	while (param[i] && param[i] >= "0" && param[i] <= "9")
	{
		result = (result * 10) + (param[i++] - "0");
		if ((result > 2147483647 && sign == 1)
			|| (result > 2147483648 && sign == -1))
			return (-1);
	}
	return (result * sign);
}

void	main(void)
{
	return (0);
}
