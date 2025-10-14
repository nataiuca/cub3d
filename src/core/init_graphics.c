/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_graphics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:17:02 by root              #+#    #+#             */
/*   Updated: 2025/10/14 10:49:09 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Función para cargar una textura desde un archivo
** - Carga una imagen .png usando mlx_load_png
CREO QUE ESTA FUNCIÓN DEBERIA DEBERIA DE ESTAR EN UN LUGAR MÁS GENÉRICO, PORK TB LA USA SPRITE
*/
mlx_image_t	*load_texture(void *mlx, char *path)
{
	mlx_texture_t	*tex;
	mlx_image_t	*img;

	tex = mlx_load_png(path);
	if (!tex)
	{
		error_msg(ERR_LOAD_TEX, path, 0); //MLX establece su propio errno? MIRAR ERROR_MLX
		return (NULL);
	}
	img = mlx_texture_to_image(mlx, tex);
	if (!img)
	{
		error_msg(NULL, NULL, 0);
		mlx_delete_texture(tex);
	}
	mlx_delete_texture(tex);
	return (img);
}

/*
** Función para cargar todas las texturas del mapa
** - Carga las 4 texturas de las pa redes (N, S, E, W)
** - Las guarda en el array de texturas del juego


*/

int	load_wall_textures(t_game *game)
{
	game->textures[TEX_NO] = load_texture(game->mlx, game->map->no_texture);
	if (!game->textures[0])
		return (0);
	game->textures[TEX_SO] = load_texture(game->mlx, game->map->so_texture);
	if (!game->textures[1])
		return (0);
	game->textures[TEX_EA] = load_texture(game->mlx, game->map->ea_texture);
	if (!game->textures[2])
		return (0);
	game->textures[TEX_WE] = load_texture(game->mlx, game->map->we_texture);
	if (!game->textures[3])
		return (0);
	return (1);
}

/**
 * @brief	Initializes the graphics system and related assets
 * 			- Initializes the mlx libraary and creattes the window via 
 * 			mlx_init()
 * 			- Creates the image buffer uesd for rendering the game
 * 			- Initializes the minimap
 * 			- Loads wall textures
 * 			- Initializes and loads the sprites
 * 			- Places the main image to window
 * 			- Places the minimap image to window
 * 
 * @param game	Pointer to the main game structure
 * @return	1 on success, 0 on failure
 */

int	init_graphics(t_game *game)
{
	int	x;
	int	y;

	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return(error_msg(ERR_MLX_INIT, NULL, 0));
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return(error_msg(NULL, NULL, 0));
	if (!init_minimap(game, game->minimap))
		return (error_msg(ERR_MINIMAP_INIT, NULL, 0));
	if (!load_wall_textures(game))
		return(error_msg(ERR_LOAD_WALL_TEX, NULL, 0));
	if (!init_sprites(game))
		return (error_msg(ERR_SPRITE_INIT, NULL, 0));
	if (!load_all_sprites(game))
		return (error_msg(ERR_LOAD_SPRITE, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_msg(NULL, NULL, 0));
	x = game->minimap->offset_x;
	y =  game->minimap->offset_y;
	if (mlx_image_to_window(game->mlx, game->minimap->img, x, y) < 0)
		return (error_msg(NULL, NULL, 0));
	return (1);
}
