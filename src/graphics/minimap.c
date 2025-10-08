/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:24:38 by root              #+#    #+#             */
/*   Updated: 2025/10/08 12:03:28 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_minimap(t_minimap *minimap)
{
	int	x;
	int	y;

	y = 0;
	while (y < minimap->img->height)
	{
		x = 0;
		while (x < minimap->img->width)
		{
			my_mlx_pixel_put(minimap->img, x, y, 0x00000000);
			x ++;
		}
		y ++;
	}
}

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
	float	x_inc; //incremento de x por step
	float	y_inc;
	float	x; //pos actual de x
	float	y;
	int		i;

	dx = ray.map_x * minimap->cell_size - player->px;
	dy = ray.map_y * minimap->cell_size - player->py;
	x = player->px;
	y = player->py;
	if (fabsf(dx) > fabsf(dy))
		steps = fabsf(dx);
	else
		steps = fabsf(dy);
	if (steps < 1)
		steps = 1;
	x_inc = dx / steps;
	y_inc = dy / steps;
	i = 0;
	while (i <= steps)
	{
		if (x >= 0 && y >= 0 && x < minimap->img->width && y < minimap->img->height)
			my_mlx_pixel_put(minimap->img, (int)x, (int)y, 0x40E0D0FF);
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
	printf("draw_rays\n");
	int		i;
	
	i = 0;
	while (i < 120) //120: ray_count ... en esta versión tiene que corresponder al num de columnas dibujadas en el 3d -> WINDOW_WIDTH??
	{
		printf("wile de ray counts\n");
		draw_line(game->minimap, game->ray[i], game->player);
		i ++;
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
	player->px = player->pos_x * minimap->cell_size + minimap->cell_size / 2;
	player->py = player->pos_y * minimap->cell_size + minimap->cell_size / 2;
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			px = (int)(player->px + dx);
			py = (int)(player->py + dy);
			if (dx * dx + dy * dy <= radius * radius)
				my_mlx_pixel_put(minimap->img, px, py, 0x00008BFF);
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
			my_mlx_pixel_put(minimap->img, px, py, color);
			dx ++;
		}
		dy ++;
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
 * @brief	
 * 			SOLO DEBERIA DIBUJAR, NO ACTUALIZAR VALORES
 * 
 * @param cub
 * @param img
 */

void	draw_minimap(t_game *game)
{
	int		x;
	int		y;
	char	cell;
	int		color;

	if (!game || !game->minimap || !game->minimap->img)
		return ;
	clear_minimap(game->minimap);
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			cell = game->map->grid[y][x];
			color = get_minimap_cell_color(cell);
			draw_square(game->minimap, x, y, color); //no dibujar el player aún
			x ++;
		}
		y ++;
	}
	draw_player(game->minimap, game->player);
	draw_rays(game);
}

int	init_minimap(t_game *game, t_minimap *minimap) //NO SE SI QUEDA UN POCO FEO PONER ESTO AQUÍ EN VEZ DE CON EL RESTO DE LOS INITS
{
	int	width;
	int	height;

	//minimap->cell_size = CELL_SIZE * MINIMAP_SCALE;
	minimap->cell_size = 32; //PRUEBA - VER CÓMO SE VE EN PANTALLA, AVERIGUAR SI SE DEF TAMAÑO DE CELDA DE MAPA EN PIXELES
	minimap->offset_x = 20;
	minimap->offset_y = 20;
	width = game->map->width * minimap->cell_size; //* MINIMAP_SCALE
	height = game->map->height * minimap->cell_size; //* MINIMAP_SCALE
	minimap->img = mlx_new_image(game->mlx, width, height);
	if (!minimap->img)
		return (error_msg(NULL, NULL, 1)); //ya establece su propio error NO SE SI EL ERROR DE MLX YA IMPRIME SU PROPIO MENSAJE O NO...
	return (0);
}
