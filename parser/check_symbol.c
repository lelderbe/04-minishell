/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cisis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 18:08:34 by cisis             #+#    #+#             */
/*   Updated: 2021/04/30 15:12:33 by cisis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_tokensep(char c)
{
	if (c == '>'
		|| c == '<'
		|| c == '|'
		|| c == ';')
		return (1);
	return (0);
}

int	is_quotes(char c)
{
	if (c == '\''
		|| c == '\"')
		return (1);
	return (0);
}
