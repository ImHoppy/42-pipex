/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:20:12 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/15 11:16:55 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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

typedef struct s_arguments {
	int		argc;
	char	**argv;
	char	**envp;
}	t_args;


typedef struct s_pipex {
	char	**cmd_args;
	char	**paths;
	char	*cmd;
	t_args	args;
	pid_t	*pid;
}	t_pipex;

// Child
void	child_error(t_pipex *pipex);
int		first_child(t_pipex pipex, int pipefd[2], char *infile, char *cmd);
int		execute_cmd(t_pipex pipex, t_fd pipefd[2], t_fd oldpipefd[2], char *cmd);
int		last_child(t_pipex pipex, int pipefd[2], char	*outfile, char *cmd);

// main
void	xexit(int i);
void	free_cmd(t_pipex *pipex);
void	free_path(t_pipex *pipex);
void	ft_error(char *s1, char *s2);
char	*get_path_cmd(t_pipex pipex, char *cmd);
char	*check_permission(t_pipex *pipex, char *cmd);
void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp);
int		execute_all_cmd(t_pipex pipex, char **argv);

// Shared
char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
void	*ft_calloc(size_t count, size_t size);

#endif
