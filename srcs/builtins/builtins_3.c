/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:38:07 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:38:08 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		g_shell.fd = ft_check_redir(g_shell.tmp, g_shell.fd);
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
