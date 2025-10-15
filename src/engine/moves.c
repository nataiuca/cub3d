/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by natferna          #+#    #+#             */
/*   Updated: 2025/10/15 16:02:44 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Moves the player forward (W key). 
 * 			Implements sliding by checking X and Y movement separately.
 *
 * @param game		Pointer to the main game structure.
 * @param player	Pointer to the player structure.
 */

void	move_forward(t_game *game, t_player *player)
{
	double	new_x;
	double	new_y;

	new_x = player->pos_x + player->dir_x * player->move_speed;
	new_y = player->pos_y + player->dir_y * player->move_speed;
	if (is_valid_pos(game, new_x, player->pos_y))
		player->pos_x = new_x;
	if (is_valid_pos(game, player->pos_x, new_y))
		player->pos_y = new_y;
	player->move_forward = false;
}

/**
 * @brief	Moves the player backward (S key).
 *
 * @param game		Pointer to the main game structure.
 * @param player	Pointer to the player structure.
 */

void	move_backward(t_game *game, t_player *player)
{
	double	new_x;
	double	new_y;

	new_x = player->pos_x - player->dir_x * player->move_speed;
	new_y = player->pos_y - player->dir_y * player->move_speed;
	if (is_valid_pos(game, new_x, player->pos_y))
		player->pos_x = new_x;
	if (is_valid_pos(game, player->pos_x, new_y))
		player->pos_y = new_y;
	player->move_backward = false;
}

/**
 * @brief	Moves the player to the left (A key).
 * 			Moves perpendicularly to the camera direction.
 *
 * @param game		Pointer to the main game structure.
 * @param player	Pointer to the player structure.
 */

void	move_left(t_game *game, t_player *player)
{
	double	new_x;
	double	new_y;

	new_x = player->pos_x + player->dir_y * player->move_speed;
	new_y = player->pos_y - player->dir_x * player->move_speed;
	if (is_valid_pos(game, new_x, player->pos_y))
		player->pos_x = new_x;
	if (is_valid_pos(game, player->pos_x, new_y))
		player->pos_y = new_y;
	player->move_left = false;
}

/**
 * @brief	Moves the player to the right (D key).
 *
 * @param game		Pointer to the main game structure.
 * @param player	Pointer to the player structure.
 */

void	move_right(t_game *game, t_player *player)
{
	double	new_x;
	double	new_y;

	new_x = player->pos_x - player->dir_y * player->move_speed;
	new_y = player->pos_y + player->dir_x * player->move_speed;
	if (is_valid_pos(game, new_x, player->pos_y))
		player->pos_x = new_x;
	if (is_valid_pos(game, player->pos_x, new_y))
		player->pos_y = new_y;
	player->move_right = false;
}

/**
 * @brief	Rotates the player's view direction and camera plane.
 * 			Handles both left (-1) and right (1) rotation directions.
 *
 * @param player	Pointer to the player structure.
 * @param direction	Rotation direction: -1 for left, 1 for right.
 */

void	rotate_player(t_player *player, int direction)
{
	double	old_dir_x;
	double	old_plane_x;
	double	rot_speed;

	rot_speed = player->rot_speed * direction;
	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(rot_speed) - player->dir_y
		* sin(rot_speed);
	player->dir_y = old_dir_x * sin(rot_speed) + player->dir_y * cos(rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(rot_speed) - player->plane_y
		* sin(rot_speed);
	player->plane_y = old_plane_x * sin(rot_speed) + player->plane_y
		* cos(rot_speed);
	if (direction == RIGHT)
		player->turn_right = false;
	else
		player->turn_left = false;
}
