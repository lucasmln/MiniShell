/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 10:37:57 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/02 13:39:25 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include "../libft/ft_printf/ft_printf.h"
#include "color.h"
#include "../libft/libft/libft.h"

# define BUF_SIZE 2048
# define S_QUOTE 39

typedef struct		s_minishell
{
	char	buf[BUF_SIZE + 1];
	char	*output;
	char	*dir;
	char	*tmp;
	char	**env;
	char	**sort_env;
	char	quote[1];
	int		len_exp;
	int		len_env;
	int		i;
	char	c;
	int		ret;
}					t_minishell;

t_minishell		g_shell;

/*
 ** cd.c
*/

void			ft_error_cd(struct stat info, char *buf);
int				ft_cd_home(char *buf, int save);
int				ft_cd(char *buf);

/*
 ** echo.c
*/

char			*ft_str_add(char *s1, char const *s2);
char			*ft_check_quote(char *buf);
char			*ft_str_del_char(char *str, char c);
int				ft_echo(char *buf);
int				ft_check_redir(char *buf, int fd, int cmd);

/*
 ** export.c
*/

int				ft_env(char *buf, char **env);
int				ft_get_var(int i);
void			ft_error_export(char *buf);
void			ft_sort_env(char **env);
char			**ft_add_var(char **env, char *buf, int len_env, int i);
void			ft_del_var(char **env, int len);
int				ft_find_var(char **env, char *var, char c);
int				ft_change_var(char **env, int len);
int				ft_export(char *buf);

/*
 ** ls.c
*/

int				ft_ls(char *buf);

/*
 ** main.c
*/

int				ft_print_prompt();
int				ft_get_cmd(char *buf);

/*
 ** pwd.c
*/

int				ft_pwd_error(char *buf, int error);
int				ft_pwd(char *buf);

/*
 ** unset.c
*/

int				ft_unset(char *buf);

#endif
