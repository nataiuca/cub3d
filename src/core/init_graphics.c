/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_graphics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:17:02 by root              #+#    #+#             */
/*   Updated: 2025/10/14 15:27:43 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Función para cargar una textura desde un archivo
** - Carga una imagen .png usando mlx_load_png
CREO QUE ESTA FUNCIÓN DEBERIA DEBERIA DE ESTAR EN UN LUGAR MÁS GENÉRICO,
	PORK TB LA USA SPRITE
*/
mlx_image_t	*load_texture(void *mlx, char *path)
{
	mlx_texture_t	*tex;
	mlx_image_t		*img;

	// xpm_t	*tex;
	// tex = mlx_load_xpm42(path);
	tex = mlx_load_png(path);
	if (!tex)
	{
		error_msg(ERR_LOAD_TEX, path, 0);
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
 * @brief	se encarga de la interacción con la libreria gráfica
 * 			mlx_init(), mlx_new_img, mlx_image_to_window
 *
 * @param
 */

int	init_graphics(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (error_msg(ERR_MLX_INIT, NULL, 0));
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return (error_msg(NULL, NULL, 0));
	if (!init_minimap(game, game->minimap))
		return (error_msg(ERR_MINIMAP_INIT, NULL, 0));
	if (!load_wall_textures(game))
		return (error_msg(ERR_LOAD_WALL_TEX, NULL, 0));
	if (!init_sprites(game))
		return (error_msg(ERR_SPRITE_INIT, NULL, 0));
	if (!load_all_sprites(game))
		return (error_msg(ERR_LOAD_SPRITE, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_msg(NULL, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->minimap->img,
			game->minimap->offset_x, game->minimap->offset_y) < 0)
		return (error_msg(NULL, NULL, 0));
	game->mouse_rotation_enabled = true;
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	mlx_cursor_hook(game->mlx, handle_mouse_movement, game);
	mlx_mouse_hook(game->mlx, handle_mouse_click, game);
	mlx_set_mouse_pos(game->mlx, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	return (1);
}
