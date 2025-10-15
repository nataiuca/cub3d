/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:47:24 by root              #+#    #+#             */
/*   Updated: 2025/10/15 16:01:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Frees all frames belonging to a sprite.
 * 			Deletes associated MLX images and resets frame pointers.
 *
 * @param game		Pointer to the main game structure.
 * @param sprite	Pointer to the sprite structure.
 */

static void	free_sprite(t_game *game)
{
	int	i;

	if (!game || !game->sprite)
		return ;
	i = 0;
	while (i < game->sprite_count)
	{
		if (game->sprite[i])
		{
			free_sprite_frames(game, game->sprite[i]);
			free(game->sprite[i]);
			game->sprite[i] = NULL;
		}
		i++;
	}
	free(game->sprite);
	game->sprite = NULL;
}

/**
 * @brief	Frees all memory and resources associated with the minimap.
 *
 * @param game	Pointer to the main game structure.
 */

static void	free_minimap(t_game *game)
{
	if (!game->minimap)
		return ;
	if (game->minimap->rays)
		free(game->minimap->rays);
	if (game->minimap->img)
		mlx_delete_image(game->mlx, game->minimap->img);
	free(game->minimap);
	game->minimap = NULL;
}

/**
 * @brief	Frees the game’s info structure and all its allocated fields.
 *
 * @param game	Pointer to the main game structure.
 */

static void	free_info(t_game *game)
{
	if (!game->info)
		return ;
	ft_free_str_array(game->info->file_raw_data);
	game->info->file_raw_data = NULL;
	free(game->info->f_color_raw);
	free(game->info->c_color_raw);
	ft_free_str_array(game->info->map_raw);
	game->info->map_raw = NULL;
	free(game->info);
	game->info = NULL;
}

/**
 * @brief	Frees all memory used by the map structure.
 * 			- Releases map grid and texture paths.
 * 			- Finally frees the map structure itself.
 *
 * @param game	Pointer to the main game structure.
 */

static void	free_map(t_game *game)
{
	if (!game->map)
		return ;
	ft_free_str_array(game->map->grid);
	if (game->map->no_texture)
		free(game->map->no_texture);
	if (game->map->so_texture)
		free(game->map->so_texture);
	if (game->map->ea_texture)
		free(game->map->ea_texture);
	if (game->map->we_texture)
		free(game->map->we_texture);
	free(game->map);
	game->map = NULL;
}

/**
 * @brief	Cleans up all game resources and safely terminates MLX.
 * 			- Frees info, map, minimap, player, rays, textures, and sprites.
 * 			- Properly closes the MLX instance.
 *
 * @param game	Pointer to the main game structure.
 */

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	free_info(game);
	free_map(game);
	free_minimap(game);
	if (game->player)
		free(game->player);
	if (game->rays)
		free(game->rays);
	free_textures(game);
	free_sprite(game);
	if (game->mlx)
	{
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}
