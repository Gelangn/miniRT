/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 20:53:49 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/20 21:54:40 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*l1;

	l1 = malloc(sizeof(t_list));
	if (!l1)
		return (NULL);
	l1->content = content;
	l1->next = NULL;
	return (l1);
}
