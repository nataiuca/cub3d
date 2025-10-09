/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:21:52 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/09 12:57:34 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
Errores: 
	- No pueden faltar configuration keys ✅ 
	- Configuratio keys invalidas ✅
	- Puede haber espacios y newlines ✅
	- Puede haber espacios entre los args de los colores ✅
	- Checkear valores de los args de los colores 0-255 y solo digitos ✅
	- Qué pasa si meto cuatro valores en vez de tres? no controlado ✅
	- NO SE SI ES NECESARIO PASAR LOS RGB A INT PORK LUEGO IGUAL HAY QUE CONVERTIR A OTRA COSA
*/

/**
 * que devuelva solo la string limpia, el validator será el que 
 * convierta y valide
 */
char	*extract_color_raw(char *line)
{
	char	*trimmed;
	char	*tmp;

	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (NULL);
	tmp = ft_strreplace(trimmed, ',', ' ');
	free(trimmed);
	if (!tmp)
		return (NULL);
	return (tmp);
}

/**
 * @brief
 * 
 * @param line
 * @param dst
 * @return 1 in success, 0 on failure
 */

int	extract_texture_path(char *line, char **dst)
{
	*dst = ft_strtrim(line, " \t\n");
	if (!*dst)
		return (error_msg(ERR_MALLOC, NULL, 0));
	return (1);
}

/**
 * @brief
 * 
 * @param line
 * @param mapinfo
 * @param configs_found
 * @return 1 in success, 0 on failure
 */

int	handle_config_line(char *line, t_game *game, int configs_found)
{
	printf("DEBUG: handle_config_lines\n");
	if (ft_strncmp(line, "NO ", 3) == 0 && !game->map->no_texture)
		return (extract_texture_path(line + 3, &game->map->no_texture));
	else if (ft_strncmp(line, "SO ", 3) == 0 && !game->map->so_texture)
		return (extract_texture_path(line + 3, &game->map->so_texture));
	else if (ft_strncmp(line, "EA ", 3) == 0 && !game->map->ea_texture)
		return (extract_texture_path(line + 3, &game->map->ea_texture));
	else if (ft_strncmp(line, "WE ", 3) == 0 && !game->map->we_texture)
		return (extract_texture_path(line + 3, &game->map->we_texture));
	else if (ft_strncmp(line, "C ", 2) == 0 && !game->mapinfo->c_color_set)
	{
		game->mapinfo->c_color_raw = extract_color_raw(line + 2);
		if (!game->mapinfo->c_color_raw)
			return (error_msg(ERR_MALLOC, NULL, 0));
		game->mapinfo->c_color_set = true;
		return (1);
	}
	else if (ft_strncmp(line, "F ", 2) == 0 && !game->mapinfo->f_color_set)
	{
		game->mapinfo->f_color_raw = extract_color_raw(line + 2);
		if (!game->mapinfo->f_color_raw)
			return (error_msg(ERR_MALLOC, NULL, 0));
		game->mapinfo->f_color_set = true;
		return (1);
	}
	if (configs_found < 6 && is_map_start_line(line))
		return (error_msg(ERR_MISSING_CONF_KEYS, NULL, 0));
	return (error_msg(ERR_KEY, line, 0));
}

/**
 * @brief
 * 
 * @param mapinfo
 * @return 1 in success, 0 on failure
 */

int	parse_config(t_game *game, t_mapinfo *mapinfo)
{
	printf("DEBUG: parse_config\n");
	int		configs_found;
	char	*line;

	configs_found = 0;
	while (*mapinfo->cursor && configs_found < 6)
	{
		if (!skip_empty_lines(mapinfo))
			return (0);
		line = ft_strtrim(*mapinfo->cursor, " \t\n"); //quitar espacios y tabs al inicio y final
		if (!line)
			return (error_msg(ERR_MALLOC, NULL, 0));
		if (*line)
		{
			if(handle_config_line(line, game, configs_found)) //si no es 0, todo va biien
			    free(line);
			else //si era 0, error, salimos con 0
			{
				free(line);
				return (0);
			}
			configs_found ++;
		}
		mapinfo->cursor++;
	}
	return (1);
}