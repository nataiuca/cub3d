/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:25:16 by root              #+#    #+#             */
/*   Updated: 2025/10/14 16:13:10 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Obtiene color de textura con bounds checking
** Clamp en lugar de return 0 para evitar artefactos
*/

int	get_tex_color(mlx_image_t *tex, int x, int y)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
	int		i;

	// char	*dst;
	if (!tex)
		return (0xFF00FF); // no se si esto sobra
	if (x < 0 || y < 0 || x >= (int)tex->width || y >= (int)tex->height)
		return (0);
	i = (y * tex->width + x) * 4;
	// dst = tex->addr + (y * tex->line_len + x * (tex->bits_per_pixel / 8));
	r = tex->pixels[i + 0];
	g = tex->pixels[i + 1];
	b = tex->pixels[i + 2];
	a = tex->pixels[i + 3];
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

/*
** Determina qué textura usar según la pared golpeada
** Norte=0, Sur=1, Este=2, Oeste=3
*/
int	get_texture_index(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (TEX_EA);
		else
			return (TEX_WE);
	}
	else
	{
		if (ray->step_y > 0)
			return (TEX_SO);
		else
			return (TEX_NO);
	}
}

/*
** Calcula coordenada X de la textura
** wall_x: posición exacta donde el rayo golpeó la pared
*/
void	calc_texture_x(t_tex_calc *calc)
{
	if (calc->ray->side == 0)
		calc->wall_x = calc->player->pos_y + calc->ray->dist * calc->ray->dir_y;
	else
		calc->wall_x = calc->player->pos_x + calc->ray->dist * calc->ray->dir_x;
	calc->wall_x -= floor(calc->wall_x);
	calc->tex_x = (int)(calc->wall_x * (double)calc->tex->width);
	if ((calc->ray->side == 0 && calc->ray->dir_x > 0) || (calc->ray->side == 1
			&& calc->ray->dir_y < 0))
		calc->tex_x = calc->tex->width - calc->tex_x - 1;
	if (calc->tex_x < 0)
		calc->tex_x = 0;
	if (calc->tex_x >= (int)calc->tex->width)
		calc->tex_x = calc->tex->width - 1;
}
