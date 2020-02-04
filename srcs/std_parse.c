/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   std_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cylemair <cylemair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 15:36:56 by cylemair          #+#    #+#             */
/*   Updated: 2020/02/04 16:14:26 by cylemair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	format_stdin(t_sh *ell)
{
	t_vect	*cmds;
	char	**tab;
	char	**args;
    char    *line;
	int		i;

	i = 0;
	cmds = NULL;
	line = NULL;
    line = replace_delim((*ell).cmd, '\t', ' ');
	tab = ft_strsplit((line) ? line : (*ell).cmd, ';');
	while (tab[i])
	{
		args = ft_strsplit(tab[i], ' ');
		if (cmds != NULL)
			vect_add(&cmds, vect_new(args));
		else if (args[0])
			cmds = vect_new(args);
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
	while (ret != -2 && lst)
	{
		ret = 0;
		if ((ret = check_builtin(&(*ell), lst)) == 0)
		{
			if ((tmp = build_path((*ell), lst)))
				ret = exec_cmd((*ell), tmp, lst);
			else if (!tmp && !access((const char*)lst->arg[0], X_OK))
				ret = exec_cmd((*ell), lst->arg[0], lst);
			else
				ret = -1;
			ft_strdel(&tmp);
		}
		printf("OUT\n");
		puterror((ret == -1) ? "commande inconnue...\n" : NULL);
		lst = (ret != -2) ? lst->next : lst;
	}
	if (ret == -2 && lst->next)
		puterror("Il y a des tâches stoppées.\n");
	return (ret);
}

void		read_stdin(t_sh ell)
{
	int		built;

	built = 0;
	while (built != -2)
	{
		ft_putstr(ell.prompt);
		get_next_line(0, &(ell.cmd));
		if (ft_strcmp(ell.cmd, ""))
		{
			format_stdin(&ell);
			if (count_delim(ell.cmd, ';') > count_lst(ell.cmds))
				puterror("erreur de syntaxe\n");
			else
				built = browse_cmd(&ell);
			//free_vector(ell.cmds);
		}
		if (ell.cmd)
			ft_strdel(&(ell.cmd));
	}
}
