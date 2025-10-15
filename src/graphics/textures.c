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

/**
 * @brief	Retrieves a pixel color from a texture image with boundary checks.
 * 			Clamps coordinates to prevent out-of-bounds access and rendering artifacts.
 *
 * @param tex	Pointer to the texture image.
 * @param x		Texture X coordinate.
 * @param y		Texture Y coordinate.
 *
 * @return	Packed RGBA color value (uint32_t).
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

/**
 * @brief	Selects the appropriate wall texture based on the ray collision.
 * 			Determines orientation (N/S/E/W) using ray direction and step.
 *
 * @param ray	Pointer to the ray structure.
 *
 * @return	Texture index corresponding to the hit wall.
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

/**
 * @brief	Computes the horizontal texture coordinate (tex_x)
 * 			for the wall segment hit by the ray.
 *
 * @param ray		Pointer to the ray structure.
 * @param player	Pointer to the player structure.
 * @param tex		Pointer to the texture image.
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

/**
 * @brief	Loads a PNG texture file into an MLX image.
 * 			Used by both walls and sprites.
 *
 * @param mlx	Pointer to the MLX instance.
 * @param path	Path to the .png texture file.
 *
 * @return	Pointer to the created MLX image, or NULL on failure.
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
