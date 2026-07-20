/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:51:23 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/07/20 09:58:09 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "simulation.h"

static int	define_cooldown(int burnout, int dongle_cooldown)
{
	int	cooldown;

	if (burnout <= 0)
		burnout = 0;
	if (dongle_cooldown <= 0)
		dongle_cooldown = 0;
	if (burnout < dongle_cooldown)
		cooldown = burnout;
	else
		cooldown = dongle_cooldown;
	return (cooldown);
}

void	waking_time(t_coder *coder, struct timespec *time, int dongle_cooldown)
{
	struct timeval	tv;
	long long		burnout;
	int				cooldown;

	pthread_mutex_lock(&coder->mutex_compile);
	burnout = (coder->global->params->time_burnout
			- (get_time(coder->global->time) - coder->last_compiled));
	pthread_mutex_unlock(&coder->mutex_compile);
	dongle_cooldown = dongle_cooldown - get_time(coder->global->time);
	cooldown = define_cooldown(burnout, dongle_cooldown);
	if (gettimeofday(&tv, NULL) == -1)
	{
		time->tv_nsec = 0;
		time->tv_sec = 0;
		return ;
	}
	time->tv_sec = tv.tv_sec;
	time->tv_nsec = (tv.tv_usec * 1000) + (cooldown * 1000000);
	if (time->tv_nsec >= 1000000000)
	{
		time->tv_sec++;
		time->tv_nsec -= 1000000000;
	}
}

long long	get_time(long long start)
{
	struct timeval	tv;
	long long		time;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (!start)
		return (time);
	return (time - start);
}
