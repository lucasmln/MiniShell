/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:47:57 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:36:56 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ft_printf.h"

t_count		ft_flag_zero(va_list aux, t_count cmp, const char *s)
{
	int		k;

	k = cmp.i + 2;
	cmp.check = 1;
	cmp.i++;
	cmp = (s[k] == '.') ? ft_flag_point(aux, cmp, s) : cmp;
	if (s[k] == '*')
	{
		if (ft_convertible(s[k + 1]) == 1)
		{
			cmp.zero = va_arg(aux, int);
			cmp.i = k;
		}
	}
	else if ((s[k] >= '0' && s[k] <= '9') || ft_convertible(s[k]) == 1)
	{
		cmp.i = (s[k] >= '0' && s[k] <= '9') ? k : cmp.i;
		while (s[k] >= '0' && s[k] <= '9')
			k++;
		if ((ft_convertible(s[k]) == 1 && s[k]) || s[k] == '.')
			cmp.zero = ft_atoi((char *)&s[cmp.i]);
		cmp.i = k - 1;
		cmp = (s[k] == '.') ? ft_switch_flag(aux, cmp, s) : cmp;
	}
	return (cmp);
}

t_count		ft_flag_point(va_list aux, t_count cmp, const char *s)
{
	int		k;

	k = cmp.i + 2;
	cmp.check = (s[cmp.i] == '-') ? 3 : 2;
	cmp.i++;
	cmp = (s[k] == '.' && s[k - 1] == '0') ? ft_flag_point(aux, cmp, s) : cmp;
	if (s[k] == '*')
	{
		if (ft_convertible(s[k + 1]) == 1)
		{
			cmp.zero = va_arg(aux, int);
			cmp.i = k;
		}
	}
	else if ((s[k] >= '0' && s[k] <= '9') || ft_convertible(s[k]) == 1)
	{
		cmp.i = (s[k] >= '0' && s[k] <= '9') ? k : cmp.i;
		while (s[k] >= '0' && s[k] <= '9')
			k++;
		if (ft_convertible(s[k]) == 1 && s[k])
			cmp.zero = ft_atoi((char *)&s[cmp.i]);
		cmp.i = k - 1;
	}
	return (cmp);
}

t_count		ft_flag_moins(va_list aux, t_count cmp, char *s)
{
	int		k;

	k = cmp.i + 2;
	if (s[k] == '*')
	{
		cmp.i++;
		cmp = ft_flag_star(aux, (const char *)s, cmp);
		cmp.space = (cmp.space < 0) ? cmp.space : -cmp.space;
	}
	else if (s[k] == '.')
		cmp.i++;
	while (s[k] >= '0' && s[k] <= '9')
		k++;
	if (ft_convertible(s[k]) == 1)
	{
		cmp.space = -ft_atoi((char *)&s[cmp.i + 2]);
		cmp.i = k - 1;
	}
	if (s[k] == '.')
	{
		cmp.space = -ft_atoi((char*)&s[cmp.i + 2]);
		cmp.i = k - 1;
	}
	return ((cmp = s[k] == '.' ? ft_flag_point(aux, cmp, s) : cmp));
}

t_count		ft_flag_nb(va_list aux, t_count cmp, char *s)
{
	int		k;

	k = cmp.i + 1;
	if (s[((s[k + 1] == '.') ? (cmp.i = k) + 1 : k + 1)] == '.')
	{
		cmp.space = ft_atoi(&s[k]);
		cmp = ft_flag_point(aux, cmp, s);
	}
	else
	{
		while (s[k] >= '0' && s[k] <= '9')
			k++;
		if (ft_convertible(s[k]) == 1)
		{
			cmp.space = ft_atoi(&s[cmp.i + 1]);
			cmp.i = k - 1;
		}
		else if (s[k] == '.')
		{
			cmp.space = ft_atoi(&s[cmp.i + 1]);
			cmp.i = k - 1;
			cmp = ft_flag_point(aux, cmp, s);
		}
	}
	return (cmp);
}
