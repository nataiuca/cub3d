/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:37:05 by root              #+#    #+#             */
/*   Updated: 2025/10/09 11:56:31 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


/* ✅❌

	- El mapa tiene que ir después de las configuraciones de texturas y colores ✅
	- No puede haber líneas vacías dentro del mapa  -> ESTA IMPLEMENTACIÓN
	INTERPRETA LÍNEA VACÍA COMO FIN DE MAPA! ✅
	- EL mapa debe parsearse con los espacios que tenga dentro 
		- no sé cómo interpretar los espacios, si como chars inválidos o qué -> los interpreto como error,
		porque no acepto que un char walkable esté tocando un espacio vacío (porque se considera
		fuera del mapa) ✅
		- los espacios al inicio o final de una linea del mapa también se conservan ✅
	- El mapa tiene que estar rodeado de paredes (1) ✅
	- Solo puede tener los caracteres 0, 1, N, S, E, W ✅
	- Solo puede haber un player ✅
	- Tiene que haber un player  ✅
	- El mapa se normaliza a forma rectangular ✅
	- No puede haber ninguna línea escrita más después del mapa ✅

		1 - wall
		0 - empty space
		N, S, E, W - player start pos + orientation
*/


/**
 * @brief	
 * 			recorre map_raw
 * 				aquí hay que cambiar algo: tiene que aceptar mapas no rectangulares
 * @param mapinfo
 * @param map
 * @return 1 in success, 0 on failure
 */

int	measure_map(t_mapinfo *mapinfo, t_map *map)
{
	int		i;
	int		curr_width;

	i = 0;
	curr_width = 0;
	while (mapinfo->map_raw[i])
	{
		curr_width = (int)ft_strlen(mapinfo->map_raw[i]);
		if (curr_width > map->width)
			map->width = curr_width;
		map->height ++; //y_size es el num de rows + 1 para terminacion NULL
		i++;
	}
	return (1);
}

int	process_map_line(t_map *map, t_player *player, int row, char *line)
{
	int	i;

	/*if (check_map_open(map, row, line))
		return (error_msg(ERR_OPEN_MAP, NULL, 0));*/
	i = 0;
	while (line[i])
	{
		if (!ft_strchr("01NSEW ", line[i])) //si no es un caracter válido
			return (error_msg(ERR_CHAR_MAP, &line[i], 0));
		if (ft_strchr("NSEW", line[i])) //si hay más de un NSEW
		{
			if (player->dir != '\0')
				return (error_msg(ERR_PLAYERS, NULL, 0));
			player->dir = line[i];
			player->pos_x = i;
			player->pos_y = row;
			init_player_orientation(player);
		}
		i ++;
	}
	map->grid[row] = ft_strdup(line);
	if (!map->grid[row])
		return (error_msg(ERR_MALLOC, NULL, 0));
	return (1);
}

static int	normalize_map(t_game *game)
{
	int		i;
	int		old_len;
	char	*new_line;
	
	i = 0;
	while (i < game->map->height)
	{
		old_len = ft_strlen(game->map->grid[i]);
		if (old_len < game->map->width)
		{
			/* Crear nueva línea con espacios adicionales */
			new_line = malloc(game->map->width + 1);
			if (!new_line)
				return (error_msg(ERR_MALLOC, NULL, 0));
			ft_memset(new_line + old_len, ' ', game->map->width - old_len);
			/* Copiar contenido original */
			ft_memcpy(new_line, game->map->grid[i], old_len);
			
			/* Rellenar con espacios */
			/*j = old_len;
			while (j < game->map->width)
			{
				new_line[j] = ' ';
				j++;
			}*/
			new_line[game->map->width] = '\0';
			
			/* Reemplazar línea original */
			free(game->map->grid[i]);
			game->map->grid[i] = new_line;
		}
		i++;
	}
	return (1);
}

/**
 * @brief
 * 			Recorre caracter a caracter todas las líneas del mapa (grid).
 * 			Si es un caracter walkable (0NSEW)
 * 				- si estamos en la primera o en la última línea o en la primera o última columna
 * 					- error open map
 * 				- si el caracter de encima o de abajo es espacio, o el caracter de la izq o la derecha
 * 				es espacio
 * 					- error open map
 * 
 * @param
 * @return
 */

static int	is_map_closed(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if(ft_strchr("0NSEW", map->grid[y][x])) //si es un char walkable
			{
				if (y == 0 || y == map->height - 1 || x == 0 || x == map->width - 1)
					return(error_msg(ERR_OPEN_MAP, NULL, 0));
				if (map->grid[y - 1][x] == ' ' || map->grid[y + 1][x] == ' ' || map->grid[y][x - 1] == ' ' || map->grid[y][x + 1] == ' ')
					return(error_msg(ERR_OPEN_MAP, NULL, 0));
			}
			x ++;
		}
		y ++;
	}
	return (1);
}

/*int	check_map_open(t_map *map, int row, char *line)
{
	int	i;
	int	len;

	len = ft_strlen(line);
	i = 0;
	if (row == 0 || row == map->height - 1) //si estamos en la primera o última línea
	{
		while (line[i])
		{
			if (line[i++] != '1') //y hay algún char que no sea 1
				return (1); //mapa abierto
		}
	}
	else
	{
		if (line[0] != '1' || line[len - 1] != '1') //el resto de líneas tienen que empezar y acabar en 1
			return (1);
	}
	return (0);
}*/


int	check_trailing_rows(t_mapinfo *mapinfo, t_map *map)
{
	char	*line;

	mapinfo->cursor += map->height; //avanzar el cursor hasta final del mapa
	while (*mapinfo->cursor)
	{
		line = ft_strtrim(*mapinfo->cursor, " \t\n");
		if (!line)
			return (error_msg(ERR_MALLOC, NULL, 0));
		if (*line)
		{
			free(line);
			return (error_msg(ERR_AFTER_MAP, NULL, 0));
		}
		free(line);
		mapinfo->cursor++;
	}
	return (1);
}

int	validate_map(t_game *game, t_mapinfo *mapinfo, t_map *map)
{
	int	row;

	measure_map(mapinfo, map); //validar forma y guardar info sobre tamaño
	map->grid = calloc(map->height + 1, sizeof(char *));
	if (!map->grid)
		return (error_msg(ERR_MALLOC, NULL, 0));
	row = 0;
	while (row < map->height)
	{
		if (!process_map_line(map, game->player, row, mapinfo->map_raw[row]))
			return (0);
		row ++;
	}
	if (!normalize_map(game))
		return (0);
	if (!is_map_closed(map))
		return (0);
	if (game->player->dir == '\0')
		return (error_msg(ERR_NO_PLAYER, NULL, 0));
	return (check_trailing_rows(mapinfo, map)); //el cursor estaba donde lo ha dejado previamente count_rows, del parser no?
}
