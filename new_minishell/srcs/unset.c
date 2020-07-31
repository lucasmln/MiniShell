/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/25 18:18:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/31 14:01:02 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_unset(int error, char *buf)
{
	if (error == 0)
		ft_printf(1, "minishell: command not found: unset%s\n", buf);
	else if (error == 1)
		ft_printf(1, "unset: not enough arguments\n", buf);
	else if (error == 2)
		ft_printf(1, "unset: %s: invalid parameter name\n", buf);
}

void		ft_unset_var(char **env, int k, int len)
{
	char *tmp;

	while (k < len)
	{
		tmp = env[k];
		env[k] = env[k + 1];
		env[k + 1] = tmp;
		k++;
	}
	free(env[k]);
	env[k] = NULL;
}

void		ft_unset(char *buf)
{
	int		i;
	int		k;
	int		error;

	i = 0;
	error = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (i == 0 && buf[i])
			return (ft_error_unset(0, g_shell.output));
	else if (!buf[i])
			return (ft_error_unset(1, g_shell.output));
	ft_strlcpy(buf, &buf[i], ft_strlen(buf));
	ft_check_quote(buf);
	i = 0;
	while (g_shell.output[i])
	{
		while (g_shell.output[i] && g_shell.output[i] == ' ')
			i++;
		ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
		i = 0;
		while (ft_isalnum(g_shell.output[i]) || g_shell.output[i] == '_')
			i++;
		if (error == 0 && g_shell.output[i] != ' ' && g_shell.output[i] != '\0')
		{
			error = g_shell.output[i + 1];
			g_shell.output[i + 1] = '\0';
			ft_error_unset(2, g_shell.output);
			g_shell.output[i + 1] = error;
			while (g_shell.output[i] && g_shell.output[i] == ' ')
				i++;
		}
		else if (error != 0 && g_shell.output[i] != ' ' && g_shell.output[i] != '\0')
			while (g_shell.output[i] && g_shell.output[i] != ' ')
				i++;
		else if ((k = ft_find_var(g_shell.sort_env, g_shell.output, ' ')) != g_shell.len_exp)
		{
			ft_unset_var(g_shell.sort_env, k, g_shell.len_exp--);
			if ((k = ft_find_var(g_shell.env, g_shell.output, ' ')) != g_shell.len_env)
				ft_unset_var(g_shell.env, k, g_shell.len_env--);
		}
		else if (k == g_shell.len_exp)
			i++;
		ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
		i = 0;
		if (!g_shell.output[i])
			break ;
	}
	free(g_shell.output);
	g_shell.output = NULL;
}
