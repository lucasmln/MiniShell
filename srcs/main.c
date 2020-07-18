/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/18 16:10:31 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
}

void		ft_cd_home(char *buf)
{
	char	*dir;
	int		i;
	int		count;

	if (!(dir = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	dir = getcwd(dir, BUF_SIZE);
	i = 0;
	count = 0;
	while (dir[i++])
		if (dir[i] == '/')
			count++;;
	while (count-- >= 2)
		chdir("..");
}

void		ft_cd(char *buf)
{
	struct stat	info;
	int			i;

	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (buf[i] == '\n')
	{
		ft_cd_home(buf);
		return;
	}
	while (buf[i] && buf[i] != '\n')
		i++;
	buf[i] = '\0';
	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (stat(&buf[i], &info) == -1 || S_ISREG(info.st_mode))
	{
		ft_error_cd(info, &buf[i]);
		return;
	}
	chdir(&buf[i]);
}

void		ft_pwd_error(char *buf, int error)
{
	if (error == 0)
		ft_printf(1, "pwd: too many arguments\n");
	else if (error == 1)
		ft_printf(1, "minishell: command not found pwd%s", buf);
}

void		ft_pwd(char *buf)
{
	char	*pwd;
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n')
	{
		if (i != 0)
			ft_pwd_error(buf, 0);
		else
			ft_pwd_error(buf, 1);
		return;
	}
	if (!(pwd = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	pwd = getcwd(pwd, BUF_SIZE);
	pwd[BUF_SIZE] = '\0';
	ft_printf(1, "%s\n", pwd);
	free(pwd);
	pwd = NULL;
}
/*
char		*ft_check_export(char *buf, char **var)
{
	int		i;
	int		save;
	int		len;

	i = 0;
	len = 0;
	while (buf[i])
	{
		while (buf[i] == ' ')
			i++;
		if (ft_isprint(buf[i]))
		{
			save = i;
			while (buf[i] && ft_isalnum(buf[i]))
				i++;
			
			if (!(var = malloc(sizeof(char) * (i - save + 1))))
				return (NULL);
			ft_strlcpy(var, &buf[save], i - save);
		}
		if (buf[i] == '=' && ft_isalpha(buf[i - 1]))
			;
	}
}

void		ft_export(char *buf, char **env)
{
	int		i;
	int		k;
	char	*var;
	char	**new;

	i = 0;
	k = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] == '\n' && buf[i + 1] == '\0')
		while (env[k])
		{
			ft_printf(1, "%s\n", env[k++]);
			return ;
		}
	ft_check_export(buf, new);

}
*/
void		ft_env(char *buf, char **env)
{
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n' || (buf[i] == '\n' && buf[i + 1] != '\0'))
	{
		if (i == 0)
			return ((void)ft_printf(1, "minishell: command not found env%s", buf));
		ft_printf(1, "env: take no argument\n");
		return ;
	}
	i = 0;
	while (env[i])
		ft_printf(1, "%s\n", env[i++]);
}

char		*ft_strtmp(char *s1, char const *s2)
{
	char	*new;
	int		len1;
	int		len2;

	len1 = 0;
	len2 = 0;
	if (!s1)
		return (NULL);
	if (!s2)
		return (s1);
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	if (!(new = malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	len1 = -1;
	len2 = 0;
	while (s1[++len1])
		new[len1] = s1[len1];
	while (s2[len2])
	{
		new[len1 + len2] = s2[len2];
		len2++;
	}
	new[len1 + len2] = '\0';
	free(s1);
	s1 = NULL;
	return (new);
}

void	ft_check_quote(char *buf, char *type_quote)
{
	int		i;
	int		ret;
	int		d_quote;
	int		s_quote;

	i = 0;
	type_quote[0] = 0;
	d_quote = 0;
	s_quote = 0;
	while (buf[i])
	{
		if (!type_quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			type_quote[0] = buf[i];
		s_quote = buf[i] == S_QUOTE && S_QUOTE == type_quote[0] ? s_quote + 1: s_quote;
		d_quote = buf[i] == '"' && '"' == type_quote[0] ? d_quote + 1: d_quote;
		i++;
	}
	if (!(g_shell.output = ft_strdup(buf)))
		return ;
	buf[0] = '\0';
	if (s_quote % 2 != 0 || d_quote % 2 != 0)
	{
		while (s_quote % 2 != 0 || d_quote % 2 != 0)
		{
			s_quote > 0 ? ft_printf(1, "quote> ") : ft_printf(1, "dquote> ");
			ret = read(0, buf, BUF_SIZE);
			buf[ret] = '\0';
			i = 0;
			while (buf[i])
			{
				s_quote = buf[i] == S_QUOTE && S_QUOTE == type_quote[0] ? s_quote + 1: s_quote;
				d_quote = buf[i] == '"' && '"' == type_quote[0] ? d_quote + 1: d_quote;
				i++;
			}
			if (!(g_shell.output = ft_strtmp(g_shell.output, buf)))
				return ;
		}
	}
}

char	*ft_strchar(char *str, char c)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	//ft_printf(1, "str =%s.", str);
	while (str[i])
	{
		if (str[i] == c)
		{
			k = i;
			while (str[k] && str[k] == c)
				k++;
			str[i] = str[k];
			if (str[k] != '\0')
				str[k] = c;
		}
		i++;
	}
	return (str);
}

void	ft_echo(char *buf)
{
	int		i;
	int		flag;
	int		quote;
	char	type_quote[1];

	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (i == 0)
	{
		ft_printf(1, "minishell: command not found %s", g_shell.buf);
		return ;
	}
	flag = ft_strncmp(&buf[i], "-n ", 3) == 0 ? 1 : 0;
	flag == 1 ? ft_strlcpy(buf, &buf[i + 3], ft_strlen(&buf[i + 3])) : ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]) + 1);
	while (buf[i] == ' ')
		i++;
	ft_check_quote(buf, type_quote);
	if (type_quote[0] == S_QUOTE)
		g_shell.output = ft_strchar(g_shell.output, S_QUOTE);
	if (type_quote[0] == '"')
		g_shell.output = ft_strchar(g_shell.output, '"');
	flag ? ft_printf(1, "%s", g_shell.output) : ft_printf(1, "%s", g_shell.output);
	free(g_shell.output);
	g_shell.output = NULL;
}

void		ft_get_signal(int code)
{
	if (code == SIGINT)
		//Pas d'exit, relancer une affiche de prompt
	if (code == SIGQUIT)
		ft_printf(1, "exit: signal code %d\n", code);
	exit(code);
}


int			main(int ac, char **av, char **env)
{
	int		ret;
	int		i;
	int		pos;

	while (ft_strncmp(g_shell.buf, "exit", ft_strlen("exit")))
	{
		signal(SIGABRT, ft_get_signal);
		signal(SIGQUIT, ft_get_signal);
		signal(SIGINT, ft_get_signal);
		signal(SIGSEGV, ft_get_signal);
		signal(SIGTERM, ft_get_signal);
		g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
		i = ft_strlen(g_shell.dir);
		while (i >= 0 && g_shell.dir[i] != '/')
			i--;
		ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, &g_shell.dir[i + 1]);
		ret = read(0, g_shell.buf, BUF_SIZE);
		g_shell.buf[ret] = '\0';
		i = 0;
		while (g_shell.buf[i] && g_shell.buf[i] == ' ')
			i++;
		if (!ft_strncmp(&g_shell.buf[i], "cd", ft_strlen("cd")))
			ft_cd(&g_shell.buf[i + 2]);
		else if (!ft_strncmp(&g_shell.buf[i], "pwd", ft_strlen("pwd")))
			ft_pwd(&g_shell.buf[i + 3]);
	//	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
	//		ft_export(&buf[i + ft_strlen("export")], env);
		else if (!ft_strncmp(&g_shell.buf[i], "env", ft_strlen("env")))
			ft_env(&g_shell.buf[i + ft_strlen("env")], g_shell.env);
		else if (!ft_strncmp(&g_shell.buf[i], "echo", ft_strlen("echo")))
			ft_echo(&g_shell.buf[i + ft_strlen("echo")]);
		else if (!ft_strncmp(g_shell.buf, "exit", ft_strlen("exit")))
			break;
		else
			ft_printf(1, "minishell: command not found %s", g_shell.buf);
		free(g_shell.dir);
		g_shell.dir = NULL;
	}
}
