/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 16:55:02 by cisis             #+#    #+#             */
/*   Updated: 2021/05/12 18:05:18 by cisis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	parser_del(t_parser *self)
{
	size_t	i;

	i = 0;
	if (self)
	{
		free(self->string);
		self->string = NULL;
		if (self->lexer)
		{
			self->lexer->del(self->lexer);
			self->lexer = NULL;
		}
		if (self->jobs)
		{
			while (self->jobs[i])
			{
				(self->jobs[i])->del(self->jobs[i]);
				self->jobs[i] = NULL;
				i++;
			}
			self->jobs = NULL;
		}
	}
}

void	parser_refresh(t_parser *self, int exit_status)
{
	size_t	i;

	i = 0;
	if (self)
	{
		if (self->jobs)
		{
			while (i < self->jobs_len)
			{
				(self->jobs[i])->del(self->jobs[i]);
				self->jobs[i] = NULL;
				i++;
			}
			free(self->jobs);
			self->jobs = NULL;
			self->jobs_len = 0;
		}
		self->exit_status = exit_status;
	}
}

void	parser_init(t_parser *self, char *string_to_parse, int exit_status)
{
	self->string = string_to_parse;
	self->exit_status = exit_status;
	self->lexer = NULL;
	self->jobs = NULL;
	self->jobs_len = 0;
	self->pos = 0;
	self->del = parser_del;
}

int	parser_next(t_parser *self)
{
	if (!self->lexer)
	{
		self->lexer = lexer_new(self->string);
		if ((self->lexer->tokenize(self->lexer) == -1)
			|| self->lexer->check_grammar(self->lexer) == -1)
		{
			self->lexer->del(self->lexer);
			return (0);
		}
	}
	if (self->pos < self->lexer->tokens_len)
	{
		parser_make_jobs(self);
		return (1);
	}
	else
	{
		self->lexer->del(self->lexer);
		return (0);
	}
}
