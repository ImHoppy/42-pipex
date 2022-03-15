/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:20:04 by hoppy             #+#    #+#             */
/*   Updated: 2022/03/15 16:59:16 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->cmd = NULL;
	pipex->cmd_args = NULL;
	pipex->a = (t_args){argc - 1, argv + 1, envp};
	pipex->paths = ft_split(find_path(envp), ':');
	pipex->pid = ft_calloc(sizeof(pid_t), (pipex->a.ac - 2 + 1));
	pipex->nbr_cmd = pipex->a.ac - 2;
}

int	middle_cmd(t_pipex pipex, int pipefd[2], int newpipefd[2])
{
	int	i;

	i = 1;
	while (i < pipex.nbr_cmd - 1)
	{
		if (pipe(newpipefd) < 0)
			return (perror("pipe"), 1);
		pipex.pid[i] = fork();
		if (pipex.pid[i] < 0)
			return (perror("pipe"), 1);
		if (pipex.pid[i] == 0)
			mid_cmd(pipex, newpipefd, pipefd, pipex.a.av[i + 1]);
		switch_fd(newpipefd, pipefd);
		i++;
	}
	return (0);
}

int	all_cmd(t_pipex pipex)
{
	int		newpipefd[2];
	int		pipefd[2];

	newpipefd[0] = -1;
	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	pipex.pid[0] = fork();
	if (pipex.pid[0] < 0)
		return (perror("pipe"), 1);
	if (pipex.pid[0] == 0)
		first_child(pipex, pipefd, pipex.a.av[0], pipex.a.av[1]);
	if (middle_cmd(pipex, pipefd, newpipefd) > 0)
		return (1);
	pipex.pid[pipex.nbr_cmd - 1] = fork();
	if (pipex.pid[pipex.nbr_cmd - 1] < 0)
		return (perror("pipe"), 1);
	if (pipex.pid[pipex.nbr_cmd - 1] == 0)
		last_child(pipex, pipefd, \
				pipex.a.av[pipex.a.ac - 1], pipex.a.av[pipex.nbr_cmd - 1 + 1]);
	close_all(newpipefd, pipefd);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		status;

	if (argc < 5)
		return (ft_error("pipex", ERR_INPUT), 1);
	pipex_init(&pipex, argc, argv, envp);
	all_cmd(pipex);
	status = wait_childs(pipex);
	free_path(&pipex);
	free(pipex.pid);
	return (((status) & 0xff00) >> 8);
}
