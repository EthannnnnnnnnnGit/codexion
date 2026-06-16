/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedulers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:25:14 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/16 17:40:58 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/coders.h"

void	edf(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(dongle->mutex_queue);
	if (!dongle->queue[0])
		dongle->queue[0] = coder;
	else if (coder->last_compiled < dongle->queue[0]->last_compiled)
	{
		dongle->queue[1] = dongle->queue[0];
		dongle->queue[0] = coder;
	}
	else
		dongle->queue[1] = coder;
	pthread_mutex_unlock(dongle->mutex_queue);
}

void	fifo(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(dongle->mutex_queue);
	if (!dongle->mutex_queue)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
	pthread_mutex_unlock(dongle->mutex_queue);
}