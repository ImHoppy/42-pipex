/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoppy <hoppy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 18:31:10 by hoppy             #+#    #+#             */
/*   Updated: 2022/02/18 19:42:04 by hoppy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Todo:
 *  - parse arguments for commands, ft_strncmp = PATH, return + 5 = PATH= and ft_split every path
 *  - get path of cmd
 *  - check permission
 *  - output all correct error
 * 
*/

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
# define READ_END 0
# define WRITE_END 1
# define STDIN 0
# define STDOUT 1
# define STDERR 2

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

char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
