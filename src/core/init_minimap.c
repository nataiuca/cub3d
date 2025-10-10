/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:17:04 by root              #+#    #+#             */
/*   Updated: 2025/10/10 17:17:12 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_minimap(t_game *game, t_minimap *minimap) //NO SE SI QUEDA UN POCO FEO PONER ESTO AQUÍ EN VEZ DE CON EL RESTO DE LOS INITS
{
	minimap->cell_size = 32 * MINIMAP_SCALE; //PRUEBA - VER CÓMO SE VE EN PANTALLA, AVERIGUAR SI SE DEF TAMAÑO DE CELDA DE MAPA EN PIXELES
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
