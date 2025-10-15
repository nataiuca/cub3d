/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:47:11 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:47:12 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Cleans up all game resources and safely terminates MLX.
 * 			- Frees info, map, minimap, player, rays, textures, and sprites.
 * 			- Properly closes the MLX instance.
 *
 * @param game	Pointer to the main game structure.
 */
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

/**
 * @brief	Frees the game’s info structure and all its allocated fields.
 *
 * @param game	Pointer to the main game structure.
 */
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

/**
 * @brief	Frees all memory used by the map structure.
 * 			- Releases map grid and texture paths.
 * 			- Finally frees the map structure itself.
 *
 * @param game	Pointer to the main game structure.
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
