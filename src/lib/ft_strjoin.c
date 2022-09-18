/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 23:32:11 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/02 14:49:33 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	size_t	slen;
	size_t	total;

	if (!s1 || !s2)
		return (NULL);
	slen = ft_strlen(s1) + 1;
	total = ft_strlen(s2) + slen;
	new = (char *)malloc(total * sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy (new, s1, slen);
	ft_strlcat (new, s2, total);
	return (new);
}
