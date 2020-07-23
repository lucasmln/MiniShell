/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:15:40 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/23 21:00:01 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_env(char *buf, char **env)
{
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n' || (buf[i] == '\n' && buf[i + 1] != '\0'))
	{
		if (i == 0)
			return ((void)ft_printf(1, "minishell: command not found env%s", buf));
		ft_printf(1, "env: take no argument\n");
		return ;
	}
	i = 0;
	while (env[i])
		ft_printf(1, "%s\n", env[i++]);
}

void		ft_error_export(char *buf)
{
	ft_printf(1, "export: not an identifier: %s\n", buf);
}

void		ft_sort_env(char **env)
{
	int		i;
	char	*tmp;

	i = -1;
	while (g_shell.sort_env[++i + 1])
	{
		if (ft_strcmp(g_shell.sort_env[i], g_shell.sort_env[i + 1]) > 0)
		{
			tmp = g_shell.sort_env[i + 1];
			g_shell.sort_env[i + 1] = g_shell.sort_env[i];
			g_shell.sort_env[i] = tmp;
			i = i - 1 >= -1 ? -1 : i - 1;
		}
	}
}

char		**ft_add_var(char **env, char *buf, int i)
{
	int		k;
	char	**new;
	char	*var;

	k = 0;
	ft_printf(1, "start buf = %s\n", buf);
	if (!(var = malloc(sizeof(char) * (i + 2))))
		return NULL;
	if (!(new = malloc(sizeof(char *) * (++g_shell.len_env + 1))))
		return NULL;
	ft_strlcpy(var, buf, i + 1);
	new[g_shell.len_env] = NULL;
	while (env[k])
	{
		new[k] = ft_strdup(env[k]);
		free(env[k]);
		g_shell.sort_env[k] = NULL;
		k++;
	}
	new[k] = var;
	free(env);
	ft_printf(1, "end buf = %s\n", buf);
	return (new);
}

void		ft_del_var(char **env, int len)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (i >= len)
		{
			free(env[i]);
			env[i] = NULL;
		}
		i++;
	}
}

void		ft_get_var(int i)
{
	int		k;
	int		save_len;

	save_len = g_shell.len_env;
	while (1)
	{
		ft_printf(1, "round, c = %c\n", g_shell.output[i]);
		if (g_shell.output[i] == '=' && i == 0)
		{
			if (g_shell.output[i + 1] == ' ')
				ft_printf(1, "minishell: bad assignment\n");
			else
			{
				k = 0;
				while (g_shell.output[i + k] && g_shell.output[i + k] != ' ')
					k++;
				g_shell.output[i + k] = '\0';
				ft_printf(1, "minishell: %s not found\n", g_shell.output);
			}
			break;
		}
		else if (g_shell.output[i] == '=' && i >= 0)
		{
			if (g_shell.output[i + 1] == '=')
			{
				k = 0;
				while (g_shell.output[i + k] && g_shell.output[i + k] != ' ')
					k++;
				g_shell.output[i + k] = '\0';
				ft_printf(1, "minishell: %s not found\n", g_shell.output);
				ft_del_var(g_shell.sort_env, save_len);
				break;
			}
			if (g_shell.output[i + 1] == ' ' || !g_shell.output[i + 1])
			{
				g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, i);
				g_shell.sort_env[g_shell.len_env - 1] =
					ft_str_add(g_shell.sort_env[g_shell.len_env - 1], "=''");
				while (g_shell.output[i] && g_shell.output[i] != ' ')
					i++;
			}
			else
			{
				while (g_shell.output[i] && g_shell.output[i] != ' ')
					i++;
				g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, i);
				g_shell.env = ft_add_var(g_shell.sort_env, g_shell.output, i);
			}
			ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
			i = 0;
		}
		else if (i > 0 && (!g_shell.output[i] || g_shell.output[i] == ' '))
		{
			k = g_shell.output[i];
			g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, i);
			g_shell.sort_env[g_shell.len_env - 1] =
				ft_str_add(g_shell.sort_env[g_shell.len_env - 1], "=''");
			if (!k)
				break;
			ft_strlcpy(g_shell.output, &g_shell.output[i + 1], ft_strlen(g_shell.output));
			i = 0;
		}
		else if (g_shell.output[i] == ' ')
		{
			while (g_shell.output[i] == ' ')
				i++;
			ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
			i = 0;
		}
		else if (!g_shell.output[i])
			break;
		else
			i++;
	}
	ft_sort_env(g_shell.sort_env);
}

void		ft_export(char *buf)
{
	int		i;
	int		check;
	char	quote[1];

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!buf[i] || (buf[i] == '\n' && !buf[i + 1]))
	{
		while (g_shell.sort_env[i])
			ft_printf(1, "%s\n", g_shell.sort_env[i++]);
		return ;
	}
	ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]));
	ft_check_quote(buf, quote);
	if (!ft_isalpha(g_shell.output[(i = 0)]) && g_shell.output[i] != '_')
	{
		ft_error_export(&g_shell.output[i]);
		return ;
	}
	i = 0;
	check = 0;
	ft_get_var(i);
	ft_sort_env(g_shell.sort_env);
/*	while (g_shell.output[i])
	{
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
		{
			ft_add_var(g_shell.output, &check, i, 0);
			while (g_shell.output[i] == ' ')
				i++;
			ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
			i = -1;
		}
		i++;
	}
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
			ft_add_var(g_shell.output, &check, i, 0);
*/}
