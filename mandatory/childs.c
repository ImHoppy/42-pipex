/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:49:49 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/08 12:26:41 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_cmd(char **paths, char *cmd)
{
	char	*temp;
	char	*command;

	while (paths && *paths)
	{
		temp = ft_strjoin(*paths, "/");
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

char	*check_permission(t_pipex pipex, char *cmd)
{
	if ((*cmd == '.' || *cmd == '/'))
	{
		if (access(cmd, 0) == 0)
		{
			if (access(cmd, R_OK | X_OK) == 0)
				return (cmd);
			else
			{
				ft_error(pipex.cmd_args[0], ERR_PERM);
				free_cmd(&pipex);
				free_path(&pipex);
				exit(1);
			}
		}
		else
			return (NULL);
	}
	else
		return (get_cmd(pipex.paths, cmd));
	return (NULL);
}

int	child_free(t_pipex pipex);

void	first_child(t_pipex pipex, char **argv, char **env)
{
	pipex.cmd_args = ft_split(argv[2], ' ');
	pipex.cmd = check_permission(pipex, pipex.cmd_args[0]);
	// dprintf(pipex.outfile, "%s [%d]", pipex.cmd, access(pipex.cmd, R_OK | X_OK));
	if (!pipex.cmd)
	{
		ft_error(pipex.cmd_args[0], ERR_CMD);
		free_cmd(&pipex);
		free_path(&pipex);
		exit(127);
	}
	dup2(pipex.pipefd[WRITE_END], STDOUT);
	dup2(pipex.infile, STDIN);
	close(pipex.outfile);
	close(pipex.pipefd[READ_END]);
	execve(pipex.cmd, pipex.cmd_args, env);
}

void	second_child(t_pipex pipex, char **argv, char **env)
{
	pipex.cmd_args = ft_split(argv[3], ' ');
	if (access(pipex.cmd_args[0], R_OK | X_OK) == 0)
		pipex.cmd = pipex.cmd_args[0];
	else
		pipex.cmd = get_cmd(pipex.paths, pipex.cmd_args[0]);
	if (!pipex.cmd)
	{
		ft_error(pipex.cmd_args[0], ERR_CMD);
		free_cmd(&pipex);
		free_path(&pipex);
		exit(1);
	}
	dup2(pipex.pipefd[READ_END], STDIN);
	dup2(pipex.outfile, STDOUT);
	close(pipex.infile);
	close(pipex.pipefd[WRITE_END]);
	execve(pipex.cmd, pipex.cmd_args, env);
}
