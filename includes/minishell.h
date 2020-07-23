/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 10:37:57 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/23 20:52:33 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <dirent.h>
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
	char	**sort_env;
	int		len_env;
	int		i;
}					t_minishell;

t_minishell		g_shell;

/*
 ** cd.c
*/

void			ft_error_cd(struct stat info, char *buf);
void			ft_cd_home(char *buf);
void			ft_cd(char *buf);

/*
 ** echo.c
*/

char			*ft_str_add(char *s1, char const *s2);
void			ft_check_quote(char *buf, char *type_quote);
char			*ft_str_del_char(char *str, char c);
void			ft_echo(char *buf);

/*
 ** export.c
*/

void			ft_env(char *buf, char **env);
void			ft_error_export(char *buf);
void			ft_sort_env(char **env);
char			**ft_add_var(char **env, char *buf, int i);
void			ft_export(char *buf);

/*
 ** ls.c
*/

void			ft_ls(char *buf);

/*
 ** main.c
*/

int				ft_print_prompt();

/*
 ** pwd.c
*/

void			ft_pwd_error(char *buf, int error);
void			ft_pwd(char *buf);

#endif
