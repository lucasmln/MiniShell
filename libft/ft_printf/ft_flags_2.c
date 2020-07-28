/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:48:05 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:37:02 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

t_count		ft_flags(va_list aux, t_count cmp, char c, char *s)
{
	if (c == '*')
		cmp = ft_flag_star(aux, s, cmp);
	else if (c == '.')
		cmp = ft_flag_point(aux, cmp, s);
	else if (c == '0')
		cmp = ft_flag_zero(aux, cmp, s);
	else if (c == '-')
		cmp = ft_flag_moins(aux, cmp, s);
	else if (c >= '0' && c <= '9')
		cmp = ft_flag_nb(aux, cmp, s);
	return (cmp);
}

t_count		ft_switch_flag(va_list aux, t_count cmp, const char *s)
{
	cmp.space = cmp.zero;
	cmp = ft_flag_point(aux, cmp, s);
	return (cmp);
}

t_count		ft_flag_star(va_list aux, const char *s, t_count cmp)
{
	int		k;

	k = cmp.i + 2;
	if (s[k] == '.')
	{
		cmp.i = k++ - 1;
		cmp.space = va_arg(aux, int);
		cmp = ft_flag_point(aux, cmp, s);
		while (s[k] >= '0' && s[k] <= '9')
			k++;
		if (ft_convertible(s[k + 1]) == 1)
			k++;
	}
	else if (ft_convertible(s[k]) == 1)
	{
		cmp.space = va_arg(aux, int);
		cmp.i = k - 1;
		return (cmp);
	}
	return (cmp);
}
