/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by natferna          #+#    #+#             */
/*   Updated: 2025/10/10 12:46:11 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/*
** Mueve al jugador hacia adelante (W)
** Sliding: intenta mover en X e Y por separado
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

/*
** Mueve al jugador hacia atrás (S)
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

/*
** Mueve al jugador a la izquierda (A)
** Perpendicular a la dirección de vista
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

/*
** Mueve al jugador a la derecha (D)
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

/*
** Rota al jugador (dirección y plano de cámara)
** direction: -1 = izquierda, 1 = derecha
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
