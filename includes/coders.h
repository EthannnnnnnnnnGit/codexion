/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 08:28:27 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/11 11:15:09 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_coder
{
	int			id;
	int			compile;
	int			debug;
	int			refactor;
	int			compiles_required;
	int			nb_compiled;
	pthread_t	coder;
}	t_coder;

#endif