
#include "cub3d.h"

//⚠️ REVISAR BIEN QUE SE LIBERE TODO Y QUE SE COMPRUEBE ANTES DE LIBERAR
//NO HAY QUE LIBERAR LOS ARRAYS NO RESERVADOS CON MALLOC...

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

void	free_info(t_game *game)
{
	ft_printf("DEBUG: free_info\n");
	if (!game->info)
		return ;
	ft_free_str_array(game->info->file_raw_data);
	game->info->file_raw_data = NULL;
	free(game->info->f_color_raw); //son producto de ft_strreplace que reserva con malloc
	free(game->info->c_color_raw);
	ft_free_str_array(game->info->map_raw);
	game->info->map_raw = NULL;
	free(game->info);
	game->info = NULL;
}

/*
** Función para liberar el mapa
** - Libera cada línea del grid
** - Libera las rutas de texturas
** - Libera la estructura del mapa
*/
void	free_map(t_game *game)
{
	if (!game->map)
		return;
	ft_free_str_array(game->map->grid);
	/* Liberar rutas de texturas */
	if (game->map->no_texture) //hay que liberar pork se forman con ft_strtrim
		free(game->map->no_texture);
	if (game->map->so_texture)
		free(game->map->so_texture);
	if (game->map->ea_texture)
		free(game->map->ea_texture);
	if (game->map->we_texture)
		free(game->map->we_texture);
	free(game->map);
	game->map = NULL;
}

void	free_minimap(t_game *game)
{
	if (!game->minimap)
		return ;
	if (game->minimap->rays)
		free(game->minimap->rays);
	if (game->minimap->img)
		mlx_delete_image(game->mlx, game->minimap->img);
	free(game->minimap);
	game->minimap = NULL;
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
	free_info(game);
	free_map(game);
	if (game->player)
		free(game->player);
	if (game->rays)
		free(game->rays);
	free_minimap(game);
	i = 0;
	while (i < 4)
	{
		if(game->textures[i])
			mlx_delete_image(game->mlx, game->textures[i]);
		i++;
	}
	if (game->mlx && game->img)
		mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
		mlx_terminate(game->mlx);
	printf("✅ Memoria liberada correctamente\n");
}

