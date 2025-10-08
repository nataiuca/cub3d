/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:17:49 by root              #+#    #+#             */
/*   Updated: 2025/10/08 16:25:33 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Salta las líneas vacías entre la configuración de texturas y colores
 * 			y el mapa.
 * @param mapinfo
 * @return int	0 si no hay linea, 1 si hay linea
 * 
 */

int	skip_empty_lines(t_mapinfo *mapinfo)
{
	char	*line;

	while (*mapinfo->cursor)
	{
		line = ft_strtrim(*mapinfo->cursor, " \t\n");
		if (!line)
			return (error_msg(ERR_MALLOC, NULL, 0));
		if (*line) //linea no vacia
		{
			free(line);
			return (1);
		}
		free(line);
		mapinfo->cursor++;
	}
	//si se nos ha acabado el *mapinfo->cursor
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