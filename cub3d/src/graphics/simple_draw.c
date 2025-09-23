
#include "../../inc/cub3d.h"

/*
** Versión simplificada de dibujado para debug
** - Dibuja líneas con colores sólidos según la dirección de la pared
** - Sin texturas por ahora, solo para verificar que el raycasting funciona
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
	while (y < WINDOW_HEIGHT)
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
** Función principal de raycasting simplificado
** - Versión de debug que dibuja colores sólidos
*/
void	cast_rays_simple(t_game *game)
{
	int	x;
	static int debug_count = 0;
	
	debug_count++;
	if (debug_count == 1)
		printf("🎯 Ejecutando raycasting frame %d\n", debug_count);
	
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		/* Debug para las primeras columnas */
		if (debug_count == 1 && (x == 0 || x == WINDOW_WIDTH/2 || x == WINDOW_WIDTH-1))
			printf("🔍 Rayo %d: pos_x=%.2f, pos_y=%.2f\n", x, game->player->pos_x, game->player->pos_y);
		
		/* Paso 1: Calcular dirección del rayo */
		calculate_ray(game, x);
		
		/* Paso 2: Calcular paso y distancia inicial */
		calculate_step_and_side_dist(game);
		
		/* Paso 3: Ejecutar DDA */
		perform_dda(game);
		
		/* Paso 4: Calcular distancia a la pared */
		calculate_wall_distance(game);
		
		/* Debug para las primeras columnas */
		if (debug_count == 1 && (x == 0 || x == WINDOW_WIDTH/2 || x == WINDOW_WIDTH-1))
		{
			printf("   → Pared encontrada en (%d, %d), distancia=%.2f, altura=%d\n", 
				   game->ray->map_x, game->ray->map_y, game->ray->perp_wall_dist, game->ray->line_height);
			printf("   → draw_start=%d, draw_end=%d\n", game->ray->draw_start, game->ray->draw_end);
		}
		
		/* Paso 5: Dibujar la columna con colores simples */
		draw_simple_line(game, x);
		
		x++;
	}
	
	/* Asegurar que la imagen se muestra */
	if (debug_count == 1)
		printf("📺 Enviando imagen a ventana...\n");
}
