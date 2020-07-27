/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:18 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/23 15:13:19 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
	free(g_shell.output);
	g_shell.output = NULL;
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
	int			save;
	char		type_quote[1];
	char		*path;

	i = 0;
	save = -1;
	while (buf[i] && buf[i] == ' ' && buf[i] != ';')
		i++;
	if (buf[i] == '\n')
		return (ft_cd_home(buf));
	ft_check_quote(&buf[i], type_quote);
	i = 0;
	while (g_shell.output[++i] && save == -1)
	{
	//	ft_printf(1, "i = %d, save = %d\n", i, save);
		if (g_shell.output[i] == ';')
			save = i;
		if (save == -1 && g_shell.output[i] == '\n' && !g_shell.output[i + 1])
			g_shell.output[i] = '\0';
	}
	if (type_quote[0] == S_QUOTE)
		g_shell.output = ft_str_del_char(g_shell.output, S_QUOTE);
	if (type_quote[0] == '"')
		g_shell.output = ft_str_del_char(g_shell.output, '"');
	save != -1 ? (g_shell.output[save] = '\0') : 0;
	buf = ft_strdup(g_shell.output);
	path = ft_strtrim(buf, " ");
	free(buf);
	buf = NULL;
	if (stat(path, &info) == -1 || S_ISREG(info.st_mode))
	{
		ft_error_cd(info, path);
		return ;
	}
	chdir(path);
	if (save != -1)
	{
		g_shell.output[save] = ';';
		ft_strlcpy(g_shell.output, &g_shell.output[save + 1], ft_strlen(g_shell.output));
		ft_get_cmd(g_shell.output);
	}
	free(g_shell.output);
	g_shell.output = NULL;
	free(path);
	path = NULL;
}
