/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 08:49:31 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/18 16:40:23 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h>
# include <stdio.h>
# include <string.h>

typedef struct s_params
{
	int				nb_of_coders;
	unsigned int	time_burnout;
	unsigned int	time_compile;
	unsigned int	time_debug;
	unsigned int	time_refactor;
	unsigned int	nb_compiles;
	unsigned int	dongle_cooldown;
	char	*scheduler;

}	t_params;

void	get_params(char	**av, t_params *params);
bool	check_params(int ac, char **av);

#endif