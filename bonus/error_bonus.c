/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:09:35 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/15 16:40:35 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(char *s1, char *s2)
{
	write(2, s1, ft_strlen(s1));
	write(2, ": ", 2);
	write(2, s2, ft_strlen(s2));
	write(2, "\n", 1);
}

void	child_error(t_pipex *pipex, char *s1, char *s2, int exit_status)
{
	if (s1 && s2)
		ft_error(s1, s2);
	free_cmd(pipex);
	free_path(pipex);
	free(pipex->pid);
	exit(exit_status);
}
