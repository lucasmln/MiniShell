/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:23:49 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/25 12:41:08 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			ft_len_sans_redir(char *buf)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (buf[i])
	{
		if (buf[i] != '>')
			len++;
		i++;
	}
	return (len);
}

char		*ft_del_redir(char *buf)
{
	char	*new;
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (!(new = malloc(sizeof(char) * ft_len_sans_redir(buf) + 1)))
		return (NULL);
	while (buf[i])
		if (buf[i] == '>')
		{
			i = buf[i + 1] == '>' ? i + 2 : i + 1;
			while (buf[i] == ' ')
				i++;
			while (buf[i] && buf[i] != ' ')
				i++;
			while (buf[i] == ' ')
				i++;
		}
		else
			new[k++] = buf[i++];
	new[k] = '\0';
	free(buf);
	buf = NULL;
	return (new);
}

int			ft_input_exist(char *test, char **av, int limite)
{
	int		i;
	int		cmp;

	i = 0;
	cmp = 0;
	while (av[i] && i < limite)
	{
		if (av[i][0] == '<' && ft_strlen(av[i]) > 1)
			if (!ft_strncmp(test, &av[i][1], ft_strlen(test)))
				cmp++;
		if (!ft_strncmp(test, av[i], ft_strlen(test)))
			cmp++;
		i++;
	}
	return (cmp);
}
