/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:37 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/23 15:13:39 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_pwd_error(char *buf, int error)
{
	if (error == 0)
		ft_printf(1, "pwd: too many arguments\n");
	else if (error == 1)
		ft_printf(1, "minishell: command not found pwd%s\n", buf);
}

void		ft_pwd(char *buf)
{
	char	*pwd;
	int		i;
	char	quote[1];

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n' && buf[i] != ';')
	{
		ft_check_quote(buf, quote);
		if (i != 0)
			ft_pwd_error(buf, 0);
		else
			ft_pwd_error(buf, 1);
		return;
	}
	ft_check_quote(buf, quote);
	if (!(pwd = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	pwd = getcwd(pwd, BUF_SIZE);
	pwd[BUF_SIZE] = '\0';
	ft_printf(1, "%s\n", pwd);
	free(pwd);
	pwd = NULL;
	if (g_shell.output[i++] == ';')
	{
		while (g_shell.output[i] && g_shell.output[i] == ' ')
			i++;
		ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
		ft_get_cmd(g_shell.output);
		free(g_shell.output);
		g_shell.output = NULL;
	}
}
