/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:17:49 by root              #+#    #+#             */
/*   Updated: 2025/10/10 19:16:01 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Salta las líneas vacías entre la configuración de texturas y colores
 * 			y el mapa.
 * @param info
 * @return int	0 si no hay linea, 1 si hay linea
 * 
 */

int	skip_empty_lines(t_info *info)
{
	char	*line;

	while (*info->cursor)
	{
		line = ft_strtrim(*info->cursor, " \t\n");
		if (!line)
			return (error_msg(NULL, NULL, 0));
		if (*line) //linea no vacia
		{
			free(line);
			return (1);
		}
		free(line);
		info->cursor++;
	}
	//si se nos ha acabado el *info->cursor
	return (error_msg(ERR_NO_MAP, NULL, 0));
}

/**
 * @brief
 * 
 * @param
 * @return 0 si no es inicio de mapa, 1 si solo contiene caracteres de mapa e interpretamos como inicio de mapa
 */

int	is_map_start_line(const char *line)
{
	while (*line)
	{
		if (!ft_strchr("10NSEW", *line) && !ft_isspace(*line))
			return (0); //no es inicio del mapa
		line ++;
	}
	return (1); //solo contiene caracteres de mapa
}
