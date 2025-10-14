#include "cub3d.h"

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
void	free_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->textures[i])
		{
			mlx_delete_image(game->mlx, game->textures[i]);
			game->textures[i] = NULL;
		}
		i++;
	}
	if (game->img)
	{
		mlx_delete_image(game->mlx, game->img);
		game->img = NULL;
	}
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	free_info(game);
	free_map(game);
	free_minimap(game);
	if (game->player)
		free(game->player);
	if (game->rays)
		free(game->rays);
	free_textures(game);
	free_sprite(game);
	if (game->mlx)
	{
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}
