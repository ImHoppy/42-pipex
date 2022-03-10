/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 12:19:42 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/10 12:20:37 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			*str;

	i = -1;
	str = (char *)s;
	while (str[++i])
		if (str[i] == (char)c)
			return (str + i);
	if (str[i] == (char)c)
		return (str + i);
	return (0);
}