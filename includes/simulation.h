/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:14 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/23 14:30:04 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H
# define SIMULATION_H

# include "../includes/coders.h"
# include <sys/time.h>

void			*simulation(void *arg);
void			edf(t_coder *coder, t_dongle *dongle);
void			fifo(t_coder *coder, t_dongle *dongle);
long long		get_time(long long start);
void			*monitoring(void *arg);
void			waking_time(t_coder *coder,
					struct timespec *time, int dongle_cooldown);
long long		get_time(long long start);
int				compiling(t_coder *coder);
int				debugging(t_coder *coder);
int				refactoring(t_coder *coder);
void			release_dongle(t_coder *coder, t_dongle *dongle);
int				take_dongle(t_coder *coder);

#endif 