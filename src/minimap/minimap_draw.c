/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:24:38 by root              #+#    #+#             */
/*   Updated: 2025/10/12 18:07:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	
 * 			dibuja una línea entre dos puntos: la posición del player y el
 * 			punto donde choca con la pared
 * 			player->px, player->py: coordenadas iniciales
 * 			rays[i]->hit_x , hit_y: coordenadas finales
 * 			dx, dy: distancia en x e y
 * 			x_inc, y_inc: cuánto avanza el rayo en cada paso
 * 			x, y : posición actual del punto que se está dibujando
 * 			i : contador del bucle
 * 
 * 			fabsf() -> pasar los float a valores absolutos. Sirve para decidir
 * 			cuántos píxeles tiene la línea. Si la línea es más horizontal que
 * 			vertical, dx es mayor, necesitamos dx pasos y viceversa. Da igual
 * 			si el valor es positivo o negativo, lo que importa es la magnitud
 * 			
 * 
 * @params
 */

void	draw_line(t_minimap *minimap, t_ray ray, t_player *player)
{
	float	dx; //diff entre x de inicio (pos de player) y x de final (hit del rayo)
	float	dy;
	float	steps; //num de pasos (pixeles)
	float	x; //pos actual de x
	float	y;
	int		i;
	
	float	x_inc;
	float	y_inc;

	dx = ray.hit_x_px - player->px;
	dy = ray.hit_y_px - player->py;
	steps = fmaxf(fabsf(dx), fabsf(dy));
	if (steps < 1)
		steps = 1;
	x_inc = dx / steps;
	y_inc = dy / steps;
	x = player->px;
	y = player->py;
	i = 0;
	while (i <= steps)
	{
		if (x >= 0 && y >= 0 && x < minimap->img->width && y < minimap->img->height)
			mlx_put_pixel(minimap->img, (int)x, (int)y, 0x40E0D0FF);
		x += x_inc;
		y += y_inc;
		i ++;
	}
}

/**
 * @brief
 * 			dibujar un rayo por cada columna de píxeles en la pantalla 3d
 * 			obtener un área de visión (field of view - FOV), desde el rayo
 * 			player->angle - FOV/2 hasta player->angle + FOV/2
 * 			La longitud del rayo dependerá de cuando se tope con una pared
 * 			Calcular distancia a pared
 * 
 * @param
 */

void	draw_rays(t_game *game) //CAMBIAR TODOOO
{
	int		i;
	
	i = 0;
	while (i < game->minimap->ray_count) //120: ray_count ... en esta versión tiene que corresponder al num de columnas dibujadas en el 3d -> WIN_WIDTH??
	{
		draw_line(game->minimap, game->minimap->rays[i], game->player);
		i ++;
	}
}

void	draw_sprite_minimap(t_minimap *minimap, t_sprite *sprite)
{
	int		radius;
	int		dy;
	int		dx;
	int		px;
	int		py;

	if (sprite->x == 0) //significa que no se ha dado valor
		return ;
	radius = minimap->cell_size / 4;
	dy = -radius;
	px = (int)(sprite->x * minimap->cell_size);
	py = (int)(sprite->y * minimap->cell_size);
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			if (dx * dx + dy * dy <= radius * radius)
				mlx_put_pixel(minimap->img, px + dx, py + dy, 0x00FF000);
			dx ++;
		}
		dy ++;
	}
}

/**
 * @brief	
 * 			x, y: posición del jugador en coordenadas de celda (mapa)
 * 			dx, dy: desplazamientos relativos dentro del círculo
 * 			px, py: posición dentro del círculo en píxeles
 * 			radius: radio del círculo -> será un cuarto del tamaño de la celda
 * 	
 *			dy = -radius: se inicia dy en el borde superior del círculo
 *			
 *			bucle while que dibuja un círculo usando la fórmula
 * 			dx2 + dy2 <= radius2
 * 
 * @param minimap
 * @param x	posición del jugador en coordenadas de celda
 * @param y posición del jugador en coordenadas de celda
 */

void	draw_player(t_minimap *minimap, t_player *player)
{
	int		radius;
	int		dy;
	int		dx;
	int		px;
	int		py;

	radius = minimap->cell_size / 4;
	dy = -radius;
	player->px = player->pos_x * minimap->cell_size;
	player->py = player->pos_y * minimap->cell_size;
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			px = (int)(player->px + dx);
			py = (int)(player->py + dy);
			if (dx * dx + dy * dy <= radius * radius)
				mlx_put_pixel(minimap->img, px, py, 0x00008BFF);
			dx ++;
		}
		dy ++;
	}
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


/**
 * @brief	
 * 			SOLO DEBERIA DIBUJAR, NO ACTUALIZAR VALORES
 * 
 * @param cub
 * @param img
 */

void	draw_grid(t_game *game)
{
	int		x;
	int		y;
	char	cell;
	int		color;

	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			cell = game->map->grid[y][x];
			color = get_minimap_cell_color(cell);
			draw_square(game->minimap, x, y, color); //no dibujar el player aún
			if (cell == 'C')
				draw_sprite_minimap(game->minimap, game->sprite);
			x ++;
		}
		y ++;
	}
}
 
void	draw_minimap(t_game *game)
{
	if (!game || !game->minimap || !game->minimap->img)
		return ;
	clear_minimap(game->minimap);
	cast_all_rays_minimap(game, game->minimap);
	draw_grid(game);
	draw_player(game->minimap, game->player);
	draw_rays(game);
}
