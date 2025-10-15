/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:25:16 by root              #+#    #+#             */
/*   Updated: 2025/10/15 13:37:34 by natferna         ###   ########.fr       */
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

	if (!tex)
		return (0xFF00FF);
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
void	calc_texture_x(t_ray *ray, t_player *player, mlx_image_t *tex)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = player->pos_y + ray->dist * ray->dir_y;
	else
		wall_x = player->pos_x + ray->dist * ray->dir_x;
	wall_x -= floor(wall_x);
	ray->tex_x = (int)(wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1
			&& ray->dir_y < 0))
		ray->tex_x = tex->width - ray->tex_x - 1;
	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= (int)tex->width)
		ray->tex_x = tex->width - 1;
}

/** Función para cargar una textura desde un archivo
** - Carga una imagen .png usando mlx_load_png
CREO QUE ESTA FUNCIÓN DEBERIA DEBERIA DE ESTAR EN UN LUGAR MÁS GENÉRICO,
	PORK TB LA USA SPRITE
*/
mlx_image_t	*load_texture(void *mlx, char *path)
{
	mlx_texture_t	*tex;
	mlx_image_t		*img;

	tex = mlx_load_png(path);
	if (!tex)
	{
		error_msg(ERR_LOAD_TEX, path, 0);
		return (NULL);
	}
	img = mlx_texture_to_image(mlx, tex);
	if (!img)
	{
		error_msg(NULL, NULL, 0);
		mlx_delete_texture(tex);
	}
	mlx_delete_texture(tex);
	return (img);
}
