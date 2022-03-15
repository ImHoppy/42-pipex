/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:11:13 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/15 16:58:16 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	wait_childs(t_pipex pipex)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < pipex.a.ac - 2)
	{
		if (pipex.pid[i] > 0)
			waitpid(pipex.pid[i], &status, 0);
		i++;
	}
	return (status);
}

void	first_child(t_pipex pipex, int pipefd[2], char *infile, char *cmd)
{
	t_fd	fd;

	pipex.cmd_args = ft_split(cmd, ' ');
	pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
	if (!pipex.cmd)
		child_error(&pipex, pipex.cmd_args[0], ERR_NOENT, 127);
	fd = open(infile, O_RDONLY);
	if (fd < 0)
		child_error(&pipex, infile, strerror(errno), 1);
	dup2(pipefd[WRITE_END], STDOUT);
	dup2(fd, STDIN);
	close(pipefd[READ_END]);
	execve(pipex.cmd, pipex.cmd_args, pipex.a.envp);
	child_error(&pipex, pipex.cmd_args[0], ERR_NOENT, 1);
}

void	mid_cmd(t_pipex pipex, t_fd pipefd[2], t_fd oldpipefd[2], char *cmd)
{
	pipex.cmd_args = ft_split(cmd, ' ');
	pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
	if (!pipex.cmd)
		child_error(&pipex, pipex.cmd_args[0], ERR_NOENT, 127);
	dup2(oldpipefd[READ_END], STDIN);
	dup2(pipefd[WRITE_END], STDOUT);
	close(oldpipefd[WRITE_END]);
	close(pipefd[READ_END]);
	execve(pipex.cmd, pipex.cmd_args, pipex.a.envp);
	child_error(&pipex, pipex.cmd_args[0], ERR_CMD, 0);
}

void	last_child(t_pipex pipex, int pipefd[2], char	*outfile, char *cmd)
{
	t_fd	fd;

	pipex.cmd_args = ft_split(cmd, ' ');
	pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
	if (!pipex.cmd)
		child_error(&pipex, pipex.cmd_args[0], ERR_NOENT, 127);
	fd = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (fd < 0)
		child_error(&pipex, outfile, strerror(errno), 1);
	dup2(pipefd[READ_END], STDIN);
	dup2(fd, STDOUT);
	close(pipefd[WRITE_END]);
	execve(pipex.cmd, pipex.cmd_args, pipex.a.envp);
	child_error(&pipex, pipex.cmd_args[0], ERR_CMD, 0);
}
