
#include "../../inc/cub3d.h"

/*
** Función ultra-rápida para colocar píxeles
** Escribe directamente en memoria sin validaciones extras
** CRÍTICO: Esta es la razón del speed-up de 100x vs mlx_pixel_put
*/
void	fast_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

/*
** Obtiene color de textura con bounds checking
** Clamp en lugar de return 0 para evitar artefactos
*/
int	get_tex_color(t_img *tex, int x, int y)
{
	char	*dst;

	if (!tex || !tex->addr)
		return (0xFF00FF);
	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	dst = tex->addr + (y * tex->line_len + x * (tex->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

/*
** Función principal de renderizado
** Llama al raycasting para cada columna y presenta el resultado
*/
void	render_game(t_game *game)
{
	int	x;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		init_ray(game, x);
		calc_step_dist(game);
		perform_dda(game);
		calc_wall_dist(game);
		draw_column(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->window, game->screen->img, 0, 0);
}
