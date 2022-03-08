/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 18:31:10 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/08 16:40:51 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
// access unlink close read write pipe dup dup2 execve exit
# include <unistd.h>
// open
# include <fcntl.h>
// malloc free
# include <stdlib.h>
// waitpid wait
# include <sys/wait.h>
// perror
# include <stdio.h>
// strerror
# include <string.h>
# include <errno.h>
# define READ_END 0
# define WRITE_END 1
# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define ERR_INPUT "Invalid number of arguments."
# define ERR_NOENT "No such file or directory"
# define ERR_CMD "Command not found"
# define ERR_PERM "Permission denied"

typedef int	t_fd;

typedef struct s_pipex {
	t_fd	infile;
	t_fd	outfile;
	t_fd	pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd_args;
	char	**paths;
	char	*cmd;
}	t_pipex;

void	first_child(t_pipex pipex, char **argv, char **env);
void	second_child(t_pipex pipex, char **argv, char **env);

void	ft_error(char *s1, char *s2);
void	free_cmd(t_pipex *pipex);
void	free_path(t_pipex *pipex);
void	close_fd(t_pipex *pipex);
int		open_fd(t_pipex *pipex, int argc, char **argv);

// Utils
char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);

#endif
