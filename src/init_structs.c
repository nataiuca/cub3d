/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:56:19 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/09 14:52:27 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*static int	init_rays(t_cub *cub)
{
	int	i;

	cub->rays = malloc(RAY_COUNT * sizeof(t_ray));
	if (!cub->rays)
		return (error_msg(ERR_MALLOC, NULL, 0, EXIT_FAILURE));
	i = 0;
	while (i < RAY_COUNT)
	{
		cub->rays[i].dist = 0;
		cub->rays[i].hit_x = 0;
		cub->rays[i].hit_y = 0;
		cub->rays[i].cell_value = 0;
		i++;
	}
	return (0);
}*/

static void	init_player(t_player *player)
{
	player->pos_x = 0;
	player->pos_y = 0;
	player->px = 0;
	player->py = 0;
	player->dir = '\0';
	player->angle = 0;

	player->move_forward = false;
	player->move_backward = false;
	player->move_right = false;
	player->move_left = false;
	player->turn_right = false;
	player->turn_right = false;
}

static void	init_mapinfo(t_mapinfo *mapinfo)
{
	mapinfo->fd = -1;
	mapinfo->file_raw_data = NULL;
	mapinfo->cursor = NULL;
	/*mapinfo->no_path = NULL;
	mapinfo->so_path = NULL;
	mapinfo->we_path = NULL;
	mapinfo->ea_path = NULL;*/
	mapinfo->f_color_raw = NULL;
	mapinfo->c_color_raw = NULL;
	mapinfo->f_color_set = false;
	mapinfo->c_color_set = false;
	mapinfo->map_raw = NULL;
}

/*
** Función para inicializar el mapa con valores por defecto
** - Pone todo a NULL o 0
** - Colores por defecto para suelo y techo
*/
static void	init_map(t_map *map)
{
	if (!map)
		return;
		
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->ea_texture = NULL;
	map->we_texture = NULL;
	map->floor_color = 0x654321;   /* Color marrón por defecto */
	map->ceiling_color = 0x87CEEB; /* Color azul cielo por defecto */
}

/*
** Inicializa estructuras del juego
** Reserva memoria y pone valores por defecto
*/
static int	init_structs(t_game *game)
{
	game->mapinfo = malloc(sizeof(t_mapinfo));
	if (!game->mapinfo)
		return (0);
	game->map = malloc(sizeof(t_map));
	if (!game->map)
		return (0);
	game->player = malloc(sizeof(t_player));
	if (!game->player)
	{
		free(game->map);
		return (0);
	}
	game->ray = malloc(sizeof(t_ray));
	if (!game->ray)
	{
		free(game->map);
		free(game->player);
		return (0);
	}
	return (1);
}

/*
** Inicializa el juego completo
** Pone todo a NULL por seguridad antes de reservar memoria
*/
int	init_game(t_game *game)
{
	game->mlx = NULL;
	game->img = NULL;
	game->textures[0] = NULL;
	game->textures[1] = NULL;
	game->textures[2] = NULL;
	game->textures[3] = NULL;
	if (!init_structs(game))
		return (0);
	init_mapinfo(game->mapinfo);
	init_map(game->map);
	init_player(game->player);
	return (1);
}
