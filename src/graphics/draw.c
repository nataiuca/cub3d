
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:58:51 by root              #+#    #+#             */
/*   Updated: 2025/10/15 10:28:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/*
** Aplica oscurecimiento a paredes horizontales
** Mejora la percepción de profundidad
*/
/*int	apply_shading(int color, int side)
{
	int	r;
	int	g;
	int	b;

	if (side == 1)
	{
		r = ((color >> 16) & 0xFF) * 3 / 4;
		g = ((color >> 8) & 0xFF) * 3 / 4;
		b = (color & 0xFF) * 3 / 4;
		return ((r << 16) | (g << 8) | b);
	}
	return (color);
}*/

/*
** Dibuja techo y suelo para una columna
*/
static void	draw_floor_ceiling(t_game *game, t_ray *ray, int x)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
	{
		//fast_pixel_put(game->screen, x, y, game->map->ceiling_color);
		mlx_put_pixel(game->img, x, y, game->map->ceiling_color);
		y++;
	}
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		//fast_pixel_put(game->screen, x, y, game->map->floor_color);
		mlx_put_pixel(game->img, x, y, game->map->floor_color);
		y++;
	}
}

/*
** Dibuja la pared con textura para una columna
*/



/*
** Dibuja una columna completa (techo, pared, suelo)
** Esta función se llama para cada X de la pantalla
*/
void	draw_column(t_game *game, int col, t_ray *ray)
{
	int		tex_index;
	mlx_image_t	*tex;
	double	wall_x;
	int		tex_x;

	tex_index = get_texture_index(ray);
	tex = game->textures[tex_index];
	if (!tex)
	{
		draw_floor_ceiling(game, ray, col);
		return ;
	}
	calc_texture_x(ray, game->player, &wall_x, &tex_x, tex);
	draw_floor_ceiling(game, ray, col);
	draw_textured_wall(game, ray, col, tex, tex_x);
}

void	draw_3d_view(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		draw_column(game, i, &game->rays[i]);
		i ++;
	}
}
