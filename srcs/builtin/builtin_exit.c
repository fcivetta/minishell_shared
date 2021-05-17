/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mciupek <mciupek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 16:51:35 by mciupek           #+#    #+#             */
/*   Updated: 2021/05/12 16:51:38 by mciupek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			str_isdigit(char *str)
{
	int	i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void		clear_lsts(t_list *c, t_list *env, t_list *hist)
{
	if (c)
		ft_lstclear(&c, free_t_cmd);
	if (env)
		ft_lstclear(&env, free);
	if (hist)
		ft_lstclear(&hist, free);
	term_off();
}

void		ft_exit(t_list *c, t_list *env, t_list *hist, int r)
{
	t_cmd	*cmd;
	char	*msg;

	if (c)
	{
		cmd = (t_cmd *)c->content;
		if (cmd && array_len(cmd->cmd) > 1)
		{
			if (!str_isdigit(cmd->cmd[1]))
			{
				msg = ft_strjoin("exit: ", cmd->cmd[1]);
				print_error(msg, EXIT_NUM);
				free(msg);
			}
			else if (array_len(cmd->cmd) > 2)
			{
				print_error("exit", EXIT_NARG);
				term_off();
				return ;
			}
			r = ft_atoi(cmd->cmd[1]);
		}
	}
	clear_lsts(c, env, hist);
	exit(r);
}

void		builtin_exit(t_list *c, t_list *env, t_list *hist, int r)
{
	ft_putstr_fd("exit\n", STDERROR);
	ft_exit(c, env, hist, r);
}
