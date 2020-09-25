/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_prompt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 15:25:03 by jvaquer           #+#    #+#             */
/*   Updated: 2020/09/25 15:29:07 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_print_prompt(void)
{
	int		i;
	int		ret;
	char	*buf;

	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	buf = NULL;
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET,
														&g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	if (g_shell.buf[ret - 1] == 0)
		exit(0);
	g_shell.buf[ret] = '\0';
	i = 0;
	while (g_shell.buf[i] == ' ')
		i++;
	buf = ft_str_add(buf, &g_shell.buf[i]);
	g_shell.save_pipfd[0] = 0;
	return (ft_check_parse(buf));
}
