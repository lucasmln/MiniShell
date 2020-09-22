/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:32:01 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:33:25 by lmoulin          ###   ########.fr       */
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

int			ft_pwd_error(char *buf, int error)
{
	if (error == 0)
		ft_printf(1, "pwd: too many arguments\n");
	else if (error == 1)
		ft_printf(1, "minishell: command not found pwd%s\n", buf);
	g_shell.ret = 1;
	return (1);
}

int			ft_error_export(char *buf)
{
	if (buf[0] == '=' && buf[1] == ' ')
		ft_printf(1, "minishell: bad assignment\n");
	else if (buf[0] == '=' && buf[1] != ' ')
		ft_printf(1, "minishell: %s not found\n", &buf[1]);
	else
		ft_printf(1, "export: not an identifier: %s\n", buf);
	free(g_shell.output);
	g_shell.output = NULL;
	g_shell.ret = 1;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (2);
}
