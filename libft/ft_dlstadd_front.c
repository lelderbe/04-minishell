/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelderbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:12:15 by lelderbe          #+#    #+#             */
/*   Updated: 2021/04/16 14:23:58 by cisis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstadd_front(t_dlist **lst, t_dlist *new)
{
	if (lst && new)
	{
		if (*lst)
		{
			(*lst)->prev = new;
			new->next = *lst;
			*lst = new;
		}
		else
			*lst = new;
	}
}
