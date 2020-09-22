/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 18:45:43 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 12:19:58 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_del_redir(char *buf)
{
	char	*new;
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (!(new = malloc(sizeof(char) * ft_strlen(buf) + 1)))
		return (NULL);
	while (buf[i])
		if (buf[i] == '>')
		{
			i = buf[i + 1] == '>' ? i + 2 : i + 1;
			while (buf[i] == ' ')
				i++;
			while (buf[i] && buf[i] != ' ')
				i++;
			while (buf[i] == ' ')
				i++;
		}
		else
			new[k++] = buf[i++];
	new[k] = '\0';
	free(buf);
	buf = NULL;
	return (new);
}

char		**ft_open_input(char **argv, int *in, char **now)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (argv[i])
	{
		if (argv[i][0] == '<')
		{
			if (ft_strlen(argv[i]) == 1)
			{
				if ((in[g_shell.nb_input] = open(argv[i + 1], O_RDWR)) > 0)
					g_shell.nb_input++;
			}
			i++;
			if (!argv[i])
				break ;
		}
		else
			now[k++] = ft_strdup(argv[i]);
		i++;
	}
	now[k] = NULL;
	return (now);
}

char		**ft_check_input(char **argv, int *in)
{
	int		i;
	char	**new;

	i = 0;
	while (argv[i])
		i++;
	if (!(new = malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	g_shell.nb_input = 0;
	new = ft_open_input(argv, in, new);
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		argv[i++] = NULL;
	}
	free(argv);
	argv = NULL;
	return (new);
}

int			ft_doub_redir(char *buf, int fd, int i)
{
	int		k;
	int		save[2];
	int		start;
	char	file_buf[2048];

	while (buf[++i] == ' ')
		;
	if (!buf[i])
		return (ft_printf(1, "minishell: no redirecton after >>\n") - 30);
	start = i;
	while (buf[i] && buf[i] != ' ')
		i++;
	save[0] = buf[i];
	buf[i] = '\0';
	if ((fd = open(&buf[start], O_CREAT | O_RDWR, S_IRUSR |
			S_IROTH | S_IRGRP | S_IWUSR)) == -1)
		return (-1);
	while ((k = read(fd, file_buf, BUF_SIZE)))
		;
	buf[i] = save[0];
	return (fd);
}

int			ft_redir_simp(char *buf, int fd, int i)
{
	int		start;
	int		save;

	ft_printf(1, "c = %c\n", buf[i]);
	while (buf[++i] == ' ')
		;
	start = i;
	ft_printf(1, "i = %d\n", i);
	if (!buf[i])
	{
		ft_printf(1, "minishell: parse error after >\n");
		fd = -1;
		return (-1);
	}
	while (buf[i] && buf[i] != ' ')
		i++;
	save = buf[i];
	buf[i] = '\0';
	if ((fd = open(&buf[start], O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IROTH |
												S_IRGRP | S_IWUSR, 0640)) == -1)
		return (-1);
	buf[i] = save;
	return (fd);
}

int			*ft_check_redir(char *buf, int *fd, int cmd)
{
	int		i;
	int		start[2];

	i = -1;
	start[0] = 0;
	start[1] = 0;
	cmd++;
	g_shell.nb_fd = 0;
	while (buf[++i])
	{
		while (buf[i])
		{
			start[0] = buf[i] != '>' && buf[i] != ' ' ? i : start[0];
			buf[i] == '>' && buf[i + 1] ? g_shell.nb_fd++ : 0;
			if (buf[i] == '>' && buf[i + 1] != '>')
				fd[g_shell.nb_fd - 1] = ft_redir_simp(buf, fd[g_shell.nb_fd - 1], ++i);
			else if (buf[i] == '>' && buf[i + 1] == '>')
				fd[g_shell.nb_fd - 1] = ft_doub_redir(buf, fd[g_shell.nb_fd - 1], ++i);
			i++;
		}
		if (!buf[i])
			break ;
	}
	ft_strdel(&buf);
	return (fd);
}
