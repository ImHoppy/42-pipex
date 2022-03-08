/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:10:32 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/08 16:10:48 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strdup(const char *s1)
{
	size_t	size;
	size_t	i;
	char	*temp;
	char	*s2;

	size = ft_strlen(s1);
	s2 = malloc(size * sizeof(char) + 1);
	if (!s2)
		return (NULL);
	temp = (char *)s1;
	i = 0;
	while (i < size)
	{
		s2[i] = temp[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}