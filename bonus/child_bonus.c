/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:11:13 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/15 11:47:20 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	child_error(t_pipex *pipex)
{
	free_cmd(pipex);
	free_path(pipex);
	ft_error("pipex", "No such files");
	xexit(0);
}

int	first_child(t_pipex pipex, int pipefd[2], char *infile, char *cmd)
{
	t_fd	fd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		pipex.cmd_args = ft_split(cmd, ' ');
		pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
		if (!pipex.cmd)
			child_error(&pipex);
		fd = open(infile, O_RDONLY);
		if (fd < 0)
			return (perror(infile), 1);
		dup2(pipefd[WRITE_END], STDOUT);
		dup2(fd, STDIN);
		close(pipefd[READ_END]);
		execve(pipex.cmd, pipex.cmd_args, pipex.args.envp);
	}
	return (0);
}

int	execute_cmd(t_pipex pipex, t_fd pipefd[2], t_fd oldpipefd[2], char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		pipex.cmd_args = ft_split(cmd, ' ');
		pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
		if (!pipex.cmd)
			child_error(&pipex);
		dup2(oldpipefd[READ_END], STDIN);
		dup2(pipefd[WRITE_END], STDOUT);
		close(oldpipefd[WRITE_END]);
		close(pipefd[READ_END]);
		execve(pipex.cmd, pipex.cmd_args, pipex.args.envp);
		xexit(1);
	}
	return (pid);
}

int	last_child(t_pipex pipex, int pipefd[2], char	*outfile, char *cmd)
{
	t_fd	fd;
	
	pipex.cmd_args = ft_split(cmd, ' ');
	pipex.cmd = check_permission(&pipex, pipex.cmd_args[0]);
	if (!pipex.cmd)
		child_error(&pipex);
	fd = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (fd < 0)
		return (perror(outfile), 1);
	dup2(pipefd[READ_END], STDIN);
	dup2(fd, STDOUT);
	close(pipefd[WRITE_END]);
	execve(pipex.cmd, pipex.cmd_args, pipex.args.envp);
	xexit(1);
	return (0);
}
