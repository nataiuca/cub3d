/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:47:11 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:57:23 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Cleans up all game resources and safely terminates MLX.
 * 			- Frees info, map, minimap, player, rays, textures, and sprites.
 * 			- Properly closes the MLX instance.
 *
 * @param game	Pointer to the main game structure.
 */

void	ft_free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
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
 * @brief	Frees all frames belonging to a sprite.
 * 			Deletes associated MLX images and resets frame pointers.
 *
 * @param game		Pointer to the main game structure.
 * @param sprite	Pointer to the sprite structure.
 */

void	free_sprite_frames(t_game *game, t_sprite *sprite)
{
	int	j;

	if (!sprite || !sprite->frames)
		return ;
	j = 0;
	while (j < sprite->frame_count)
	{
		if (sprite->frames[j])
		{
			mlx_delete_image(game->mlx, sprite->frames[j]);
			sprite->frames[j] = NULL;
		}
		j++;
	}
	free(sprite->frames);
	sprite->frames = NULL;
}
