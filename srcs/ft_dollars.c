/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:48:17 by jvaquer           #+#    #+#             */
/*   Updated: 2020/09/25 12:49:46 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_dollars_2(int *i, char *new, char *buf, int save)
{
	int		start;
	int		pos;

	start = *i += 1;
	if (buf[*i] && (ft_isalnum(buf[*i]) || buf[*i] == '_'))
	{
		while (buf[*i] && (ft_isalnum(buf[*i]) || buf[*i] == '_'))
			*i += 1;
		save = buf[*i];
		buf[*i] = '\0';
		pos = ft_find_var(g_shell.sort_env, &buf[start], '=');
		new = ft_str_add(new, ft_strchr(g_shell.sort_env[pos], '=') ?
				ft_strchr(g_shell.sort_env[pos], '=') + 1 : "");
		buf[*i] = save;
	}
	else if (buf[*i] == '?')
	{
		g_shell.tmp = ft_itoa(g_shell.ret);
		new = ft_str_add(new, g_shell.tmp);
		free(g_shell.tmp);
		*i += 1;
	}
	else
		new = ft_str_add(new, "$");
	return (new);
}

char		*ft_dollars(char *buf)
{
	int		i;
	int		save;
	char	*new;

	i = 0;
	if (!(new = malloc(sizeof(char) * 1)))
		return (NULL);
	new[0] = '\0';
	while (buf[i])
	{
		if (buf[i] == '$')
			new = ft_dollars_2(&i, new, buf, save);
		else
		{
			save = buf[i + 1];
			buf[i + 1] = '\0';
			new = ft_str_add(new, &buf[i]);
			buf[i + 1] = save;
			i++;
		}
	}
	g_shell.tmp = NULL;
	free(buf);
	buf = NULL;
	return (new);
}