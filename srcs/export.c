/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:15:40 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:16:29 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_get_var_4(int *i)
{
	char	*tmp;

	while (g_shell.output[*i] && g_shell.output[*i] != ' ')
		*i += 1;
	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
														++g_shell.len_exp, *i);
	g_shell.env = ft_add_var(g_shell.env, g_shell.output,
														++g_shell.len_env, *i);
	if (ft_find_var(g_shell.sort_env,
			g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_change_var(g_shell.sort_env, g_shell.len_exp);
	if (ft_find_var(g_shell.env,
				g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
		ft_change_var(g_shell.env, g_shell.len_env);
	else
	{
		tmp = g_shell.env[g_shell.len_env - 1];
		g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
		g_shell.env[g_shell.len_env - 2] = tmp;
	}
	return (1);
}

int			ft_get_var_3(int *i)
{
	char	*tmp;

	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
														++g_shell.len_exp, *i);
	g_shell.env = ft_add_var(g_shell.env, g_shell.output,
														++g_shell.len_env, *i);
	g_shell.env[g_shell.len_env - 1] =
					ft_str_add(g_shell.env[g_shell.len_env - 1], "=");
	g_shell.sort_env[g_shell.len_exp - 1] =
					ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
	if (ft_find_var(g_shell.sort_env,
			g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_change_var(g_shell.sort_env, g_shell.len_exp);
	if (ft_find_var(g_shell.env,
				g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
		ft_change_var(g_shell.env, g_shell.len_env);
	else
	{
		tmp = g_shell.env[g_shell.len_env - 1];
		g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
		g_shell.env[g_shell.len_env - 2] = tmp;
	}
	if (!g_shell.output[*i + 1])
		return (0);
	return (1);
}

int			ft_get_var_2(int *i)
{
	char	*tmp;

	g_shell.tmp = ft_strdup(&g_shell.output[*i]);
	ft_check_redir(g_shell.tmp, g_shell.fd);
	if (g_shell.output[*i] == '>')
	{
		*i += 1;
		g_shell.tmp = ft_strdup(&g_shell.output[*i]);
		ft_check_redir(g_shell.tmp, g_shell.fd);
		tmp = ft_del_redir(ft_strdup(g_shell.output));
		free(g_shell.output);
		g_shell.output = tmp;
	}
	return (1);
}

int			ft_get_var(int i)
{
	while (1)
	{
		if (g_shell.output[i] == '>')
			ft_get_var_2(&i);
		if (g_shell.output[i] == '=' && i == 0)
			g_shell.ret_export = ft_get_var_7(&i);
		else if (g_shell.output[i] == '=' && i > 0)
			g_shell.ret_export = ft_get_var_5(&i);
		else if (i > 0 && (!g_shell.output[i] || g_shell.output[i] == ' '))
			g_shell.ret_export = ft_get_var_6(&i);
		else if (g_shell.output[i] == ' ')
		{
			while (g_shell.output[i] == ' ')
				i++;
			g_shell.tmp = ft_strdup(&g_shell.output[i]);
			free(g_shell.output);
			g_shell.output = g_shell.tmp;
			i = 0;
		}
		else
			i++;
		if (!g_shell.output || !g_shell.ret_export)
			break ;
	}
	return (ft_sort_env(g_shell.sort_env));
}

int			ft_export(char *buf)
{
	int		i;

	ft_create_pipe();
	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!buf[i] || buf[i] == '>')
	{
		g_shell.fd = ft_check_redir(ft_strdup(&buf[i]), g_shell.fd);
		g_shell.pip == -1 ? ft_export_no_pipe() : ft_export_pipe();
		g_shell.fd = ft_close_fd(g_shell.fd);
		if (g_shell.save != -1 || g_shell.pip != -1)
			return (ft_ispipe_is_ptvirgule());
		return (1);
	}
	g_shell.output = ft_strdup(&buf[i]);
	if (!ft_isalpha(g_shell.output[(i = 0)]) && g_shell.output[i] != '_')
		return (ft_error_export(g_shell.output));
	g_shell.ret_export = 1;
	i = ft_get_var(0);
	g_shell.output ? free(g_shell.output) : 0;
	g_shell.output = NULL;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (i);
}
