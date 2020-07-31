/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:47:30 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:36:40 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

t_count		ft_arg_int(va_list aux, t_count cmp)
{
	int		nb;

	nb = va_arg(aux, int);
	cmp.str = ft_itoa((int)nb);
	return (cmp);
}

t_count		ft_unsigned_int(va_list aux, t_count cmp)
{
	long	nb;

	nb = va_arg(aux, unsigned int);
	if (nb < 0)
		nb = nb + 4294967295;
	cmp.str = ft_itoa((long)nb);
	return (cmp);
}

t_count		ft_arg_char(va_list aux, t_count cmp)
{
	unsigned char	c;

	c = va_arg(aux, int);
	cmp.len = (c == 0) ? cmp.len + 1 : cmp.len;
	if ((cmp.str = malloc(sizeof(char) * 2)))
	{
		cmp.str[0] = c;
		cmp.str[1] = '\0';
		return (cmp);
	}
	cmp.str = NULL;
	return (cmp);
}

t_count		ft_arg_str(va_list aux, t_count cmp)
{
	char	*temp;
	int		i;

	temp = va_arg(aux, char*);
	if (!temp || temp == NULL)
	{
		if ((cmp.str = malloc(sizeof(char) * 7)) != NULL)
			i = ft_strlcpy(cmp.str, "(null)", 7);
		else
			cmp.str = NULL;
		return (cmp);
	}
	else
	{
		cmp.str = ft_strdup(temp);
		i = -1;
		while (temp[++i])
		{
			cmp.str[i] = temp[i];
		}
		cmp.str[i] = '\0';
	}
	return (cmp);
}

t_count		ft_arg_percent(va_list au, t_count cmp)
{
	if (!(cmp.str = malloc(sizeof(char) * 2)))
	{
		cmp.str = NULL;
		return (cmp);
	}
	cmp.str[0] = '%';
	cmp.str[1] = '\0';
	return (cmp);
}
