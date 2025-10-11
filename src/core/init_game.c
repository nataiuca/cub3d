/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:56:19 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/11 17:14:49 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initializes a t_info structure with default values.
 * 			Sets all pointers to NULL, file descriptor to -1, and boolean 
 * 			values to false.
 * 
 * @param info	Pointer to the t_info structure
 */

static void	init_info(t_info *info)
{
	info->fd = -1;
	info->file_raw_data = NULL;
	info->cursor = NULL;
	info->f_color_raw = NULL;
	info->c_color_raw = NULL;
	info->f_color_set = false;
	info->c_color_set = false;
	info->map_raw = NULL;
}

/**
 * @brief	Initializes the t_map structure with default values.
 * 			Sets texture_paths, colors, dimensions and map grid to zero or 
 * 			NULL.
 * 
 * @param map	Pointer to the t_map structure.
 */
static void	init_map(t_map *map)
{
	if (!map)
		return ;
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->ea_texture = NULL;
	map->we_texture = NULL;
	map->ceiling_color = 0;
	map->floor_color = 0;
}

/**
 * @brief	Initializes a t_player structure with default values.
 * 			Sets initial position, direction, and movement flags to neutral
 * 			values. Default movement and rotation speeds are assigned here.
 * 
 * 			⚠️ AQUÍ NO SÉ SI DEBERÍAMOS INICIALIZAR A NÚMEROS NEGATIVOS (-1) PARA QUE SI SE ASIGNA MAL EN ALGÚN LADO SALTE ERROR
 * 			TAMBIÉN HAY QUE REPASAR SI SE USAN TODAS ESAS VARIABLES Y DEFINIRLAS MEJOR, ES CONFUSO
 * 
 * @param player	Pointer to the t_player structure.
 */

static void	init_player(t_player *player)
{
	player->pos_x = 0;
	player->pos_y = 0;
	player->px = 0;
	player->py = 0;
	player->dir = '\0';
	player->dir_x = 0; //sí hay que inicializar en 0
	player->dir_y = 0; //sí hay que inicializar en 0
	player->plane_x = 0; //sí hay que inicializar en 0
	player->plane_y = 0; //sí hay que inicializar en 0
	player->angle = 0;
	player->move_speed = 0.1;
	player->rot_speed = 0.08;
	player->move_forward = false;
	player->move_backward = false;
	player->move_right = false;
	player->move_left = false;
	player->turn_right = false;
	player->turn_left = false;
}

/* ⚠️ DEBERIAMOS INICIALIZAR EN DEFAULT TB T_RAY??
por ahora t_ray se inicializa en raycasting.c y en minimap_rays... en cada lado
se tocan solo las variables que se usan... */

/**
 * @brief	Allocates memory for the primary componentes of the game, including
 * 			map information, parsed map data, minimap, player, and ray array.
 * 
 * @param game	Pointer to the main t_game structure
 * @return 1 on sucessfully allocation, 0 on any memory allocation failure
 */

//⚠️ IGUAL CAMBIAR DE NOMBRE? alloc_structs()?

static int	init_structs(t_game *game)
{
	game->info = malloc(sizeof(t_info));
	if (!game->info)
		return (error_msg(NULL, NULL, 0));
	game->map = malloc(sizeof(t_map));
	if (!game->map)
		return (error_msg(NULL, NULL, 0));
	game->minimap = malloc(sizeof(t_minimap));
	if (!game->minimap)
		return (error_msg(NULL, NULL, 0));
	game->player = malloc(sizeof(t_player));
	if (!game->player)
		return (error_msg(NULL, NULL, 0));
	game->rays = malloc(WIN_WIDTH * sizeof(t_ray));
	if (!game->rays)
		return (error_msg(NULL, NULL, 0));
	return (1);
}

/**
 * @brief	Initializes the core game state before loading resources.
 * 			Sets default values for all primary fields of the t_game structure,
 * 			allocates memory for the substructures, and initializes them
 * 			with safe defaults.
 * 
 * @param game	Pointer to the main t_game structure
 * @return	1 on success, 0 on failure (allocation error)
 */

int	init_game(t_game *game)
{
	game->mlx = NULL;
	game->img = NULL;
	game->textures[TEX_NO] = NULL;
	game->textures[TEX_SO] = NULL;
	game->textures[TEX_EA] = NULL;
	game->textures[TEX_WE] = NULL;
	if (!init_structs(game))
		return (0);
	init_info(game->info);
	init_map(game->map);
	init_player(game->player);
	return (1);
}
