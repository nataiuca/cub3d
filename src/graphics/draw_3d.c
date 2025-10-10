
#include "../../inc/cub3d.h"

/*
** Obtiene color de textura con bounds checking
** Clamp en lugar de return 0 para evitar artefactos
*/

int	get_tex_color(mlx_image_t *tex, int x, int y)
{
	//char	*dst;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
	int		i;

	if (!tex )
		return (0xFF00FF); //no se si esto sobra
	if (x < 0 || y < 0 || x >= (int)tex->width || y>= (int)tex->height)
		return (0);
	i = (y * tex->width + x) * 4;
	//dst = tex->addr + (y * tex->line_len + x * (tex->bits_per_pixel / 8));
	r = tex->pixels[i + 0];
	g = tex->pixels[i + 1];
	b = tex->pixels[i + 2];
	a = tex->pixels[i + 3];
	//return (*(unsigned int *)dst);
	return ((a << 24) | (r << 16) | (g << 8) | b);
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
void	calc_texture_x(t_ray *ray, t_player *player, double *wall_x, int *tex_x, mlx_image_t *tex)
{
	if (ray->side == 0)
		*wall_x = player->pos_y + ray->perp_wall_d * ray->dir_y;
	else
		*wall_x = player->pos_x + ray->perp_wall_d * ray->dir_x;
	*wall_x -= floor(*wall_x);
	*tex_x = (int)(*wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1 && ray->dir_y < 0))
		*tex_x = tex->width - *tex_x - 1;
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= (int)tex->width)
		*tex_x = tex->width - 1;
}

/*
** Aplica oscurecimiento a paredes horizontales
** Mejora la percepción de profundidad
*/
static int	apply_shading(int color, int side)
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
}

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
static void	draw_textured_wall(t_game *game, t_ray *ray, int x, mlx_image_t *tex, int tex_x)
{
	int		y;
	int		tex_y;
	int		color;
	double	step;
	double	tex_pos;


	step = (double)tex->height / (double)ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2.0 + ray->line_height / 2.0) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= (int)tex->height)
			tex_y = tex->height - 1;
		tex_pos += step;
		color = get_tex_color(tex, tex_x, tex_y);
		color = apply_shading(color, ray->side);
		mlx_put_pixel(game->img, x, y, color);
		y++;
	}
}

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
	printf("DEBUG: draw_3d_view\n");
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		draw_column(game, i, &game->rays[i]);
		i ++;
	}
}
