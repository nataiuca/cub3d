
#include "../../inc/cub3d.h"

/*
** Función para renderizar usando mlx_pixel_put directo (compatible con Mac)
*/
void	draw_direct_line(t_game *game, int x)
{
	int		y;
	int		wall_color;
	
	/* Determinar color según la dirección de la pared */
	if (game->ray->side == 0) /* Lado vertical (Norte/Sur) */
	{
		if (game->ray->step_x > 0)
			wall_color = 0xFF0000; /* Rojo para Este */
		else
			wall_color = 0x0000FF; /* Azul para Oeste */
	}
	else /* Lado horizontal (Este/Oeste) */
	{
		if (game->ray->step_y > 0)
			wall_color = 0x00FF00; /* Verde para Sur */
		else
			wall_color = 0xFFFFFF; /* Blanco para Norte */
	}
	
	/* Oscurecer paredes horizontales */
	if (game->ray->side == 1)
		wall_color = (wall_color >> 1) & 8355711;
	
	/* Dibujar la columna píxel por píxel directamente */
	y = 0;
	while (y < WIN_HEIGHT)
	{
		if (y < game->ray->draw_start)
		{
			/* Techo */
			mlx_pixel_put(game->mlx, game->window, x, y, game->map->ceiling_color);
		}
		else if (y >= game->ray->draw_start && y <= game->ray->draw_end)
		{
			/* Pared */
			mlx_pixel_put(game->mlx, game->window, x, y, wall_color);
		}
		else
		{
			/* Suelo */
			mlx_pixel_put(game->mlx, game->window, x, y, game->map->floor_color);
		}
		y++;
	}
}

/*
** Función para dibujar una línea con buffer (para raycasting normal)
** - Versión con colores sólidos para testing
*/
static void	draw_simple_line(t_game *game, int x)
{
	int	y;
	int	wall_color;
	
	/* Determinar color según la dirección de la pared */
	if (game->ray->side == 0) /* Lado vertical (Norte/Sur) */
	{
		if (game->ray->step_x > 0)
			wall_color = 0xFF0000; /* Rojo para Este */
		else
			wall_color = 0x0000FF; /* Azul para Oeste */
	}
	else /* Lado horizontal (Este/Oeste) */
	{
		if (game->ray->step_y > 0)
			wall_color = 0x00FF00; /* Verde para Sur */
		else
			wall_color = 0xFFFFFF; /* Blanco para Norte */
	}
	
	/* Oscurecer paredes horizontales para dar efecto de profundidad */
	if (game->ray->side == 1)
		wall_color = (wall_color >> 1) & 8355711; /* Dividir RGB por 2 */
	
	/* Dibujar la columna píxel por píxel */
	y = 0;
	while (y < WIN_HEIGHT)
	{
		if (y < game->ray->draw_start)
		{
			/* Dibujar techo */
			my_mlx_pixel_put(game->screen, x, y, game->map->ceiling_color);
		}
		else if (y >= game->ray->draw_start && y <= game->ray->draw_end)
		{
			/* Dibujar pared con color sólido */
			my_mlx_pixel_put(game->screen, x, y, wall_color);
		}
		else
		{
			/* Dibujar suelo */
			my_mlx_pixel_put(game->screen, x, y, game->map->floor_color);
		}
		y++;
	}
}

/*
** Función principal de raycasting simplificado con buffer
** - Versión que usa buffer de imagen (mejor para Linux)
*/
void	cast_rays_simple(t_game *game)
{
	int	x;
	static int debug_count = 0;
	
	debug_count++;
	if (debug_count <= 3)
		printf("Raycasting con buffer frame %d\n", debug_count);
	
	x = 0;
	while (x < WIN_WIDTH)
	{
		/* Paso 1: Calcular dirección del rayo */
		calculate_ray(game, x);
		
		/* Paso 2: Calcular paso y distancia inicial */
		calculate_step_and_side_dist(game);
		
		/* Paso 3: Ejecutar DDA */
		perform_dda(game);
		
		/* Paso 4: Calcular distancia a la pared */
		calculate_wall_distance(game);
		
		/* Paso 5: Dibujar la columna con colores simples */
		draw_simple_line(game, x);
		
		x++;
	}
}
