
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
** Función principal de renderizado
** Llama al raycasting para cada columna y presenta el resultado
*/
/*SUSTITUIR RENDER GAME POR UPDATE_GAME() Y SEPARAR:
- UPDATE PLAYER (YA IMPLEMENTADO)
- RAYCASTING 
- DRAW MINIMAP
- DRAW 3D VIEW


*/
/**/
void	render_game(t_game *game)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		init_ray(game, x);
		calc_step_dist(game);
		perform_dda(game);
		calc_wall_dist(game);
		draw_column(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->window, game->screen->img, 0, 0);
}*/
