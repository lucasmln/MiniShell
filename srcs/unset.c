/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/25 18:18:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/18 14:47:03 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_error_unset(int error, char *buf)
{
	if (error == 0)
		ft_printf(1, "minishell: command not found: unset%s\n", buf);
	else if (error == 1)
		ft_printf(1, "unset: not enough arguments\n", buf);
	else if (error == 2)
		ft_printf(1, "unset: %s: invalid parameter name\n", buf);
	g_shell.ret = 1;
	return (2);
}

void		ft_unset_var(char **env, int k, int len)
{
	char	*tmp;

	while (k < len)
	{
		tmp = ft_strdup(env[k]);
		free(env[k]);
		env[k] = NULL;
		env[k] = env[k + 1];
		env[k + 1] = tmp;
		k++;
	}
	free(env[k]);
	env[k] = NULL;
}

int			ft_check_error(char *buf, int *i, int *error)
{
	*error = 0;
	*i = 0;
	while (buf[*i] && buf[*i] == ' ')
		*i += 1;
	if (*i == 0 && buf[*i])
		*error = ft_error_unset(0, buf);
	else if (!buf[*i])
		*error = ft_error_unset(1, buf);
	if (*error && g_shell.save == -1)
		return (2);
	*error = 0;
	g_shell.output = ft_strdup(&buf[*i]);
	*i = 0;
	return (0);
}

int			ft_unset_3(int *i, int *error, char *tmp)
{
	int		k;

	if (*error == 0 && g_shell.output[*i] != ' ' && g_shell.output[*i] != '\0')
	{
		*error = g_shell.output[*i + 1];
		g_shell.output[*i + 1] = '\0';
		ft_error_unset(2, g_shell.output);
		g_shell.output[*i + 1] = *error;
		*i += 1;
	}
	else if (*error != 0 && g_shell.output[*i] != ' ' &&
			g_shell.output[*i] != '\0')
		while (g_shell.output[*i] && g_shell.output[*i] != ' ')
			*i += 1;
	else if ((k = ft_find_var(g_shell.sort_env, tmp, '=')) !=
			g_shell.len_exp)
	{
		ft_unset_var(g_shell.sort_env, k, g_shell.len_exp--);
		if ((k = ft_find_var(g_shell.env, tmp, '=')) !=
				g_shell.len_env)
			ft_unset_var(g_shell.env, k, g_shell.len_env--);
	}
	else if (k == g_shell.len_exp)
		i += 1;
	return (1);
}

char		*ft_unset_2(int *i)
{
	int		k;
	char	*tmp;

	*i = 0;
	while (ft_isalnum(g_shell.output[*i]) || g_shell.output[*i] == '_')
		*i += 1;
	if (g_shell.output[*i] == '>')
	{
		k = *i;
		*i += 1;
		while (g_shell.output[*i] == ' ')
			*i += 1;
		while (g_shell.output[*i] && g_shell.output[*i] != ' ')
			*i += 1;
		g_shell.c = g_shell.output[*i];
		g_shell.output[*i] = '\0';
		g_shell.tmp = ft_strdup(&g_shell.output[k]);
		g_shell.fd = ft_check_redir(g_shell.tmp, g_shell.fd, 0);
		g_shell.output[*i] = g_shell.c;
	}
	g_shell.c = g_shell.output[*i];
	g_shell.output[*i] = '\0';
	tmp = ft_strdup(g_shell.output);
	g_shell.output[*i] = g_shell.c;
	return (tmp);
}

int			ft_free_unset(char *tmp, int *i)
{
	if (!g_shell.output[(*i = 0)] || !tmp)
	{
		free(tmp);
		tmp = NULL;
		free(g_shell.output);
		g_shell.output = NULL;
		return (0);
	}
	free(tmp);
	tmp = NULL;
	return (1);
}

int			ft_unset(char *buf)
{
	int		i;
	int		error;
	char	*tmp;

	if (ft_check_error(buf, &i, &error))
		return (2);
	while (g_shell.output[i])
	{
		while (g_shell.output[i] && g_shell.output[i] == ' ')
			i++;
		g_shell.tmp = ft_strdup(&g_shell.output[i]);
		free(g_shell.output);
		g_shell.output = g_shell.tmp;
		tmp = ft_unset_2(&i);
		if (!ft_unset_3(&i, &error, tmp))
			break ;
		g_shell.tmp = ft_strdup(&g_shell.output[i]);
		free(g_shell.output);
		g_shell.output = g_shell.tmp;
		if (!ft_free_unset(tmp, &i))
			break ;
	}
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}
