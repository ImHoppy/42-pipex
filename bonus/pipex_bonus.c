/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:20:04 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/10 17:41:17 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*find_path(char **env)
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (*env == NULL)
		return (NULL);
	return (*env + 5);
}

int	first_child(t_pipex pipex, int pipefd[2], char *infile)
{
	t_fd	fd;
	pid_t	pid;
	char *test[2] = {"/bin/clear", NULL};

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		fd = open(infile, O_RDONLY);
		if (fd < 0)
			return(perror(infile), 1);
		dup2(pipefd[WRITE_END], STDOUT);
		dup2(fd, STDIN);
		close(pipefd[READ_END]);
		execve("/bin/clear", test, pipex.args.envp);
	}
	// waitpid(-1, NULL, 0);
	return (0);
}

int	execute_cmd(t_pipex pipex, t_fd pipefd[2], t_fd oldpipefd[2])
{
	pid_t	pid;
	char *test[3] = {"cat", "2", NULL};
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		dup2(oldpipefd[READ_END], STDIN);
		dup2(pipefd[WRITE_END], STDOUT);
		close(oldpipefd[WRITE_END]);
		close(pipefd[READ_END]);
		execve("/bin/ls", test, pipex.args.envp);

		exit(1);
	}
	return (pid);
}

int	last_child(t_pipex pipex, int pipefd[2], char	*outfile)
{
	t_fd	fd;
	pid_t	pid;
	char *test[3] = {"cat", NULL, NULL};
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		fd = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 00644);
		if (fd < 0)
			return(perror(outfile), 1);
		dup2(pipefd[READ_END], STDIN);
		dup2(fd, STDOUT);
		close(pipefd[WRITE_END]);
		execve("/bin/cat", test, pipex.args.envp);

		exit(1);
	}
	return (pid);
}

void	free_cmd(t_pipex *pipex)
{
	int	i;
	int	j;

	if (pipex->cmds)
	{
		i = 0;
		while (pipex->cmds[i])
		{
			j = 0;
			while (pipex->cmds[i]->cmd_args[j])
			{
				free(pipex->cmds[i]->cmd_args[j]);
				j++;
			}
			free(pipex->cmds[i]->cmd_args);
			free(pipex->cmds[i]->cmd);
			free(pipex->cmds[i]);
			i++;
		}
	}
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

int	check_perm(char *cmd)
{
	if (ft_strchr(cmd, '.') || ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, R_OK | X_OK) == 0)
				return (1);
			return (3);
		}
		return (2);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		pipefd[2];
	// int		newpipefd[2];
	int		i;
printf("");
	if (argc < 5)
		return (ft_error("pipex", ERR_INPUT), 1);
	pipex.args = (t_args) {argc, argv, envp};
	pipex.paths = ft_split(find_path(envp), ':');
	pipex.cmds = NULL;
	pipex.cmds = ft_calloc(sizeof(t_cmds),(argc - 3));
	i = 0;
	while (i < argc - 3)
	{
		printf("cmd %s\n",argv[i+2]);
		
		if (pipe(pipefd) < 0)
			return (perror("pipe"), 1);
		if (i == 0)
			first_child(pipex, pipefd, argv[1]);
		if (i == argc - 2)
			last_child(pipex, pipefd, argv[argc - 1]);
		pipex.cmds[i] = malloc(sizeof(t_cmds));
		pipex.cmds[i]->cmd_args = ft_split(argv[i+2], ' ');
		pipex.cmds[i]->cmd = get_path_cmd(pipex, pipex.cmds[i]->cmd_args[0]);
		pipex.cmds[i]->index = i;
		
		// if (pipe(newpipefd) < 0)
		// 	return (perror("pipe"), 1);
		// dup2(pipefd[0], newpipefd[0]);
		// dup2(pipefd[1], newpipefd[1]);
		// close(newpipefd[0]);
		// close(newpipefd[1]);
		i++;
	}
	i = 0;
	while (i < argc - 3)
	{
		printf("%s\n", pipex.cmds[i]->cmd_args[0]);
		waitpid(*pipex.cmds[i]->pid, NULL, 0);
		i++;
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(i, NULL, 0);
	free_path(&pipex);
	free_cmd(&pipex);
	free(pipex.cmds);
	return (0);
}


char *getstr(int i)
{
	switch (i)
	{
		case 0:
			return ("getcmd");
		case 1:
			return ("cmd");
		case 2:
			return ("no such file");
		case 3:
			return ("no perm");
	}
	return (NULL);
}

/*

access 0
	access R X
		ret cmd
	ret perm denied
ret get_cmd

./cat rx
./cat x
./cat r
./cat 
/cat
*/