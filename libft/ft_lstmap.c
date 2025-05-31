/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:58:10 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/30 20:03:47 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newelem;
	void	*newcontent;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	newlst = NULL;
	newelem = NULL;
	while (lst != NULL)
	{
		newcontent = f(lst->content);
		newelem = ft_lstnew(newcontent);
		if (newelem == NULL)
		{
			del(newcontent);
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, newelem);
		lst = lst->next;
	}
	return (newlst);
}
