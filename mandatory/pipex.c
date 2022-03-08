/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 13:11:06 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/08 16:41:20 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*find_path(char **env)
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (*env == NULL)
		return (NULL);
	return (*env + 5);
}

int	main(int argc, char *argv[], char *env[])
{
	t_pipex	pipex;
	int		status;
	int		fd;

	if (argc != 5)
		return (ft_error("pipex", ERR_INPUT), 1);
	fd = open_fd(&pipex, argc, argv);
	if (fd != -1)
		exit(fd);
	pipex.paths = ft_split(find_path(env), ':');
	pipex.pid1 = fork();
	if (pipex.pid1 == -1)
		return (perror("fork"), 1);
	if (pipex.pid1 == 0)
		first_child(pipex, argv, env);
	pipex.pid2 = fork();
	if (pipex.pid2 == -1)
		return (perror("fork"), 1);
	if (pipex.pid2 == 0)
		second_child(pipex, argv, env);
	close_fd(&pipex);
	waitpid(pipex.pid1, &status, 0);
	waitpid(pipex.pid2, &status, 0);
	free_path(&pipex);
	return (((status) & 0xff00) >> 8);
}
