#include "pipex.h"

#include <stdio.h>

char	*find_path(char **env)
{
	while (ft_strncmp("PATH", *env, 4))
		env++;
	return (*env + 5);
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
}

int	main(int argc, char *argv[], char *env[])
{
	t_pipex	pipex;
	int		status;
	int		pipe_fd[2];
	int		files[2];
	char 	*av[2] = {argv[2], NULL};
	char 	*av2[2] = {argv[3], NULL};

	if (argc < 5)
		return 1;
	printf("%s", find_path(env));
	pipex.paths = ft_split(find_path(env), ':');
	files[0] = open(argv[1], O_RDONLY);
	files[1] = open(argv[argc - 1], O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	if ((pipex.pid1 = fork()) == -1)
		return (perror("fork"), 1);
	if (pipex.pid1 == 0)
	{
		// dup2(files[0], STDIN);
		dup2(pipe_fd[WRITE_END], STDOUT);
		dup2(files[0], STDIN);
		close(files[1]);
		close(pipe_fd[READ_END]);
		execve(argv[2], av, env);
	}

	if ((pipex.pid2 = fork()) == -1)
		return (perror("fork"), 1);
	if (pipex.pid2 == 0)
	{
		// close(pipe_fd[WRITE_END]);
		dup2(pipe_fd[READ_END], STDIN);
		dup2(files[1], STDOUT);
		// close(pipe_fd[READ_END]);
		close(files[0]);
		close(pipe_fd[WRITE_END]);
		execve(argv[3], av2, env);
	}
	close(pipe_fd[WRITE_END]);
	close(pipe_fd[READ_END]);

	close(files[0]);
	close(files[1]);

	waitpid(-1, &status, 0);
	waitpid(-1, &status, 0);
	ft_free(&pipex);
	return 0;
}
