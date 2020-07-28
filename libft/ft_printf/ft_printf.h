/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 12:29:24 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:37:14 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>

typedef struct	s_count
{
	int				fd;
	int				i;
	int				len;
	int				space;
	long			zero;
	int				check;
	char			*str;
}				t_count;
/*
 **annexe.c
*/
int				ft_strlen_nb(long nb, int base);
t_count			ft_init_count(int fd, t_count init, int k);
int				ft_convertible(char c);
void			ft_strdel(char **s);
/*
 **arg.c
*/
t_count			ft_arg_int(va_list aux, t_count cmp);
t_count			ft_unsigned_int(va_list aux, t_count cmp);
t_count			ft_arg_char(va_list aux, t_count cmp);
t_count			ft_arg_str(va_list aux, t_count cmp);
t_count			ft_arg_percent(va_list au, t_count cmp);
/*
 **arg2.c
*/
t_count			ft_arg_xp(va_list aux, t_count cmp, char c);
t_count			ft_is_p(t_count cmp, long long nb);
t_count			ft_is_x(t_count cmp, long long nb, char c);
/*
 **flags.c
*/
t_count			ft_flag_zero(va_list aux, t_count cmp, const char *s);
t_count			ft_flag_point(va_list aux, t_count cmp, const char *s);
t_count			ft_flag_moins(va_list aux, t_count cmp, char *s);
t_count			ft_flag_nb(va_list aux, t_count cmp, char *s);
/*
 **flags2.c
*/
t_count			ft_flag_star(va_list aux, const char *s, t_count cmp);
t_count			ft_flags(va_list aux, t_count cmp, char c, char *s);
t_count			ft_switch_flag(va_list aux, t_count cmp, const char *s);
/*
 **ft_printf.c
*/
t_count			ft_print_front_flag(t_count cmp, int neg, char *s);
t_count			ft_print_back_flag(t_count cmp, int neg);
t_count			ft_print_arg(t_count cmp, char *s);
t_count			ft_check(va_list aux, const char *s, t_count cmp);
int				ft_printf(int fd, const char *str, ...);
/*
 **exceptiom.c
*/
t_count			ft_check_null_str(t_count cmp, char *s);
t_count			ft_except_p(t_count cmp, char *s);
t_count			ft_reduc_str(t_count cmp, char *s);

#endif
