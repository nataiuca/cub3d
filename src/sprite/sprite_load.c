/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:21:55 by root              #+#    #+#             */
/*   Updated: 2025/10/15 11:45:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Loads all texture frames for a single animated sprite.
 * 			- Allocates memory for the sprite's frames array
 * 			- Loads each frame using load_texture()
 * 			- If any frame fails to load, the function cleans up previously
 * 			loaded frames and returns failure
 * 
 * @param game	Pointer to the game main structure
 * @param sprite	Pointer to the sprite to load frames for
 * @return	1 on success, 0 on failure
 */

static int	load_sprite(t_game *game, t_sprite *sprite)
{
	int			i;
	char		*sprite_paths[6];

	sprite->frames = malloc(sizeof(mlx_image_t *) * sprite->frame_count);
	if (!sprite->frames)
		return (error_msg(NULL, NULL, 0));
	sprite_paths[0] = "./sprites/cat_1.png";
	sprite_paths[1] = "./sprites/cat_2.png";
	sprite_paths[2] = "./sprites/cat_3.png";
	sprite_paths[3] = "./sprites/cat_4.png";
	sprite_paths[4] = "./sprites/cat_5.png";
	sprite_paths[5] = "./sprites/cat_6.png";
	i = 0;
	while (i < sprite->frame_count)
	{
		sprite->frames[i] = load_texture(game->mlx, sprite_paths[i]);
		if (!sprite->frames[i])
		{
			free_sprite_frames(game, sprite);
			return (0);
		}
		i ++;
	}
	return (1);
}

/**
 * @brief	Loads all sprites in the game.
 * 
 * @param game	Pointer to the game main structure.
 * @param return	1 on succes, 0 if any sprite fails to load
 */

int	load_all_sprites(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->sprite_count)
	{
		if (!load_sprite(game, game->sprite[i]))
			return (0);
		i ++;
	}
	return (1);
}
