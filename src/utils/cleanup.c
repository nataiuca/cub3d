
#include "../../inc/cub3d.h"

void	ft_free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i ++;
	}
	free(array);
}

void	free_mapinfo(t_mapinfo *mapinfo)
{
	ft_printf("DEBUG: free_mapinfo\n");
	if (!mapinfo)
		return ;
	ft_free_str_array(mapinfo->file_raw_data);
	mapinfo->file_raw_data = NULL;
	/*free(mapinfo->no_path);
	free(mapinfo->so_path);
	free(mapinfo->ea_path);
	free(mapinfo->we_path);*/
	free(mapinfo->f_color_raw);
	free(mapinfo->c_color_raw);
	ft_free_str_array(mapinfo->map_raw);

	free(mapinfo);
}

/*
** Función para liberar el mapa
** - Libera cada línea del grid
** - Libera las rutas de texturas
** - Libera la estructura del mapa
*/
void	free_map(t_map *map)
{
	if (!map)
		return;
	ft_free_str_array(map->grid);
	
	/* Liberar rutas de texturas */
	if (map->no_texture)
		free(map->no_texture);
	if (map->so_texture)
		free(map->so_texture);
	if (map->ea_texture)
		free(map->ea_texture);
	if (map->we_texture)
		free(map->we_texture);
		
	/* Liberar la estructura del mapa */
	free(map);
	printf("🧹 Mapa liberado de memoria\n");
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
	free_mapinfo(game->mapinfo);
	free_map(game->map);
	if (game->player)
		free(game->player);
	if (game->rays)
		free(game->rays);
	i = 0;
	while (i < 4)
	{
		if(game->textures[i])
			mlx_delete_image(game->mlx, game->textures[i]);
		i++;
	}
	mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
	{
		/* En Linux: mlx_destroy_display(game->mlx); */
		//free(game->mlx);
		mlx_terminate(game->mlx);
	}
	printf("✅ Memoria liberada correctamente\n");
}

