/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/18 17:00:13 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
}

void		ft_cd_home(char *buf)
{
	char	*dir;
	int		i;
	int		count;

	if (!(dir = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	dir = getcwd(dir, BUF_SIZE);
	i = 0;
	count = 0;
	while (dir[i++])
		if (dir[i] == '/')
			count++;;
	while (count-- >= 2)
		chdir("..");
}

void		ft_cd(char *buf)
{
	struct stat	info;
	int			i;
	char		type_quote[1];

	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (buf[i] == '\n')
	{
		ft_cd_home(buf);
		return;
	}
	ft_check_quote(&buf[i], type_quote);
	i = 0;
	while (g_shell.output[++i])
		if (g_shell.output[i] == '\n' && !g_shell.output[i + 1])
			g_shell.output[i] = '\0';
	if (type_quote[0] == S_QUOTE)
		g_shell.output = ft_str_del_char(g_shell.output, S_QUOTE);
	if (type_quote[0] == '"')
		g_shell.output = ft_str_del_char(g_shell.output, '"');
	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
	{
		ft_error_cd(info, g_shell.output);
		return;
	}

	chdir(g_shell.output);
}
