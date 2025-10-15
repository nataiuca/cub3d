/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:47:24 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:47:25 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Frees all memory and resources associated with the minimap.
 *
 * @param game	Pointer to the main game structure.
 */
void	free_minimap(t_game *game)
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
 * @brief	Frees all loaded textures and screen images.
 * 			- Deletes MLX texture images for the four walls.
 * 			- Deletes the main render image if present.
 *
 * @param game	Pointer to the main game structure.
 */
void	free_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->textures[i])
		{
			mlx_delete_image(game->mlx, game->textures[i]);
			game->textures[i] = NULL;
		}
		i++;
	}
	if (game->img)
	{
		mlx_delete_image(game->mlx, game->img);
		game->img = NULL;
	}
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
