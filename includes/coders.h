/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 08:28:27 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/23 11:35:11 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_dongle	t_dongle;
typedef struct s_global	t_global;

typedef struct s_coder
{
	unsigned int	id;
	long long		last_compiled;
	unsigned int	nb_compiled;
	t_dongle		*first_dongle;
	t_dongle		*second_dongle;
	pthread_t		*coder;
	t_global		*global;
}	t_coder;

#endif