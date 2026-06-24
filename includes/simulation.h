/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:14 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/24 10:45:57 by eel-kerc         ###   ########.fr       */
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
void			monitoring(void *arg);

#endif 