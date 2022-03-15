/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:33:39 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/15 16:44:57 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*find_path(char **env)
{
	while (*env && ft_strncmp("PATH", *env, 4))
		env++;
	if (*env == NULL)
		return (NULL);
	return (*env + 5);
}

char	*get_path_cmd(t_pipex pipex, char *cmd)
{
	int		i;
	char	*tmp;
	char	*command;

	i = 0;
	while (pipex.paths && pipex.paths[i])
	{
		tmp = ft_strjoin(pipex.paths[i], "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, X_OK | R_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (ft_strdup(cmd));
}

char	*check_permission(t_pipex *pipex, char *cmd)
{
	if ((*cmd == '.' || *cmd == '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, R_OK | X_OK) == 0)
				return (ft_strdup(cmd));
			else
				child_error(pipex, cmd, ERR_PERM, 126);
		}
		else
			return (NULL);
	}
	else
		return (get_path_cmd(*pipex, cmd));
	return (NULL);
}
