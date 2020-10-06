/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:59:04 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/06 11:01:06 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Pour exemple : echo hey > 1 > 2 > 3
** Afin d'ecire uniquement dans le dernier fichier,
** Remplacer par ex.i = g_shell.nb_fd - 1 (l?)
** Et remplacer le while qui suit par if (g_shell.pip == -1) (l?)
*/

t_exe		ft_loop_exe(t_exe ex)
{
	ex.try_path = ft_get_path(ex.path);
	if (ex.cmd[0] == '/' || !ex.try_path)
		ex.cmd_path = ft_strdup(ex.cmd);
	else
		ex = ft_create_cmdpath(ex);
	if (!ex.cmd_path)
		return (ex);
	ft_create_pipe();
	if (!stat(ex.cmd_path, &ex.info))
	{
		ex.i = 0;
		ft_printf(1, "fd = %d, ex.i = %d\n", g_shell.nb_fd, ex.i);
		while (g_shell.pip == -1 && (ex.i < g_shell.nb_fd ||
					(g_shell.nb_fd == 0 && ex.i == 0)))
			ex = ft_exe_no_pipe(ex);
		if (g_shell.pip != -1)
			ex = ft_exe_pipe(ex);
	}
	if (!ft_check_end_exe(ex))
		return (ex);
	ex.i = 0;
	while (ex.path[ex.i] && ex.path[ex.i] != ':')
		ex.i++;
	ex.path = &ex.path[ex.i + 1];
	return (ex);
}

int			ft_ispipe_is_ptvirgule(void)
{
	char	*tmp;

	tmp = g_shell.save != -1 ? ft_strdup(&g_shell.save_buf[g_shell.save + 1]) :
								ft_strdup(&g_shell.save_buf[g_shell.pip + 1]);
	free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	return (ft_check_parse(tmp));
}

char		*ft_add_quote(char *s)
{
	char		*new;

	if (!(new = malloc(sizeof(char) * 2)))
		exit(-1000);
	new[0] = 39;
	new[1] = '\0';
	s = ft_str_add(s, new);
	new = ft_str_add(new, s);
	return (new);
}

void		ft_pass_word(char *buf, int *i, int *count, char c)
{
	*i += 1;
	while (buf[*i] && buf[*i] != c)
		*i += 1;
	*i = buf[*i] == c ? *i + 1 : *i;
	*count += 1;
}

int			ft_count_word(char *buf)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (buf[i])
	{
		if (buf[i] == '"')
			ft_pass_word(buf, &i, &count, '"');
		else if (buf[i] == 39)
			ft_pass_word(buf, &i, &count, 39);
		else if (ft_isprint(buf[i]) && buf[i] != ' ')
		{
			while (ft_isprint(buf[i]) && buf[i] != ' ')
				i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

char		*ft_create_word(char *buf, int *i, char c)
{
	int		k;
	char	t;
	int		len;
	int		check;
	char	*new;

	len = 1;
	check = c == ' ' ? 0 : 1;
	t = c == ' ' ? 0 : c;
	while (buf[*i + len])
	{
		if (check == 1 && buf[*i + len] == t)
		{
			check = 0;
			t = 0;
		}
		else if (check == 0 && (buf[*i + len] == 39 || buf[*i + len] == '"'))
		{
			check = 1;
			t = buf[*i + len];
		}
		if (check == 0 && buf[*i + len] == ' ')
			break ;
		len++;
	}
	if (!(new = malloc(sizeof(char) * (len + 1))))
		exit(-1000);
	k = 0;
	while (k < len)
	{
		new[k] = buf[*i];
		k++;
		*i += 1;
	}
	new[k] = '\0';
	return (new);
}

char		**ft_split_av(char *buf)
{
	int		ac;
	int		i;
	int		k;
	char	**av;

	ac = ft_count_word(buf);
	if (!(av = malloc(sizeof(char *) * (ac + 1))))
		exit(-1000);
	i = 0;
	k = 0;
	while (buf[i])
	{
		if (buf[i] == 39 || buf[i] == '"')
			av[k++] = ft_create_word(buf, &i, buf[i]);
		else if (ft_isprint(buf[i]) && buf[i] != ' ')
			av[k++] = ft_create_word(buf, &i, ' ');
		i = buf[i] ? i + 1 : i;
	}
	av[k] = NULL;
	return (av);
}

int			ft_len_without_quote(char *buf)
{
	int		len;
	int		i;
	char	c;

	i = -1;
	len = 0;
	while (buf[++i])
	{
		if (buf[i] == '"' || buf[i] == 39)
		{
			c = buf[i];
			i++;
			while (buf[i] && buf[i] != c)
			{
				i++;
				len++;
			}
		}
		else
			len++;
	}
	return (len);
}

char		*ft_del_quote_av0(char *buf)
{
	char	*new;
	int		i;
	int		k;
	int		c;

	i = 0;
	k = 0;
	if (!(new = malloc(sizeof(char) * (ft_len_without_quote(buf) + 1))))
		exit(-1000);
	while (buf[i])
	{
		if (buf[i] == 39 && buf[i + 1] == '$')
		{
			new[k++] = buf[i++];
			while (buf[i] && buf[i] != 39)
				new[k++] = buf[i++];
			if (!buf[i])
				break ;
			new[k++] = buf[i++];
		}
		else if (buf[i] == '"' || buf[i] == 39)
		{
			c = buf[i++];
			while (buf[i] && buf[i] != c)
				new[k++] = buf[i++];
			i++;
		}
		else
			new[k++] = buf[i++];
	}
	new[k] = '\0';
	ft_strdel(&buf);
	return (new);
}

char		*ft_change_buf(char *buf, char *av)
{
	int		i;
	char	*save;
	char	*new;

	i = 0;
	save = ft_strdup(av);
	while (buf[i] && buf[i] != ' ')
		i++;
	new = ft_str_add(av, &buf[i]);
	av = ft_strdup(save);
	ft_strdel(&buf);
	ft_strdel(&save);
	return (new);
}

int			ft_ex_2(t_exe ex)
{
	ex.argv = ft_check_input(ex.argv, ex.in);
	if (g_shell.save_pipfd[0] > 0 || g_shell.nb_input > 0)
		ft_add_input(ex.in, ex.fd);
	ex.i = -1;
	while (1)
	{
		ex = ft_loop_exe(ex);
		if (!ex.cmd_path || !ex.try_path || ex.save == -1)
			break ;
	}
	g_shell.ret = ex.save == -1 ? ex.status / 256 : 0;
	if (ex.save != -1)
	{
		g_shell.ret = 127;
		g_shell.save_pipfd[0] = 0;
		ft_printf(1, "minishell: command not found: %s\n", ex.buf);
	}
	ex = ft_free_exe(ex);
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

char		*ft_check_redir_in_av(char *buf, int *check)
{
	char	*tmp;
	int		i;

	i = 0;
	while (buf[i])
	{
		if (buf[i] == '>')
		{
			if (!buf[i + 1] || (buf[i + 1] == '>' && !buf[i + 2]))
				*check = 1;
			buf[i] = '\0';
			tmp = ft_strdup(buf);
			ft_strdel(&buf);
			return (tmp);
		}
		i++;
	}
	return (buf);
}

char		**ft_del_redir_av(char **av)
{
	int		i;
	int		k;
	int		check;
	char	**new;

	i = 0;
	while (av[i])
		i++;
	if (!(new = malloc(sizeof(char *) * (i + 1))))
		exit(-1000);
	i = 0;
	k = 0;
	check = 0;
	while (av[i])
	{
		if (!ft_strncmp(av[i], ">", ft_strlen(av[i])) ||
									!ft_strncmp(av[i], ">>", ft_strlen(av[i])))
		{
			ft_strdel(&av[i]);
			if (av[++i])
			{
				ft_strdel(&av[i]);
				i++;
			}
		}
		else if ((av[i][0] == '>' && ft_strlen(av[i]) > 1) || check == 1)
		{
			check = 0;
			ft_strdel(&av[i++]);
		}
		else
			new[k++] = ft_check_redir_in_av(av[i++], &check);
	}
	new[k] = NULL;
	return (new);
}

int			ft_exe(char *buf)
{
	t_exe			ex;
	int				k;

	ex.buf = ft_strdup(buf);
	ex.argv = ft_split_av(ex.buf);
	k = -1;
	while (ex.argv[++k])
		ex.argv[k] = ft_del_quote_av0(ex.argv[k]);
	k = -1;
	while (ex.argv[++k])
		ex.argv[k] = ft_dollars(ex.argv[k]);
	ex.buf = ft_change_buf(ex.buf, ex.argv[0]);
	ex = ft_set_fd_path(ex);
	if (!ex.in)
		return (0);
	ex.i = 0;
	while (ex.buf[ex.i] && ex.buf[ex.i] != ' ')
		ex.i++;
	ex.save = ex.buf[ex.i];
	ex.buf[ex.i] = '\0';
	if (!(ft_strncmp(ex.buf, "./", ft_strlen("./"))))
	{
		ex.cmd = ft_strdup("./");
		ex.binary = ft_strdup(&ex.buf[2]);
	}
	else
		ex.cmd = ft_strdup(ex.buf);
	ex.buf[ex.i] = ex.save;
	ex.i = ft_ex_2(ex);
	return (ex.i);
}
