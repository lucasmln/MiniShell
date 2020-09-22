/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 10:37:57 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:27:07 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <stdio.h>
# include <dirent.h>
# include "../libft/ft_printf/ft_printf.h"
# include "color.h"
# include "../libft/libft/libft.h"

# define BUF_SIZE 2048
# define S_QUOTE 39

typedef struct		s_minishell
{
	char	buf[BUF_SIZE + 1];
	char	*output;
	char	*dir;
	char	*tmp;
	char	*save_buf;
	char	**env;
	char	**sort_env;
	char	quote[1];
	int		quote_pos[BUF_SIZE];
	int		i_quote;
	int		len_exp;
	int		len_env;
	int		save;
	int		*fd;
	int		nb_fd;
	int		nb_input;
	int		pipe_fd[2];
	int		save_pipfd[2];
	int		pip;
	int		i;
	int		ret_export;
	char	c;
	int		ret;
}					t_minishell;

typedef struct		s_exe
{
	pid_t			pid;
	int				*in;
	char			**argv;
	struct	stat	info;
	char			*cmd;
	char			*path;
	char			*save_path;
	char			*binary;
	char			save;
	int				i;
	int				k;
	int				l;
	int				status;
	char			*try_path;
	char			*cmd_path;
	int				fd[2];
	char			*buf;
}					t_exe;


t_minishell			g_shell;

/*
 ** cd.c
*/

void				ft_error_cd(struct stat info, char *buf);
int					ft_cd_home(char *buf);
int					ft_cd(char *buf);

/*
 ** echo.c
*/

char				*ft_str_add(char *s1, char const *s2);
char				*ft_str_del_char(char *str, char c);
int					ft_echo(char *buf);

/*
 ** export.c
*/

int					ft_env(char *buf, char **env);
int					ft_get_var(int i);
char				**ft_add_var(char **env, char *buf, int len_env, int i);
void				ft_del_var(char **env, int len);
int					ft_find_var(char **env, char *var, char c);
int					ft_change_var(char **env, int len);
int					ft_export(char *buf);
int					ft_get_var_2(int *i);
int					ft_get_var_3(int *i);
int					ft_get_var_4(int *i);
int					ft_get_var_5(int *i);

/*
 ** export_2.c
*/

int					ft_error_export(char *buf);
int					ft_sort_env(char **env);
int					ft_export_no_pipe(void);
void				ft_export_pipe_2(void);
int					ft_export_pipe(void);

/*
 ** export_3.c
*/

char				**ft_add_var(char **env, char *buf, int len_env, int i);
void				ft_del_var(char **env, int len);
int					ft_find_var(char **env, char *var, char c);
int					ft_change_var(char **env, int len);
int					ft_get_var_7(int *i);

/*
 ** export_4.c
*/

int					ft_get_var_6(int *i);
int					ft_get_var_5(int *i);

/*
 ** main.c
*/

int					ft_check_parse(char *buf);
int					ft_print_prompt();
int					ft_get_cmd(char *buf);
int					*ft_init_fd_tab(int *tab, int len);
char				*ft_del_redir(char *buf);
int					ft_ispipe_is_ptvirgule();
void				ft_create_pipe(void);

/*
 ** main_utils.c
*/

void				ft_copy_env_utils(const char **env, int i);
int					ft_copy_env(const char **env);
int					*ft_init_fd_tab(int *tab, int len);
int					*ft_close_fd(int *fd);

/*
 ** pwd.c
*/

int					ft_pwd_error(char *buf, int error);
int					ft_pwd(char *buf);

/*
 ** quote.c
*/

void				ft_init_quote(int *i, int *d_quote, int *s_quote);
char				*ft_multiligne_quote(char *buf, int s_quote, int d_quote, int i);
char				*ft_check_quote(char *buf);

/*
 ** redir.c
*/

char				*ft_del_redir(char *buf);
char				**ft_open_input(char **argv, int *in, char **now);
char				**ft_check_input(char **argv, int *in);
int					ft_double_redir(char *buf, int fd, int i);
int					*ft_check_redir(char *buf, int *fd);

/*
 ** unset.c
*/

int					ft_unset(char *buf);

/*
 ** unset_utils.c
*/

int					ft_error_unset(int error, char *buf);
int					ft_check_error(char *buf, int *i, int *error);

#endif
