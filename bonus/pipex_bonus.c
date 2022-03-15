/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:20:04 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/15 11:40:48 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void xexit(int i)
{
	printf("%d: %d\n", getpid(), i);
	exit(i);
}

static char	*find_path(char **env)
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (*env == NULL)
		return (NULL);
	return (*env + 5);
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

	if (pipex->paths)
	{
		i = 0;
		while (pipex->paths[i])
			free(pipex->paths[i++]);
		free(pipex->paths);
	}
}

void	ft_error(char *s1, char *s2)
{
	write(2, s1, ft_strlen(s1));
	write(2, ": ", 2);
	write(2, s2, ft_strlen(s2));
	write(2, "\n", 1);
}


char	*get_path_cmd(t_pipex pipex, char *cmd)
{
	int		i;
	char	*tmp;
	char	*command;

	i = 0;
	while (pipex.paths && pipex.paths[i])
	{
		tmp = ft_strjoin(pipex.paths[i],"/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, X_OK | R_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

char	*check_permission(t_pipex *pipex, char *cmd)
{
	if ((*cmd == '.' || *cmd == '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, R_OK | X_OK) == 0)
				return (ft_strdup(cmd));
			else
			{
				ft_error(cmd, ERR_PERM);
				free_cmd(pipex);
				free_path(pipex);
				xexit(1);
			}
		}
		else
			return (NULL);
	}
	else
		return (get_path_cmd(*pipex, cmd));
	return (NULL);
}


void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->cmd = NULL;
	pipex->cmd_args = NULL;
	pipex->args = (t_args){argc - 1, argv + 1, envp};
	pipex->paths = ft_split(find_path(envp), ':');
	pipex->pid = ft_calloc(sizeof(pid_t), (pipex->args.argc - 2 + 1));
}

/*
 * Remove all fork in child function and put in the while.
 * Do not return the pid.
 */

int	execute_all_cmd(t_pipex pipex, char **argv)
{
	int		i;
	int		newpipefd[2];
	int		pipefd[2];
	char	*cmd;

	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	i = 0;
	while (i < pipex.args.argc - 2)
	{
		cmd = pipex.args.argv[i + 1];
		if (i == 0)
			pipex.pid[i] = first_child(pipex, pipefd, argv[1], cmd);
		else if (i == pipex.args.argc - 3)
			pipex.pid[i] = last_child(pipex, pipefd, \
				pipex.args.argv[pipex.args.argc - 1], cmd);
		else if (i != 0 && i != pipex.args.argc - 2)
		{
			if (pipe(newpipefd) < 0)
				return (perror("pipe"), 1);
			pipex.pid[i] = execute_cmd(pipex, newpipefd, pipefd, cmd);
			close(pipefd[0]);
			close(pipefd[1]);
			pipefd[0] = newpipefd[0];
			pipefd[1] = newpipefd[1];
		}
		//free_cmd(&pipex);
		i++;
	}
	//if (pipex.args.argc - 2 > 5)
	//{
//		close(newpipefd[0]);
//		close(newpipefd[1]);
//	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	if (argc < 5)
		return (ft_error("pipex", ERR_INPUT), 1);
	pipex_init(&pipex, argc, argv, envp);
//	Function replace the while, add pipefd to pipex or function args 
//	Remove argc and argv ? And only use pipex.args
	execute_all_cmd(pipex, argv);
	i = 0;
	while (i < argc - 3)
	{
		if (pipex.pid[i] > 0)
			waitpid(pipex.pid[i], NULL, 0);
		i++;
	}
	free_path(&pipex);
	free(pipex.pid);
	return (0);
}

