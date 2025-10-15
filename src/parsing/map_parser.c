/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:13:09 by root              #+#    #+#             */
/*   Updated: 2025/10/15 16:11:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Extracts and stores the map section from the file data.
 * 			Starting from the current position of info->cursor (after iterating
 * 			through configuration lines), this function:
 * 			- Counts the number of map lines using count_rows()
 * 			- Allocates memory for info->map_raw
 * 			- Duplicates each map line into the new array
 * 			The resulting info->map_raw will contain a NULL-terminated copy
 * 			of all valid map lines. The function stops reading when it encounters
 * 			an empty line or the end of the file
 * 
 * @param info	Pointer to the t_info structure containing the map cursor and
 * 				the map_raw string array to store the map lines
 * @result	1 on succes, 0 on allocation or read error
 */

int	parse_map(t_info *info)
{
	int		rows;
	int		i;

	rows = count_rows(info->cursor);
	info->map_raw = calloc(rows + 1, sizeof(char *));
	if (!info->map_raw)
		return (error_msg(NULL, NULL, 0));
	i = 0;
	while (i < rows)
	{
		info->map_raw[i] = ft_strdup(info->cursor[i]);
		if (!info->map_raw[i])
			return (error_msg(NULL, NULL, 0));
		i ++;
	}
	info->map_raw[rows] = NULL;
	return (1);
}
