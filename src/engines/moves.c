/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by natferna          #+#    #+#             */
/*   Updated: 2025/10/09 15:03:38 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/*
** Mueve al jugador hacia adelante (W)
** Sliding: intenta mover en X e Y por separado
*/
void	move_forward(t_game *game)
{
	double	new_x;
	double	new_y;
	double	speed;

	speed = game->player->move_speed;
	new_x = game->player->pos_x + game->player->dir_x * speed;
	new_y = game->player->pos_y + game->player->dir_y * speed;
	if (is_valid_pos(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (is_valid_pos(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Mueve al jugador hacia atrás (S)
*/
void	move_backward(t_game *game)
{
	double	new_x;
	double	new_y;
	double	speed;

	speed = game->player->move_speed;
	new_x = game->player->pos_x - game->player->dir_x * speed;
	new_y = game->player->pos_y - game->player->dir_y * speed;
	if (is_valid_pos(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (is_valid_pos(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Mueve al jugador a la izquierda (A)
** Perpendicular a la dirección de vista
*/
void	move_left(t_game *game)
{
	double	new_x;
	double	new_y;
	double	speed;

	speed = game->player->move_speed;
	new_x = game->player->pos_x + game->player->dir_y * speed;
	new_y = game->player->pos_y - game->player->dir_x * speed;
	if (is_valid_pos(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (is_valid_pos(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Mueve al jugador a la derecha (D)
*/
void	move_right(t_game *game)
{
	double	new_x;
	double	new_y;
	double	speed;

	speed = game->player->move_speed;
	new_x = game->player->pos_x - game->player->dir_y * speed;
	new_y = game->player->pos_y + game->player->dir_x * speed;
	if (is_valid_pos(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (is_valid_pos(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Rota al jugador (dirección y plano de cámara)
** direction: -1 = izquierda, 1 = derecha
*/
void	rotate_player(t_game *game, int direction)
{
	double	old_dir_x;
	double	old_plane_x;
	double	rot_speed;

	rot_speed = game->player->rot_speed * direction;
	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(rot_speed)
		- game->player->dir_y * sin(rot_speed);
	game->player->dir_y = old_dir_x * sin(rot_speed)
		+ game->player->dir_y * cos(rot_speed);
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(rot_speed)
		- game->player->plane_y * sin(rot_speed);
	game->player->plane_y = old_plane_x * sin(rot_speed)
		+ game->player->plane_y * cos(rot_speed);
}
