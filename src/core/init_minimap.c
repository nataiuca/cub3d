/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:17:04 by root              #+#    #+#             */
/*   Updated: 2025/10/11 15:19:42 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_minimap(t_game *game, t_minimap *minimap) //NO SE SI QUEDA UN POCO FEO PONER ESTO AQUÍ EN VEZ DE CON EL RESTO DE LOS INITS
{
	int	cell_size_w;
	int	cell_size_h;

	cell_size_w = MAX_MINIMAP_SIZE / game->map->width;
	cell_size_h = MAX_MINIMAP_SIZE / game->map->height;
	if (cell_size_w < cell_size_h)
		minimap->cell_size = cell_size_w;
	else
		minimap->cell_size = cell_size_h;
	if (minimap->cell_size < MIN_CELL_SIZE)
		minimap->cell_size = MIN_CELL_SIZE;
	else if (minimap->cell_size > MAX_CELL_SIZE)
		minimap->cell_size = MAX_CELL_SIZE;
	minimap->offset_x = 20;
	minimap->offset_y = 20;
	minimap->width = game->map->width * minimap->cell_size;
	minimap->height = game->map->height * minimap->cell_size;
	minimap->img = mlx_new_image(game->mlx, minimap->width, minimap->height);
	if (!minimap->img)
		return (error_msg(NULL, NULL, 0));
	minimap->ray_count = WIN_WIDTH / 20;
	minimap->rays = malloc(sizeof(t_ray) * minimap->ray_count);
	if (!minimap->rays)
		return(error_msg(NULL, NULL, 0));
	return (1);
}
