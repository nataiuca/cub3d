/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:13:26 by root              #+#    #+#             */
/*   Updated: 2025/10/10 13:07:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_minimap(t_minimap *minimap)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < minimap->img->height)
	{
		x = 0;
		while (x < minimap->img->width)
		{
			mlx_put_pixel(minimap->img, x, y, 0x00000000);
			x ++;
		}
		y ++;
	}
}

int	get_minimap_cell_color(char cell)
{
	if (cell == '1') //wall
		return (0xFF7F50FF); //coral
	else if (cell == '0') //floor
		return (0x556B2FFF); //kaki
	else if (ft_strchr("NSEW", cell))
		return (0x556B2FFF); //kaki para el suelo
	return (0x222222FF); //dark grey - no deberia ocurrir - espacios
}

/**
 * @brief	Pinta todos los píxeles de una celda (x, y) del color pasado  
 * 			como parámetro.
 * 			- x e y son las coordenadas del mapa lógico
 * 			- Cada celda del mapa se representa en el minimapa como un 
 * 			cuadrado de tamaño CELL_SIZE * CELL_SIZE píxeles
 * 			- El bucle interno dx recorre los píxeles de una fila del cuadrado
 * 			- El bucle externo dy recorre las filas del cuadrado
 * 			- px y py calculan la posición real del píxel en la imagen:
 * 				- x * CELL_SIZE: desplazamiento horizontal, salta cuadrados previos
 * 				- y * CELL_SIZE: desplazamiento vertical
 * 				- Luego se suma dx y dy para moverse dentro del cuadrado actual
 * 				- Y en cada posición (px, py) se pinta el pixel del color que toca
 * 
 * @param img	Imagen en la que se pone el pixel
 * @param x		Coordenada x de la celda del mapa - del mapa itnroducido como input
 * @param y		Coordenada y de la celda del mapa
 * @param color	Color del que hay que pintar
 * 
 */

void	draw_square(t_minimap *minimap, int x, int y, int color)
{
	int	dy;
	int	dx;
	int	px;
	int	py;

	dy = 0;
	while (dy < minimap->cell_size)
	{
		dx = 0;
		while (dx < minimap->cell_size)
		{
			px = x * minimap->cell_size + dx;
			py = y * minimap->cell_size + dy;
			mlx_put_pixel(minimap->img, px, py, color);
			dx ++;
		}
		dy ++;
	}
}

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
