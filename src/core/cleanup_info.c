#include "cub3d.h"

void	ft_free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
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
	free(game->info->f_color_raw);
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
		return ;
	ft_free_str_array(game->map->grid);
	if (game->map->no_texture)
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
