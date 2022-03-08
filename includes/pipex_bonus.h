/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:20:12 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/08 16:47:48 by mbraets          ###   ########.fr       */
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

typedef struct s_pipexb {
	char	**cmd_args;
	char	**paths;
	char	*cmd;
}	t_pipexb;

int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif