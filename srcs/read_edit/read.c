/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkonig <lkonig@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 14:12:33 by lkonig            #+#    #+#             */
/*   Updated: 2021/05/05 14:12:34 by lkonig           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		init_pos(t_pos *pos)
{
	pos->line = 0;
	pos->hist = 0;
	pos->curs = 0;
}

char		*cp_begin_str(t_pos *pos, char *line)
{
	int		i;
	char	*begin;

	i = 0;
	begin = NULL;
	begin = (char *)malloc(sizeof(char *) * (pos->curs + 1));
	if (!begin)
		return (NULL);
	ft_strlcpy(begin, line, pos->curs + 1);
	begin[pos->curs] = '\0';
	return (begin);
}

void		display_char_end(char c, t_pos *pos, char *line)
{
	ft_putchar_fd(c, STDOUT_FILENO);
	line[pos->line] = c;
	pos->line++;
	pos->curs++;
	line[pos->line] = '\0';
}

char		*display_char_mid(char c, t_pos *pos, char *line)
{
	char	*end;
	char	*begin;
	char	*tmp;
	char	mid[2];
	int		to_store;

	mid[0] = c;
	mid[1] = '\0';
	to_store = pos->line - pos->curs;
	end = ft_strdup(line + pos->curs);
	begin = cp_begin_str(pos, line);
	insert_char(c);
	tmp = ft_strjoin(begin, mid);
	line = ft_strjoin(tmp, end);
	pos->line = ft_strlen(line);
	pos->curs++;
	free(tmp);
	free(begin);
	free(end);
	return (line);
}

static char	*fill_line(char *line, t_list *hist, t_list *env)
{
	char	buf[6];
	char	*tmp;
	t_pos	pos;
	int r;

	buf[0] = '\0';
	init_pos(&pos);
	while (buf[0] != '\n')
	{
		r = read(STDIN_FILENO, buf, 5);
		buf[r] = '\0';
		if (buf[0] == 127)
			line = delete_backspace(&pos, line);
		else if (!ft_strncmp(buf, UP, 4))
		{
			free(line);
			line = NULL;
			line = access_history('u', &pos, hist);
		}
		else if (!ft_strncmp(buf, DOWN, 4))
		{
			free(line);
			line = NULL;
			line = access_history('d', &pos, hist);
		}
		else if (!ft_strncmp(buf, RIGHT, 4))
			cursorright(&pos.curs, pos.line);
		else if (!ft_strncmp(buf, LEFT, 4))
			cursorleft(&pos.curs);
		else if (!ft_strncmp(buf, CTRL_C, 1))
		{
			free(line);
			return (NULL);
		}
		else if (!ft_strncmp(buf, CTRL_D, 1))
		{
			if (line[0])
				continue ;
			free(line);
			builtin_exit(NULL, env, hist);
		}
		else if (r > 0)
		{
			if (r == 1 && buf[0] != '\n' && buf[0] != '\034')
			{
				if (pos.curs == pos.line)
					display_char_end(buf[0], &pos, line);
				else if (pos.curs < pos.line)
				{
					tmp = line;
					free(line);
					line = display_char_mid(buf[0], &pos, tmp);
					free(tmp);
				}
			}
		}
	}
	return (line);
}

char		*read_line(t_list *hist, t_list *env)
{
	char	*line;

	line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	line[0] = '\0';
	if (!line)
		printf("ERROR\n");
	line = fill_line(line, hist, env);
	write(STDOUT_FILENO, "\n", 1);
	return (line);
}
