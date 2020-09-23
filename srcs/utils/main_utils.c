/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 18:06:18 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/21 18:07:47 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ft_copy_env_utils(const char **env, int i)
{
	if (!(ft_strncmp(env[i], "SHLVL=", ft_strlen("SHLVL="))))
	{
		g_shell.tmp = ft_itoa(ft_atoi(&env[i][ft_strlen("SHLVL=")]) + 1);
		g_shell.sort_env[i] = ft_str_add(ft_strdup("SHLVL="), g_shell.tmp);
		free(g_shell.tmp);
		g_shell.tmp = NULL;
	}
	else
		g_shell.sort_env[i] = ft_strdup(env[i]);
}

int			ft_copy_env(const char **env)
{
	int		i;
	int		len;

	len = 0;
	while (env[len])
		len++;
	g_shell.len_env = len;
	g_shell.len_exp = len;
	if (!(g_shell.env = malloc(sizeof(char *) * (len + 1))) ||
					!(g_shell.sort_env = malloc(sizeof(char *) * (len + 1))))
		return (0);
	i = -1;
	while (env[++i])
		ft_copy_env_utils(env, i);
	g_shell.sort_env[i] = NULL;
	i = -1;
	while (g_shell.sort_env[++i])
		g_shell.env[i] = ft_strdup(g_shell.sort_env[i]);
	g_shell.env[i] = NULL;
	ft_sort_env(g_shell.sort_env);
	return (1);
}

int			*ft_init_fd_tab(int *tab, int len)
{
	int		i;

	i = 0;
	tab[i] = 1;
	while (++i < len)
		tab[i] = -2;
	return (tab);
}

int			*ft_close_fd(int *fd)
{
	int		i;

	i = 0;
	while (i < 512)
	{
		if (fd[i] > 1)
			close(fd[i]);
		fd[i] = -2;
		i++;
	}
	fd = ft_init_fd_tab(fd, 512);
	return (fd);
}
