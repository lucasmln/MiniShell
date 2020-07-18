/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 10:37:57 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/18 11:10:10 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include "ft_printf.h"
#include "color.h"
#include "../libft/libft/libft.h"

# define BUF_SIZE 2048
# define S_QUOTE 39

typedef struct		s_minishell
{
	char	buf[BUF_SIZE + 1];
	char	*output;
	char	*dir;
	char	**env;
	int		i;
}					t_minishell;

t_minishell		g_shell;

#endif
