/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:32:42 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:32:43 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ft_pass_redir_cd(char *buf, int *i, int *fd)
{
	while (buf[++*i] == ' ')
		;
	while (buf[*i] && buf[*i] != ' ')
		*i += 1;
	g_shell.c = buf[*i];
	buf[*i] = '\0';
	g_shell.tmp = ft_strdup(buf);
	buf[*i] = g_shell.c;
	ft_check_redir(g_shell.tmp, fd);
	while (buf[*i] && buf[*i] == ' ')
		*i += 1;
}

int			ft_cd_home(char *buf)
{
	int		i;
	int		fd;

	fd = 1;
	if (buf[(i = 0)] == '>')
		ft_pass_redir_cd(buf, &i, &fd);
	if (buf[i] && buf[i] != ' ')
		ft_printf(1, "cd: error string after redirection\n");
	else if (g_shell.pip != -1)
		;
	else
	{
		i = 0;
		while (ft_strncmp(g_shell.env[i], "HOME", 4))
			i++;
		if (!g_shell.env[i])
			ft_printf(1, "minishell: no HOME path\n");
		else
			chdir(&g_shell.env[i][5]);
	}
	ft_free_av(g_shell.argv_empty);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

int			ft_cd_utils(char *buf, int *i, int *redir)
{
	int		k;

	*redir = 1;
	k = *i;
	*i += 1;
	while (buf[*i] == ' ')
		*i += 1;
	while (buf[*i] && buf[*i] != ' ')
		*i += 1;
	g_shell.c = buf[*i];
	buf[*i] = '\0';
	g_shell.tmp = ft_strdup(&buf[k]);
	g_shell.fd = ft_check_redir(g_shell.tmp, g_shell.fd);
	if (g_shell.fd[0] != -2)
		buf = ft_del_redir(ft_strdup(buf));
	buf[*i] = g_shell.c;
	while (buf[*i] == ' ')
		*i += 1;
	*redir = buf[*i] ? -1 : *redir;
	buf[k] = '\0';
	return (1);
}

int			ft_cd_2(int redir, char *buf)
{
	struct stat		info;

	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
		S_ISREG(info.st_mode) ?
			ft_printf(1, "cd: not a directory: %s\n", g_shell.output) :
			ft_printf(1, "cd: no such file or directory: %s\n", g_shell.output);
	else if (redir == -1)
		ft_printf(1, "cd: error string after redirection\n");
	else if (g_shell.pip != -1)
		;
	else
		chdir(g_shell.output);
	free(buf);
	buf = NULL;
	free(g_shell.output);
	g_shell.output = NULL;
	ft_free_av(g_shell.argv_empty);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

int			ft_cd(char *buf)
{
	int			i;
	int			redir;

	i = 0;
	redir = 0;
	if (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '>')
		return (ft_pwd_error(buf, 1));
	while (buf[i] != '\0' && buf[i] == ' ' && buf[i] != '>')
		i++;
	if (!buf[i] || buf[i] == '>')
		return (ft_cd_home(&buf[i]));
	i = -1;
	g_shell.fd = ft_check_redir(ft_strdup(buf), g_shell.fd);
	if (g_shell.fd[0] != -2)
		buf = ft_del_redir(ft_strdup(buf));
	while (buf[++i] && !redir)
		if (buf[i] == '>')
			ft_cd_utils(buf, &i, &redir);
	g_shell.output = ft_strtrim(buf, " ");
	return (ft_cd_2(redir, buf));
}
