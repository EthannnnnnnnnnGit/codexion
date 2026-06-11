/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 08:32:45 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/11 08:50:49 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMS_H
# define PARAMS_H

void	get_params(char	**av, t_params params);

typedef struct s_params
{
	int	nb_of_coders;
	int	time_burnout;
	int	time_compile;
	int	time_debug;
	int	time_refactor;
	int	nb_compiles;
	int	dongle_cooldown;
	char	*scheduler;

}	t_params;


#endif