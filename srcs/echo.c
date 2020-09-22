/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:25 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 14:53:05 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_create_new(char *new, char *s1, char const *s2)
{
	int		len1;
	int		len2;

	len1 = -1;
	len2 = -1;
	while (s1[++len1])
		new[len1] = s1[len1];
	while (s2[++len2])
		new[len1 + len2] = s2[len2];
	new[len1 + len2] = '\0';
	return (new);
}

char		*ft_str_add(char *s1, char const *s2)
{
	char	*new;
	int		len1;
	int		len2;

	len1 = 0;
	len2 = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		new = ft_strdup(s1);
		ft_strdel(&s1);
		return (new);
	}
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	if (!(new = malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	new = ft_create_new(new, s1, s2);
	ft_strdel(&s1);
	s1 = new;
	return (s1);
}

char		*ft_str_del_char(char *str, char c)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			k = i;
			while (str[k] && str[k] == c)
				k++;
			str[i] = str[k];
			if (str[k] != '\0')
				str[k] = c;
		}
		i++;
	}
	return (str);
}
