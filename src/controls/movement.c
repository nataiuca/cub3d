/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by natferna          #+#    #+#             */
/*   Updated: 2025/10/08 00:00:24 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/*
** Verifica colisiones en las esquinas del jugador
** Previene atravesar paredes en diagonal
*/
static int	check_corners(t_game *game, double x, double y, double margin)
{
	if (game->map->grid[(int)(y - margin)][(int)(x - margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y - margin)][(int)(x + margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y + margin)][(int)(x - margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y + margin)][(int)(x + margin)] == '1')
		return (0);
	return (1);
}

/*
** Verifica si una posición es válida (no hay pared)
** Incluye margen de seguridad para evitar atravesar paredes
*/
int	is_valid_pos(t_game *game, double x, double y)
{
	int		map_x;
	int		map_y;
	double	margin;
	char	tile;

	margin = 0.3;
	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map->width
		|| map_y < 0 || map_y >= game->map->height)
		return (0);
	tile = game->map->grid[map_y][map_x];
	if (tile != '0' && tile != 'N' && tile != 'S'
		&& tile != 'E' && tile != 'W')
		return (0);
	return (check_corners(game, x, y, margin));
}

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
