/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 23:33:04 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 13:19:05 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*ret;

	if (!s1)
		return (0);
	if (!set)
		return (ft_strdup(s1));
	start = 0;
	while (*(s1 + start) && ft_strchr(set, *(s1 + start)))
		start++;
	end = ft_strlen(s1);
	while (*(s1 + end - 1) && (end - 1) && ft_strchr(set, *(s1 + end - 1)))
		end--;
	if (start > end)
		return (ft_strdup(""));
	ret = (char *)malloc(end - start + 1);
	if (ret == 0)
		return (0);
	ft_strlcpy(ret, s1 + start, end - start + 1);
	return (ret);
}
