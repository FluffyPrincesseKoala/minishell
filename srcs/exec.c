/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cylemair <cylemair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 02:18:38 by cylemair          #+#    #+#             */
/*   Updated: 2020/01/16 17:09:44 by cylemair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*build_path(t_sh ell)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp = NULL;
	tmp2 = NULL;
	while (ell.paths[i])
	{
		tmp2 = ft_strjoin(ell.paths[i], "/");
		tmp = (tmp2) ? ft_strjoin(tmp2, ell.cmds->arg[0]) : NULL;
		if (tmp && !access((const char*)tmp, X_OK))
			return (tmp);
		i += 1;
	}
	return (NULL);
}

int			exec_cmd(t_sh ell, char *path, t_vect *cmd)
{
	int		status;
	pid_t	cpid;

	cpid = fork();
	if (cpid < 0)
	{
		ft_putstr_fd("fork faild at ", 2);
		ft_putnbr_fd((int)cpid, 2);
		ft_putchar('\n');
		exit(-1);
	}
	else if (cpid == 0)
	{
		if (execve(path, cmd->arg, ell.env) == -1)
			exit(0);
			return (-1);
	}
	else
		wait(&status);
	return (0);
}

static void	init(t_built *fct)
{
	fct[0].f = &builtin_cd;
	fct[0].name = "cd";
	fct[1].f = &builtin_env;
	fct[1].name = "env";
	fct[2].f = &builtin_setenv;
	fct[2].name = "setenv";
	fct[3].f = &builtin_unsetenv;
	fct[3].name = "unsetenv";
	fct[4].f = &builtin_echo;
	fct[4].name = "echo";
}

int			check_builtin(t_sh *ell, t_vect *cmd)
{
	t_built	fct[5];
	int		i;

	init(fct);
	i = 0;
	while (i != 5)
	{
		if (!ft_strcmp(cmd->arg[0], "exit"))
			return (-2);
		else if (!ft_strcmp(cmd->arg[0], fct[i].name))
		{
			fct[i].f(ell, cmd);
			return (1);
		}
		i++;
	}
	return (0);
}
