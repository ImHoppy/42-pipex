/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:20:04 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/09 15:21:10 by mbraets          ###   ########.fr       */
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
	char *test[2] = {"/bin/cat", NULL};

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
		execve("/bin/cat", test, pipex.args.envp);
	}
	// waitpid(-1, NULL, 0);
	return (0);
}

int	last_child(t_pipex pipex, int pipefd[2], char	*outfile)
{
	t_fd	fd;
	pid_t	pid;
	char *test[3] = {"grep", "noperm", NULL};
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
		execve("/bin/grep", test, pipex.args.envp);

		exit(1);
	}
	return (pid);
}


int	execute_cmd(t_pipex pipex, t_fd pipefd[2], t_fd oldpipefd[2])
{
	pid_t	pid;
	char *test[3] = {"ls", "2", NULL};
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

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		pipefd[2];
	int		newpipefd[2];
	int		i;

	if (argc < 5)
		return (ft_error("pipex", ERR_INPUT), 1);
	pipex.args = (t_args) {argc, argv, envp};
	pipex.paths = ft_split(find_path(envp), ':');
	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	first_child(pipex, pipefd, argv[1]);
	i = 3;
	while (i < argc - 2)
	{
		printf("fwwfw\n");
		if (pipe(newpipefd) < 0)
			return (perror("pipe"), 1);
		dup2(pipefd[0], newpipefd[0]);
		dup2(pipefd[1], newpipefd[1]);
		close(newpipefd[0]);
		close(newpipefd[1]);
		i++;
	}
	i = last_child(pipex, pipefd, argv[argc - 1]);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(i, NULL, 0);
	free_path(&pipex);
	return (0);
}
