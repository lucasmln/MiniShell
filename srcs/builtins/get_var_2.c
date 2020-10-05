/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:38:29 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:38:32 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			ft_get_var_6(int *i)
{
	int		k;

	k = g_shell.output[*i];
	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
												++g_shell.len_exp, *i);
	g_shell.sort_env[g_shell.len_exp - 1] =
		ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
	if (ft_find_var_export(g_shell.sort_env,
	g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_del_var(g_shell.sort_env, (g_shell.len_exp -= 1));
	if (!k)
		return (0);
	g_shell.tmp = ft_strdup(&g_shell.output[*i + 1]);
	free(g_shell.output);
	g_shell.output = g_shell.tmp;
	*i = 0;
	return (1);
}

int			ft_get_var_5(int *i)
{
	int		k;

	if (g_shell.output[*i + 1 + (k = 0)] == '=')
	{
		while (g_shell.output[*i + k] && g_shell.output[*i + k] != ' ')
			k++;
		g_shell.output[*i + k] = '\0';
		ft_printf(1, "minishell: %s not found\n", &g_shell.output[*i + 2]);
		return (0);
	}
	if (g_shell.output[*i + 1] == ' ' || !g_shell.output[*i + 1])
	{
		if (!ft_get_var_3(i))
			return (0);
		while (g_shell.output[*i] && g_shell.output[*i] != ' ')
			*i += 1;
	}
	else
		ft_get_var_4(i);
	if (!g_shell.output[*i] || !(g_shell.tmp = ft_strdup(&g_shell.output[*i])))
		return (0);
	free(g_shell.output);
	g_shell.output = g_shell.tmp;
	*i = 0;
	return (1);
}
