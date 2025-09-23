
#include "../../inc/cub3d.h"

/*
** Función para liberar el mapa
** - Libera cada línea del grid
** - Libera las rutas de texturas
** - Libera la estructura del mapa
*/
void	free_map(t_map *map)
{
	int	i;
	
	if (!map)
		return;
		
	/* Liberar cada línea del mapa */
	if (map->grid)
	{
		i = 0;
		while (i < map->height && map->grid[i])
		{
			free(map->grid[i]);
			i++;
		}
		free(map->grid);
	}
	
	/* Liberar rutas de texturas */
	if (map->north_texture)
		free(map->north_texture);
	if (map->south_texture)
		free(map->south_texture);
	if (map->east_texture)
		free(map->east_texture);
	if (map->west_texture)
		free(map->west_texture);
		
	/* Liberar la estructura del mapa */
	free(map);
	printf("🧹 Mapa liberado de memoria\n");
}

/*
** Función para liberar una imagen de MLX
** - Destruye la imagen en MLX si existe
** - Libera la estructura
*/
static void	free_image(void *mlx, t_img *img)
{
	if (!img)
		return;
		
	if (img->img && mlx)
		mlx_destroy_image(mlx, img->img);
	free(img);
}

/*
** Función para limpiar todo el juego
** - Libera todas las texturas
** - Destruye la ventana
** - Libera todas las estructuras
** - Cierra la conexión con MLX
*/
void	cleanup_game(t_game *game)
{
	int	i;
	
	if (!game)
		return;
		
	printf("🧹 Limpiando memoria del juego...\n");
	
	/* Liberar texturas */
	i = 0;
	while (i < 4)
	{
		free_image(game->mlx, game->textures[i]);
		i++;
	}
	
	/* Liberar imagen de pantalla */
	free_image(game->mlx, game->screen);
	
	/* Destruir ventana */
	if (game->window && game->mlx)
		mlx_destroy_window(game->mlx, game->window);
		
	/* Liberar estructuras */
	free_map(game->map);
	
	if (game->player)
		free(game->player);
		
	if (game->ray)
		free(game->ray);
		
	/* Cerrar MLX (en algunos sistemas) */
	if (game->mlx)
	{
		/* En Linux: mlx_destroy_display(game->mlx); */
		free(game->mlx);
	}
	
	printf("✅ Memoria liberada correctamente\n");
}

/*
** Función para inicializar el mapa con valores por defecto
** - Pone todo a NULL o 0
** - Colores por defecto para suelo y techo
*/
void	init_map(t_map *map)
{
	if (!map)
		return;
		
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->north_texture = NULL;
	map->south_texture = NULL;
	map->east_texture = NULL;
	map->west_texture = NULL;
	map->floor_color = 0x654321;   /* Color marrón por defecto */
	map->ceiling_color = 0x87CEEB; /* Color azul cielo por defecto */
	
	printf("🗺️  Mapa inicializado con valores por defecto\n");
}
