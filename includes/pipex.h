/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoppy <hoppy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 18:31:10 by hoppy             #+#    #+#             */
/*   Updated: 2022/02/18 17:38:19 by hoppy            ###   ########.fr       */
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

#endif
