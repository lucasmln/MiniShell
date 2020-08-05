/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 14:37:30 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/31 14:00:04 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_ls(char *buf)
{
	DIR				*dir;
	int				i;
	int				pt_virgule;
	int				k;
	int				l;
	int				o;
	int				flag;
	int				len_max;
	char			*tmp;
	struct dirent	*ent;
	struct dirent	**save;
	struct stat		info;

	i = -1;
	pt_virgule = -1;
	if (buf[ft_strlen(buf) - 1] == '\n')
		buf[ft_strlen(buf) - 1] = '\0';
	while (buf[++i] && pt_virgule == -1)
		if (buf[i] == ';')
			pt_virgule = i;
	if (pt_virgule != -1)
		buf[pt_virgule] = '\0';
	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	flag = ft_strncmp(&buf[i], "-a", ft_strlen("-a")) == 0 ? 1 : 0;
	ft_strlcpy(buf, &buf[flag == 1 ? i + 2 : i], ft_strlen(buf));
	if (!(dir = opendir(!buf[0] ? "." : buf)))
		return (1);
	i = 0;
	while ((ent = readdir(dir)))
		i++;
	closedir(dir);
	if (!(save = malloc(sizeof(struct dirent) * (i + 1))))
		return (0);
	if (!(dir = opendir(!buf[0] ? "." : buf)))
		return (0);
	i = -1;
	len_max = 0;
	while ((save[++i] = readdir(dir)))
		if (len_max < (int)ft_strlen(save[i]->d_name))
			len_max = ft_strlen(save[i]->d_name);
	save[i] = NULL;
	i = -1;
	while (save[++i + 1])
	{
		if (ft_strcmp(save[i]->d_name, save[i + 1]->d_name) > 0)
		{
			ent = save[i];
			save[i] = save[i + 1];
			save[i + 1] = ent;
			i = i - 2 >= -1 ? i - 2 : -1;
		}
	}
	k = 0;
	while (k < 3)
	{
		i = 0;
		l = 0;
		while (save[i + l])
		{
			if (flag == 1 || save[i + l]->d_name[0] != '.')
				if (i % 3 == k)
				{
					o = len_max - ft_strlen(save[i + l]->d_name);
					if (!stat(save[i + l]->d_name, &info) && S_ISREG(info.st_mode))
							ft_printf(1, RESET "%s", save[i + l]->d_name);
					else
						ft_printf(1, BOLDCYAN "%s" RESET, save[i + l]->d_name);
					while (o-- >= -2)
						ft_printf(1, " ");
				}
			if (save[i + l]->d_name[0] == '.' && flag == 0)
				l++;
			else
				i++;
		}
		ft_printf(1, "\n");
		k++;
	}
	closedir(dir);
	free(save);
	save = NULL;
	if (pt_virgule != -1)
	{
		buf[pt_virgule] = ';';
		tmp = ft_strdup(&buf[pt_virgule + 1]);
		return (ft_get_cmd(tmp));
	}
	return (1);
}
