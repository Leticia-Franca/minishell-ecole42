/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 23:32:15 by cleticia          #+#    #+#             */
/*   Updated: 2022/03/26 17:08:39 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srlen;
	size_t	dtlen;

	i = 0;
	srlen = ft_strlen(src);
	dtlen = ft_strlen(dst);
	if (size < dtlen + 1)
	{
		return (srlen + size);
	}
	while (dtlen + 1 + i < size && src[i] != '\0')
	{
		dst[dtlen + i] = src[i];
		i++;
	}
	dst[dtlen + i] = '\0';
	return (dtlen + srlen);
}
