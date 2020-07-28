/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exception.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:47:49 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:36:51 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

t_count			ft_check_null_str(t_count cmp, char *s)
{
	if (s[cmp.i - 1] == '0' && s[cmp.i - 2] == '%')
		return (cmp);
	if ((s[cmp.i] == 'x' || s[cmp.i] == 'X' || s[cmp.i] == 'd' ||
	s[cmp.i] == 'u') && (cmp.zero == -4294967295 || cmp.zero == 0) &&
	ft_strncmp(cmp.str, "0", ft_strlen(cmp.str)) == 0 &&
	(s[cmp.i - 1] == '.' || (s[cmp.i - 2] == '.' && s[cmp.i - 1] == '0')))
	{
		cmp.str[0] = '\0';
		cmp.zero = 0;
	}
	else if (cmp.check != 0 && cmp.zero == 0 && ft_strncmp(cmp.str, "0",
			ft_strlen(cmp.str)) == 0)
	{
		cmp.str[0] = '\0';
		cmp.zero = 0;
	}
	return (cmp);
}

t_count			ft_except_p(t_count cmp, char *s)
{
	int		ret;

	if (s[cmp.i] == 'p' && cmp.check != 0)
	{
		write(1, "0x", 2);
		ret = ft_strlcpy(cmp.str, cmp.str + 2, ft_strlen(cmp.str) - 1);
	}
	else if (s[cmp.i] == 'c' && cmp.str[0] == '\0' && cmp.space > 0)
	{
		cmp.space -= 1;
		cmp.len -= 1;
	}
	return (cmp);
}

t_count			ft_reduc_str(t_count cmp, char *s)
{
	if (s[cmp.i] == 's' && cmp.zero == 0 && cmp.check == 1)
		return (cmp);
	if (s[cmp.i] == 's' && (long)ft_strlen(cmp.str) > cmp.zero && cmp.zero !=
		-4294967295)
		cmp.str[cmp.zero] = '\0';
	return (cmp);
}
