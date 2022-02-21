#include "pipex.h"

#include <stdio.h>

char	*find_path(char **env)
{
	while (ft_strncmp("PATH", *env, 4))
		env++;
	return (*env + 5);
}

static char	*get_cmd(char **paths, char *cmd)
{
	char	*temp;
	char	*command;

	while (*paths)
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

void	ft_error(char *cmd, char *s)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, s, ft_strlen(s));
}

void	free_cmd(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (pipex->cmd_args[i])
	{
		free(pipex->cmd_args[i]);
		i++;
	}
	free(pipex->cmd_args);
	free(pipex->cmd);
}

void	ft_free(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (pipex->paths[i])
	{
		free(pipex->paths[i]);
		i++;
	}
	free(pipex->paths);
	// free_cmd(pipex);
}

int	main(int argc, char *argv[], char *env[])
{
	t_pipex	pipex;
	int		status;
	// int		pipe_fd[2];

	if (argc < 5)
		return 1;
	pipex.paths = ft_split(find_path(env), ':');
	pipex.infile = open(argv[1], O_RDONLY);
	pipex.outfile = open(argv[argc - 1], O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (pipe(pipex.pipefd) == -1)
		return (perror("pipe"), 1);
	if ((pipex.pid1 = fork()) == -1)
		return (perror("fork"), 1);
	if (pipex.pid1 == 0)
	{
		pipex.cmd_args = ft_split(argv[2], ' ');
		pipex.cmd = get_cmd(pipex.paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			ft_error(pipex.cmd_args[0], ERR_CMD);
			free_cmd(&pipex);
			exit(1);
		}
		// dup2(pipex.infile, STDIN);
		dup2(pipex.pipefd[WRITE_END], STDOUT);
		dup2(pipex.infile, STDIN);
		close(pipex.outfile);
		close(pipex.pipefd[READ_END]);
		execve(pipex.cmd, pipex.cmd_args, env);
		// first_child(pipex, argv, env);
	}

	if ((pipex.pid2 = fork()) == -1)
		return (perror("fork"), 1);
	if (pipex.pid2 == 0)
	{
		pipex.cmd_args = ft_split(argv[3], ' ');
		pipex.cmd = get_cmd(pipex.paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			ft_error(pipex.cmd_args[0], ERR_CMD);
			free_cmd(&pipex);
			exit(1);
		}
		dup2(pipex.pipefd[READ_END], STDIN);
		dup2(pipex.outfile, STDOUT);
		// close(pipe_fd[READ_END]);
		close(pipex.infile);
		close(pipex.pipefd[WRITE_END]);
		execve(pipex.cmd, pipex.cmd_args, env);
	}
	close(pipex.pipefd[WRITE_END]);
	close(pipex.pipefd[READ_END]);

	close(pipex.infile);
	close(pipex.outfile);

	waitpid(pipex.pid1, &status, 0);
	waitpid(pipex.pid2, &status, 0);
	ft_free(&pipex);
	
	return 0;
}
