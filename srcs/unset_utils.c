/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:26:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:26:48 by lmoulin          ###   ########.fr       */
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
