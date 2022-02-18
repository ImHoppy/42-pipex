#include "pipex.h"

#include <stdio.h>

int
main(int argc, char *argv[], char *env[])
{
	pid_t	pid;
	pid_t	pid2;
	int		status;
	int		pipe_fd[2];
	int		files[2];
	char 	*av[2] = {argv[1], NULL};
	char 	*av2[2] = {argv[2], NULL};
printf("%d", O_RDWR | O_TRUNC | O_CREAT);
	if (argc < 3)
		return 1;
	files[0] = open("./infile", O_RDONLY);
	files[1] = open("./outfile", O_RDWR | O_TRUNC | O_CREAT, 00644);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	if ((pid = fork()) == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		// dup2(files[0], STDIN);
		dup2(pipe_fd[WRITE_END], STDOUT);
		dup2(files[0], STDIN);
		close(files[1]);
		close(pipe_fd[READ_END]);
		execve(argv[1], av, env);
	}

	if ((pid2 = fork()) == -1)
		return (perror("fork"), 1);
	if (pid2 == 0)
	{
		// close(pipe_fd[WRITE_END]);
		dup2(pipe_fd[READ_END], STDIN);
		dup2(files[1], STDOUT);
		// close(pipe_fd[READ_END]);
		close(files[0]);
		close(pipe_fd[WRITE_END]);
		execve(argv[2], av2, env);
	}
	close(pipe_fd[WRITE_END]);
	close(pipe_fd[READ_END]);

	close(files[0]);
	close(files[1]);
	
	// {
		waitpid(-1, &status, 0);
		waitpid(-1, &status, 0);
	// 	// wait(&status);
	// 	char	buffer[1024];
	// 	int		ret;

	// 	close(pipe_fd[WRITE_END]);
	// 	while ((ret = read(pipe_fd[READ_END], buffer, 1023)) > 0)
	// 	{
	// 		printf("%d\n", ret);
	// 		buffer[ret] = 0;
	// 		printf("%s", buffer);
	// 	}
	// 	close(pipe_fd[READ_END]);
	// }
	(void)argc;
	(void)argv;
	(void)env;
	return 0;
}
