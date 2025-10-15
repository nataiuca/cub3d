/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_graphics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:17:02 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:15:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Loads all wall textures defined in the map configuration.
 * 			Each cardinal direction (N, S, E, W) is loaded into the
 * 			game’s texture array.
 *
 * @param game	Pointer to the main game structure.
 *
 * @return	1 on success, 0 on failure.
 */

int	load_wall_textures(t_game *game)
{
	game->textures[TEX_NO] = load_texture(game->mlx, game->map->no_texture);
	if (!game->textures[0])
		return (0);
	game->textures[TEX_SO] = load_texture(game->mlx, game->map->so_texture);
	if (!game->textures[1])
		return (0);
	game->textures[TEX_EA] = load_texture(game->mlx, game->map->ea_texture);
	if (!game->textures[2])
		return (0);
	game->textures[TEX_WE] = load_texture(game->mlx, game->map->we_texture);
	if (!game->textures[3])
		return (0);
	return (1);
}

/**
 * @brief	Initializes all MLX graphical components.
 * 			Creates the main window, rendering image, minimap, wall textures,
 * 			and sprites. Configures mouse and cursor hooks.
 *
 * @param game	Pointer to the main game structure.
 *
 * @return	1 on success, 0 on error.
 */

int	init_graphics(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (error_msg(ERR_MLX_INIT, NULL, 0));
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return (error_msg(NULL, NULL, 0));
	if (!init_minimap(game, game->minimap))
		return (error_msg(ERR_MINIMAP_INIT, NULL, 0));
	if (!load_wall_textures(game))
		return (error_msg(ERR_LOAD_WALL_TEX, NULL, 0));
	if (!init_sprites(game))
		return (error_msg(ERR_SPRITE_INIT, NULL, 0));
	if (!load_all_sprites(game))
		return (error_msg(ERR_LOAD_SPRITE, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_msg(NULL, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->minimap->img,
			game->minimap->offset_x, game->minimap->offset_y) < 0)
		return (error_msg(NULL, NULL, 0));
	return (1);
}
