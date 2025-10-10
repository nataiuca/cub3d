/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:04:08 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/10 09:32:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
