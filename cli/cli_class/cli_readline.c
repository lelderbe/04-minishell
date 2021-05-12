/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 13:21:30 by cisis             #+#    #+#             */
/*   Updated: 2021/05/12 17:38:55 by cisis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cli.h"

static void	handle_up_down(t_cli *self, char *buf)
{
	if (!ft_strncmp(buf, "\e[A", 3))
	{
		if (self->hist_cur->next)
		{
			self->hist_cur = self->hist_cur->next;
			self->line = (t_line *)((t_hnode *)self->hist_cur->content)->buf;
			tputs(restore_cursor, 1, ft_putchar);
			tputs(clr_eos, 1, ft_putchar);
			write(1, self->line->str, self->line->len);
		}
	}
	else if (!ft_strncmp(buf, "\e[B", 3))
	{
		if (self->hist_cur->prev)
		{
			self->hist_cur = self->hist_cur->prev;
			self->line = (t_line *)((t_hnode *)self->hist_cur->content)->buf;
			tputs(restore_cursor, 1, ft_putchar);
			tputs(clr_eos, 1, ft_putchar);
			write(1, self->line->str, self->line->len);
		}
	}
}

static void	handle_backspace(t_cli *self)
{
	if ((self->line->pop_last(self->line)))
	{
		tputs(cursor_left, 1, ft_putchar);
		tputs(delete_character, 1, ft_putchar);
	}
}

void	cli_update_history(t_cli *self)
{
	t_hnode		*begin;
	t_hnode		*current;

	begin = (t_hnode *)self->hist->content;
	current = (t_hnode *)self->hist_cur->content;
	if (begin == current)
		begin->hist_upd(begin, begin->buf);
	else
		begin->hist_upd(begin, current->buf);
	self->line = (t_line *)((t_hnode *)self->hist->content)->_hist;
	ft_dlstiter(self->hist, hnode_buf_upd);
}

static int	cli_readbuf(t_cli *self, char *buf)
{
	int				nbytes;
	int				to_read;

	while (ft_strncmp(buf, "\n", 1))
	{
		ioctl(0, FIONREAD, &to_read);
		nbytes = read(0, buf, 100);
		if (!ft_strncmp(buf, "\e", 1))
		{
			handle_up_down(self, buf);
			if ((!ft_strncmp(buf, "\e[D", 3)) || (!ft_strncmp(buf, "\e[C", 3)))
				continue ;
		}
		else if ((!ft_strncmp(buf, "\x04", 1)) && (!self->line->str))
			return (0);
		else if (!ft_strncmp(buf, "\x7f", 1))
			handle_backspace(self);
		else
		{
			write(1, buf, nbytes);
			if ((buf[0] >= 32 && buf[0] <= 126) || (buf[0] == '\n'))
				self->line->append(self->line, buf, nbytes);
		}
	}
	return (1);
}

int 	cli_readline(t_cli *self)
{
	char			buf[101];

	cli_launch_term(self);
	ft_bzero(buf, 101);
	ft_dlstadd_front(&(self->hist), ft_dlstnew(hnode_new()));
	self->hist_cur = self->hist;
	self->line = (t_line *)((t_hnode *)self->hist_cur->content)->buf;
	if (cli_readbuf(self, buf))
	{
		cli_update_history(self);
		if (!ft_strcmp(self->line->str, ""))
		{
			self->hist_cur = self->hist;
			self->hist = self->hist->next;
			if (self->hist)
				self->hist->prev = NULL;
			ft_dlstdelone(self->hist_cur, hnode_del);
		}
		cli_stop_term(self);
		return (1);
	}
	cli_stop_term(self);
	write(1, "exit\n", 5);
	return (0);
}
