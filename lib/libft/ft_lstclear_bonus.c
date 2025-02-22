/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 22:13:53 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/20 22:44:58 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (lst && del)
	{
		while (*lst)
		{
			ft_lstdelone(*lst, del);
			*lst = (*lst)->next;
		}
		*lst = NULL;
	}
}
