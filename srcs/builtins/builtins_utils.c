/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:49:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/25 12:20:37 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ft_free_empty(void)
{
	ft_free_av(g_shell.argv_empty);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
}

void		ft_pass_input(char *buf, int *i)
{
	if (buf[*i] == '<')
	{
		*i += 1;
		while (buf[*i] == ' ')
			*i += 1;
		while (buf[*i] != ' ' && buf[*i])
			*i += 1;
		while (buf[*i] == ' ')
			*i += 1;
	}
}
