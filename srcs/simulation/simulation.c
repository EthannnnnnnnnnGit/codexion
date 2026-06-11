/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:34:12 by eel-kerc          #+#    #+#             */
/*   Updated: 2026/06/11 11:15:49 by eel-kerc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/coders.h"

void	simulation(t_coder *coder)
{
	printf("%i %i has taken a dongle", coder->id);
	printf("%i %i has taken a dongle", coder->id);
	printf("%i %i is compiling", coder->id);
	usleep(1000 * coder->compile);
	printf("%i %i debugging", coder->id);
	usleep(1000 * coder->debug);
	printf("%i %i is refactoring", coder->id);
	usleep(1000 * coder->refactor);
}