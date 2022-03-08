/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 13:09:50 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/08 16:39:22 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *s1, char *s2)
{
	write(2, s1, ft_strlen(s1));
	write(2, ": ", 2);
	write(2, s2, ft_strlen(s2));
	write(2, "\n", 1);
}

void	free_cmd(t_pipex *pipex)
{
	int	i;

	if (pipex->cmd_args)
	{
		i = 0;
		while (pipex->cmd_args[i])
		{
			free(pipex->cmd_args[i]);
			i++;
		}
		free(pipex->cmd_args);
	}
	if (pipex->cmd)
		free(pipex->cmd);
}

void	free_path(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (pipex->paths && pipex->paths[i])
	{
		free(pipex->paths[i]);
		i++;
	}
	free(pipex->paths);
}

void	close_fd(t_pipex *pipex)
{
	close(pipex->pipefd[WRITE_END]);
	close(pipex->pipefd[READ_END]);
	close(pipex->infile);
	close(pipex->outfile);
}

int	open_fd(t_pipex *pipex, int argc, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	pipex->outfile = open(argv[argc - 1], O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (pipex->infile < 0)
		return (perror(argv[1]), 0);
	if (pipex->outfile < 0)
		return (perror(argv[argc - 1]), 1);
	if (pipe(pipex->pipefd) == -1)
		return (perror("pipe"), 1);
	return (-1);
}
