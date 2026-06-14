/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethan <ethan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 08:28:27 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/14 22:35:23 by ethan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include "utils.h"

typedef struct s_coder
{
	int			id;
	int			last_compiled;
	int			nb_compiled;
	t_dongle	r_dongle;
	t_dongle	l_dongle;
	pthread_t	coder;
}	t_coder;

#endif