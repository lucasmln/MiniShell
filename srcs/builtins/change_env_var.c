/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:38:16 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:38:17 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char		**ft_add_var(char **env, char *buf, int len_env, int i)
{
	int		k;
	char	**new;
	char	*var;

	k = 0;
	if (!(var = malloc(sizeof(char) * (i + 2))))
		return (NULL);
	if (!(new = malloc(sizeof(char *) * (len_env + 1))))
		return (NULL);
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
	int		k;

	i = 0;
	while (env[i])
	{
		k = 0;
		while (env[i][k] && env[i][k] != c)
			k++;
		c = env[i][k];
		env[i][k] = '\0';
		if (!ft_strncmp(env[i], var, ft_strlen(env[i])))
		{
			env[i][k] = c;
			return (i);
		}
		env[i][k] = c;
		i++;
	}
	return (i);
}

int			ft_change_var(char **env, int len)
{
	int		k;

	k = 0;
	while (env[k] && ft_strncmp(env[k], env[len - 1], ft_strlen(env[len - 1]) -
								ft_strlen(ft_strchr(env[len - 1], '=')) - 1))
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

int			ft_get_var_7(int *i)
{
	int		k;

	if (!g_shell.output[*i + 1] || g_shell.output[*i + 1] == ' ')
		ft_printf(1, "minishell: bad assignment\n");
	else
	{
		k = *i + 1;
		while (g_shell.output[k] && g_shell.output[k] != ' ')
			k++;
		g_shell.c = g_shell.output[k];
		g_shell.output[k] = '\0';
		ft_printf(1, "minishell: %s not found\n", &g_shell.output[*i + 1]);
		g_shell.output[k] = g_shell.c;
	}
	return (0);
}
