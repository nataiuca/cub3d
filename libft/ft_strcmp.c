/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:52:22 by natferna          #+#    #+#             */
/*   Updated: 2025/09/22 18:53:00 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *str1, const char *str2)
{
	if (*str1 != *str2)
		return ((unsigned char)*str1 - (unsigned char)*str2);
	if (*str1 == '\0' && *str2 == '\0')
		return (0);
	return (ft_strcmp(str1 + 1, str2 + 1));
}
