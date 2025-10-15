/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:47:02 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:47:03 by root             ###   ########.fr       */
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

/**
 * @brief	Frees all frames belonging to a sprite.
 * 			Deletes associated MLX images and resets frame pointers.
 *
 * @param game		Pointer to the main game structure.
 * @param sprite	Pointer to the sprite structure.
 */
void	free_sprite(t_game *game)
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
