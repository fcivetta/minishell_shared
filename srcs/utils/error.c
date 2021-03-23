/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mciupek <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:09:17 by mciupek           #+#    #+#             */
/*   Updated: 2021/03/23 14:09:19 by mciupek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	g_errors[] =
{
	{ UKN_ERR, "Unknown error." },
    { RD_ERR, "Cannot read file." },
    { CMD_ERR, "Cannot execute command." },
    { PWD_ERR, "Cannot execute pwd." },
    { CD_ERR, "Cannot execute cd." }
};

static char	*get_error_msg(t_err raised)
{
	if (raised != ERRNO_TO_STR)
		return (g_errors[raised].msg);
	return ((char *)strerror(errno));
}

void		error(t_err raised)
{
	char	*msg;

	msg = get_error_msg(raised);
	printf("Error\n%d: %s\n", raised, msg);
	exit(EXIT_FAILURE);
}
