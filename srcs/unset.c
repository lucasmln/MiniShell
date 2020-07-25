/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/25 18:18:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/25 18:46:25 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_unset(int error, char *buf)
{
	if (error == 0)
		ft_printf(1, "minishell: command not found: unset%s\n", buf);
	else if (error == 1)
		ft_printf(1, "unset: not enough arguments\n");
	else if (error == 2)
		ft_printf(1, "unset: %s: invalid parameter name\n");
}

void		ft_unset(char *buf)
{
	int		i;
	int		k;
	char	quote[1];

	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (i == 0 && buf[i])
			return (ft_error_unset(0, g_shell.output));
	else if (!buf[i])
			return (ft_error_unset(1, g_shell.output));
	ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]));
	ft_check_quote(buf, quote);
	i = 0;
	while (g_shell.output[i])
	{
		while (g_shell.output[i] && g_shell.output[i] == ' ')
			i++;
		ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(&g_shell.output[i]));
		if (!(ft_isalpha(g_shell.output[i]) || g_shell.output[i] == '_'))
			return (ft_error_unset(2, g_shell.output));
		i = 0;
		while (g_shell.output[i] && ft_isalnum(g_shell.output[i]))
			i++;
		if ((k = ft_find_var(g_shell.sort_env, g_shell.output)))
		{
		}
	}
}
