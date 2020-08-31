/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:15:40 by lmoulin           #+#    #+#             */
/*   Updated: 2020/08/31 16:47:30 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_env(char *buf, char **env)
{
	int		i;
	char	*tmp;
	int		fd;
	int		k;

	i = 0;
	fd = 1;
	tmp = NULL;
	k = 0;
	while (buf[i] == ' ')
		i++;
	if (buf [i] != ';' && buf[i] != '\0' && buf[i] != '>')
	{
		if (i == 0)
			ft_printf(1, "minishell: command not found env%s\n", buf);
		else
			ft_printf(1, "env: take no argument\n");
		return (2);
	}
	if (buf[i] == ';')
		tmp = ft_strdup(&buf[i + 1]);
	if (buf[i] == '>')
	{
		k = i;
		while (buf[++i] == ' ')
			;
		while (buf[i] && buf[i] != ' ')
			i++;
		g_shell.c = buf[i];
		buf[i] = '\0';
		g_shell.tmp = ft_strdup(&buf[k]);
		buf[i] = g_shell.c;
		fd = ft_check_redir(g_shell.tmp, fd, 0);
		while (buf[++i])
			if (buf[i] != ' ' && buf[i] != '\0')
				k = -1;
	}
	i = -1;
	while (k != -1 && env[++i])
	{
		write(fd, env[i], ft_strlen(env[i]));
		write(fd, "\n", 1);
	}
	if (k == -1)
		ft_printf(1, "env: take no argument\n");
	if (tmp)
		return (ft_get_cmd(tmp));
	return (1);
}

void		ft_error_export(char *buf)
{
	ft_printf(1, "export: not an identifier: %s\n", buf);
}

void		ft_sort_env(char **env)
{
	int		i;
	char	*tmp;

	(void)env;
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

char		**ft_add_var(char **env, char *buf, int len_env, int i)
{
	int		k;
	char	**new;
	char	*var;

	k = 0;
	if (!(var = malloc(sizeof(char) * (i + 2))))
		return NULL;
	if (!(new = malloc(sizeof(char *) * (len_env + 1))))
		return NULL;
	ft_strlcpy(var, buf, i + 1);
	new[len_env] = NULL;
	while (env[k])
	{
		new[k] = ft_strdup(env[k]);
		free(env[k]);
		env[k] = NULL;
		k++;
	}
	new[k] = var;
	free(env);
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

int			ft_find_var(char **env, char *var, char c)
{
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, ft_strlen(var) - ft_strlen(ft_strchr(var, c))))
				return (i);
		i++;
	}
	return (i);
}

int			ft_change_var(char **env, int len)
{
	int		k;

	k = 0;
	while (env[k] && ft_strncmp(env[k], env[len - 1], ft_strlen(env[len - 1]) - ft_strlen(ft_strchr(env[len - 1], '=')) - 1))
		k++;
	free(env[k]);
	env[k] = NULL;
	env[k] = ft_strdup(env[len - 1]);
	free(env[len - 1]);
	env[len - 1] = NULL;
	if (g_shell.sort_env == env)
		g_shell.len_exp--;
	else if (g_shell.env == env)
		g_shell.len_env--;
	return (1);
}

int			ft_get_var(int i)
{
	int		k;
	int		save_len[2];
	char	*tmp;
	int		fd;

	save_len[0] = g_shell.len_exp;
	save_len[1] = g_shell.len_env;
	i = 0;
	fd = 0;
	while (1)
	{
		if (g_shell.output[i] == ';')
		{
			tmp = ft_strdup(&g_shell.output[i + 1]);
			free(g_shell.output);
			ft_sort_env(g_shell.sort_env);
			return (ft_get_cmd(tmp));
		}
		if (g_shell.output[i] == '>')
		{
			k = i;
			while (g_shell.output[++i] == ' ')
				;
			while (g_shell.output[i] && g_shell.output[i] != ' ')
				i++;
			g_shell.c = g_shell.output[i];
			g_shell.output[i] = '\0';
			g_shell.tmp = ft_strdup(&g_shell.output[k]);
			ft_check_redir(g_shell.tmp, fd, 0);
			g_shell.output[i] = g_shell.c;
		}
		if (!ft_isalnum(g_shell.output[i]) && g_shell.output[i] != '_' && g_shell.output[i] != '=' && g_shell.output[i] != ' ' && g_shell.output[i] != '\0')
		{
			while (g_shell.output[i] && g_shell.output[i] != ' ')
				i++;
			k = g_shell.output[i];
			g_shell.output[i] = '\0';
			ft_printf(1, "export: not valid in this context: %s\n", g_shell.output);
			if (!k)
				break;
			g_shell.tmp = ft_strdup(&g_shell.output[i]);
			free(g_shell.output);
			g_shell.output = g_shell.tmp;
			i = 0;
		}
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
				ft_del_var(g_shell.sort_env, save_len[1]);
				ft_del_var(g_shell.env, save_len[0]);
				break;
			}
			if (g_shell.output[i + 1] == ' ' || !g_shell.output[i + 1])
			{
				g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, ++g_shell.len_exp, i);
				g_shell.env = ft_add_var(g_shell.env, g_shell.output, ++g_shell.len_env, i);
				g_shell.env[g_shell.len_env - 1] =
					ft_str_add(g_shell.env[g_shell.len_env - 1], "=");
				g_shell.sort_env[g_shell.len_exp - 1] =
					ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
				if (ft_find_var(g_shell.sort_env, g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
					ft_change_var(g_shell.sort_env, g_shell.len_exp);
				if (ft_find_var(g_shell.env, g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
						ft_change_var(g_shell.env, g_shell.len_env);
				else
				{
					tmp = g_shell.env[g_shell.len_env - 1];
					g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
					g_shell.env[g_shell.len_env - 2] = tmp;
				}
				if (!g_shell.output[i + 1])
				{
					free(g_shell.output);
					g_shell.output = NULL;
					break ;
				}
				while (g_shell.output[i] && g_shell.output[i] != ' ')
						i++;
			}
			else
			{
				while (g_shell.output[i] && g_shell.output[i] != ' ')
					i++;
				g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, ++g_shell.len_exp, i);
				g_shell.env = ft_add_var(g_shell.env, g_shell.output, ++g_shell.len_env, i);
				if (ft_find_var(g_shell.sort_env, g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
					ft_change_var(g_shell.sort_env, g_shell.len_exp);
				if (ft_find_var(g_shell.env, g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
					ft_change_var(g_shell.env, g_shell.len_env);
				else
				{
					tmp = g_shell.env[g_shell.len_env - 1];
					g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
					g_shell.env[g_shell.len_env - 2] = tmp;
				}
			}
			if (!g_shell.output[i])
			{
				free(g_shell.output);
				g_shell.output = NULL;
				break;
			}
			g_shell.tmp = ft_strdup(&g_shell.output[i]);
			free(g_shell.output);
			g_shell.output = g_shell.tmp;
			i = 0;
		}
		else if (i > 0 && (!g_shell.output[i] || g_shell.output[i] == ' '))
		{
			k = g_shell.output[i];
			g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output, ++g_shell.len_exp, i);
			g_shell.sort_env[g_shell.len_exp - 1] =
				ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
			if (ft_find_var(g_shell.sort_env, g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
				ft_del_var(g_shell.sort_env, (g_shell.len_exp -= 1));
			if (!k)
			{
				free(g_shell.output);
				g_shell.output = NULL;
				break;
			}
			g_shell.tmp = ft_strdup(&g_shell.output[i + 1]);
			free(g_shell.output);
			g_shell.output = g_shell.tmp;
			i = 0;
		}
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
		if (!g_shell.output)
			break;
	}
	ft_sort_env(g_shell.sort_env);
	return (1);
}

int			ft_export(char *buf)
{
	int		i;
	int		k;
	int		save;
	char	*tmp;
	int		fd;

	i = -1;
	save = -1;
	fd = 0;
	while (buf[++i] && save == -1)
		if (buf[i] == ';')
			save = i;
	save != -1 ? buf[save] = '\0': 0;
	i = 0;
	while (buf[i])
		i++;
	if (i > 0 && buf[i - 1] == '\n')
		buf[i - 1] = '\0';
	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!buf[i] || (buf[i] == '\n' && !buf[i + 1]) || buf[i] == ';' || buf[i] == '>')
	{
		k = -1;
		fd = ft_check_redir(ft_strdup(&buf[i]), fd, 1);
		while (g_shell.sort_env[++k])
		{
			write(fd, g_shell.sort_env[k], ft_strlen(g_shell.sort_env[k]));
			write(fd, "\n", 1);
		}
		if (save != -1)
		{
			buf[save] = ';';
			tmp = ft_strdup(&buf[save + 1]);
			return (ft_get_cmd(tmp));
		}
		return (1);
	}
	g_shell.output = ft_strdup(&buf[i]);
	if (!ft_isalpha(g_shell.output[(k = 0)]) && g_shell.output[k] != '_')
	{
		ft_error_export(g_shell.output);
		free(g_shell.output);
		g_shell.output = NULL;
		g_shell.ret = 1;
		if (save != -1)
		{
			buf[save] = ';';
			tmp = ft_strdup(&buf[save + 1]);
			return (ft_get_cmd(tmp));
		}
		return (2);
	}
	k = ft_get_var(k);
	if (save != -1)
	{
		buf[save] = ';';
		tmp = ft_strdup(&buf[save + 1]);
		return (ft_get_cmd(tmp));
	}
	return (k);
}
