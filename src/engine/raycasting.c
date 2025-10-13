/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:42:11 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/13 18:01:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/**
 * @brief	Inicializa el rayo para la comlumna número 'col' de la pantalla.
 * 			- Proyecta la columna de pantalla al rango [-1, 1], donde -1
 * 			es el extremo izquierdo de la pantalla y el 1 el derecho
 * 			HACER ESQUEMA DIBUJADO DE ESTO!
 * 			- A partir de la proyección de la columna en pantalla en rango [-1 ,1]
 * 			se calcula la dirección del rayo en el campo de visión (dir_x, dir_y)
 * 			- Usa el camera_x para ajustar la dirección del rayo a lo ancho de la 
 * 			pantalla
 * 			- Si la posición incial del player en el mapa es 0, pone delta_dx
 * 			en UN VALOR ALTISIMO para prevenir división por cero
 * 			- delta_dx/y es la distancia entre líneas verticales/horizontales
 * 			que el rayo debe recorrer.
 * 
 * @param ray	El rayo concreto a inicializar, el de la columna col
 * @param player	Puntero a estructura del jugador
 * @param col	Columna actual, número de rayo
 */

//CAMBIARLE DE NOMBRE! PARA QUE NO SE CONFUNDA CON INIT_RAY DEFAULT
t_ray	init_ray(t_player *player, int col)
{
	t_ray	ray;

	ray.camera_x = 2 * (double)col / (double)WIN_WIDTH - 1;
	ray.dir_x = player->dir_x + player->plane_x * ray.camera_x;
	ray.dir_y = player->dir_y + player->plane_y * ray.camera_x;
	ray.map_x = (int)player->pos_x;
	ray.map_y = (int)player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dx = HUGE_VAL;
	else
		ray.delta_dx = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dy = HUGE_VAL;
	else
		ray.delta_dy = fabs(1 / ray.dir_y);
	return(ray);
}

/**
 * @brief	Calcula:
 * 			- step_x/y: dirección en la que el rayo se moverá (hacia
 * 			adelante o atrás). 
 * 			- side_dx/y: distancia desde la posición del jugadro hasta el
 * 			primer cruce de línea de celda.
 * 
 * @param game
 */
void	calc_step_dist(t_ray *ray, t_player *player)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dx = (player->pos_x - ray->map_x)* ray->delta_dx;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dx = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dx;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dy = (player->pos_y - ray->map_y)* ray->delta_dy;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dy = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dy;
	}
}

/**
 * @brief	Checkea si hemos golpeado una pared
 * 			- Si estamos fuera del mapa: devuelve 1 para evitar segfault
 * 			- Si la celda actual del mapa contiene '1' (pared), devuelve 1.
 */
static int	check_hit(t_ray *ray, t_map *map)
{
	if (ray->map_x < 0 || ray->map_x >= map->width) //si estamos fuera de los extremos laterales del mapa
		return (1);
	if (ray->map_y < 0 || ray->map_y >= map->height) //si estamos fuera de los extremos superior e inferior del mapa
		return (1);
	if (map->grid[ray->map_y][ray->map_x] == '1')
		return (1);
	return (0);
}

/**
 * @brief	Algoritmo DDA
 * 			- Compara cuál es el menor (side_dx o y) para determinar si avanza
 * 			en x o y. 
 * 			- Avanza a la siguiente celda en esa dirección
 * 			- Repite hasta golpear una pared
 * 
 * 
 */

void	perform_dda(t_game *game, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_dx < ray->side_dy)
		{
			ray->side_dx += ray->delta_dx;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dy += ray->delta_dy;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (check_hit(ray, game->map))
			ray->hit = 1;
	}
}

/**
 * @brief	Calcula:
 * 			- Distancia perpendicular (dist) desde el player hasta la pared hitteada
 * 
 * La distancia perpendicular se calcula usando geometría para evitar el efecto
 * ojo de pez
 * 
 * Cálculo de la distancia a la pared hiteada:
 * Ecuación de línea paramétrica:
 * (x, y) = (player_x, player_y) + t * (dir_x, dir_y)
 * 
 * 	t: distancia a lo largo del rayo
 * 
 * 			- La altura de la línea a dibujar en pantalla (inversamente
 * 			proporcional a la distancia de choque)
 * 			- draw_start/draw_end: límites de esa línea vertical en la pantalla,
 * 			para no dibujar fuera de rango
 */

void	calc_wall_dist(t_ray *ray, t_player *player)
{
	double EPS = 1e-9;

	if (ray->side == 0) //hitea pared vertical (E-W)
	{
		if (fabs(ray->dir_x) < EPS)
			ray->dist = HUGE_VAL;
		else
			ray->dist = (ray->map_x - player->pos_x + (1 - ray->step_x) / 2.0) / ray->dir_x;
	}
	else //hitea pared horizontal
	{
		if (fabs(ray->dir_y) < EPS)
			ray->dist = HUGE_VAL;
		else
			ray->dist = (ray->map_y - player->pos_y + (1 - ray->step_y) / 2.0) / ray->dir_y;
	}
	if (!isfinite(ray->dist) || ray->dist <= 0.0)
		ray->line_height = WIN_HEIGHT;
	else
		ray->line_height = (int)(WIN_HEIGHT / ray->dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

void	cast_all_rays(t_game *game)
{
	int		i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		game->rays[i] = init_ray(game->player, i);
		calc_step_dist(&game->rays[i], game->player);
		perform_dda(game, &game->rays[i]);
		calc_wall_dist(&game->rays[i], game->player);
		i ++;
	}
}
