/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:48:17 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/04 16:38:00 by jvaquer          ###   ########.fr       */
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

int			ft_check_dollar(char *buf, int *i, int *check)
{
	if (buf[*i] == 39)
	{
		*check = *check == 0 ? 1 : 0;
		*i += 1;
		if (!buf[*i])
			return (0);
	}
	return (1);
}

char		*ft_dollars_utils(char *buf, int save, char *new, int *i)
{
	save = buf[*i + 1];
	buf[*i + 1] = '\0';
	new = ft_str_add(new, &buf[*i]);
	buf[*i + 1] = save;
	*i += 1;
	return (new);
}

char		*ft_dollars(char *buf)
{
	int		i;
	int		save;
	int		check;
	char	*new;

	i = 0;
	check = 0;
	if (!(new = malloc(sizeof(char) * 1)))
		exit(-1000);
	new[0] = '\0';
	while (buf[i])
	{
		if (!ft_check_dollar(buf, &i, &check))
			break ;
		if (buf[i] == '$' && check == 0)
			new = ft_dollars_2(&i, new, buf, (save = 0));
		else
			new = ft_dollars_utils(buf, save, new, &i);
	}
	g_shell.tmp = NULL;
	ft_strdel(&buf);
	return (new);
}
