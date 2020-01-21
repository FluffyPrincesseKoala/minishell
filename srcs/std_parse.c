/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   std_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cylemair <cylemair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 15:36:56 by cylemair          #+#    #+#             */
/*   Updated: 2020/01/21 11:32:37 by cylemair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	format_stdin(t_sh *ell)
{
	t_vect	*cmds;
	char	**tab;
	char	**args;
	int		i;

	i = 0;
	cmds = NULL;
	printf("\nbefore {%s}\n", (*ell).cmd);
	tab = ft_strsplit((*ell).cmd, ';');
	printf("after\n");
	while (tab[i])
	{
		args = ft_strsplit(tab[i], ' ');
		putab(tab);
		if (cmds != NULL)
		{
			printf("lapin\n");
			cmds = vect_add(&cmds, vect_new(args));
		}
		else
		{
			printf("lap2\n");
			cmds = vect_new(args);
		}
		free_array(args);
		i++;
	}
	get_var(&cmds, (*ell).env);
	tilt(&cmds, *ell);
	(*ell).cmds = cmds;
}

static int	browse_cmd(t_sh *ell)
{
	t_vect	*lst;
	char	*tmp;
	int		ret;

	lst = (*ell).cmds;
	ret = 0;
	while (lst && ret != -2)
	{
		ret = 0;
		if ((ret = check_builtin(&(*ell), lst)) == 0)
		{
			(*ell).paths = ft_strsplit(findenv((*ell).env, "PATH"), ':');
			if ((tmp = build_path((*ell))))
				ret = exec_cmd((*ell), tmp, lst);
			else if (!tmp)
				ret = exec_cmd((*ell), lst->arg[0], lst);
			ft_strdel(&tmp);
		}
		puterror((ret == -1) ? "commande inconnue...\n" : NULL);
		if (ret == -2)
			break;
		else
			lst = lst->next;
	}
	if (ret == -2 && lst->next)
		puterror("Il y a des tâches stoppées.\n");
	return (ret);
}

void		read_stdin(t_sh ell)
{
	int		built;

	built = 0;
	while (1 && built != -2)
	{
		ft_putstr(ell.prompt);
		get_next_line(0, &(ell.cmd));
		if (ft_strcmp(ell.cmd, ""))
		{
			format_stdin(&ell);
			built = browse_cmd(&ell);
			free_vector(ell.cmds);
		}
		if (ell.cmd)
			ft_strdel(&(ell.cmd));
	}
}
