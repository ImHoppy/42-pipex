/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:33:16 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/15 16:36:45 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_all(int newpipefd[2], int pipefd[2])
{
	if (newpipefd[0] > -1 && newpipefd[1] > -1)
	{
		close(newpipefd[0]);
		close(newpipefd[1]);
	}
	close(pipefd[0]);
	close(pipefd[1]);
}

void	switch_fd(int newpipefd[2], int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	pipefd[0] = newpipefd[0];
	pipefd[1] = newpipefd[1];
}
