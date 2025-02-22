/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 22:16:15 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/25 22:00:12 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*l;
	t_list	*newl;

	l = NULL;
	if (!lst || !f)
		return (NULL);
	while (lst)
	{
		newl = ft_lstnew(f(lst->content));
		if (!newl)
		{
			ft_lstclear(&l, del);
			return (NULL);
		}
		ft_lstadd_back(&l, newl);
		lst = lst->next;
	}
	return (l);
}
