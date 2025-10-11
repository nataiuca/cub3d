/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:33:52 by root              #+#    #+#             */
/*   Updated: 2025/10/11 15:59:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Converts RGB componentes to a 32-bit int (RGBA 0xRRGGBBAA format),
 * 			setting alpha to full opacity (0xFF)
 * 
 * @param r	Red component (0-255)
 * @param g	Green component (0-255)
 * @param b	Blue component (0-255)
 * @return	Combined 0xRRGGBBAA integer with full opacity
 */

static int	rgb_to_int(int r, int g, int b)
{
	int	a;

	a = 0xFF;
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

/**
 * @brief	Validates RGB numeric values and converts them into a 32-bit color.
 * 			Iterates through the given string array of RGB componentes,
 * 			checking:
 * 			- Each character is a valid digit
 * 			- Each numeric value is within the valid range [0, 255]
 * 			If all checks pass, the three components are converted to a 
 * 			0xRRGGBBAA int via rgb_to_int()
 * 			If any error occurs, an error message is displayed and returns 0
 * 
 * @param parts	Array of string containing RGB components
 * @param color	Pointer to store the resulting 32-bit int color
 * @return	1 on success, 0 on failure (invalid digit or out of range)
 */

static int	validate_rgb_values(char **parts, uint32_t *color)
{
	int	i;
	int	j;
	int	val;
	int	rgb[3];

	i = 0;
	while (parts[i])
	{
		j = 0;
		while (parts[i][j])
		{
			if (!ft_isdigit(parts[i][j]))
				return (error_msg(ERR_RGB, ERR_RGB_NUM, 0));
			j ++;
		}
		val = atoi(parts[i]);
		if (val < 0 || val > 255)
			return (error_msg(ERR_RGB, ERR_RGB_RANGE, 0));
		rgb[i] = val;
		i ++;
	}
	*color = rgb_to_int(rgb[0], rgb[1], rgb[2]);
	return (1);
}

/**
 * @brief	Validates an RGB color string and converts it into a 32-bit integer.
 * 			- Splits the input into three integer values (R, G, B)
 * 			- Ensures there are exactly three values
 * 			- Validates rgb values and combines them into a single 0xRRGGBBAA
 * 			int via validate_rgb_values()
 * 			- Frees the splitted array before returning
 * 	
 * @param color_str	The string containing the RGB values separated by spaces
 * @param color	Pointer to store the final 32-bit color value
 * @return 1 on success, 0 on failure	
 */

static int	validate_color_str(char *color_str, uint32_t *color)
{
	char	**parts;
	int		ret;
	int		count;

	parts = ft_split(color_str, ' ');
	if (!parts)
		return (error_msg(NULL, NULL, 0));
	count = 0;
	while (parts[count])
		count ++;
	if (count != 3)
	{
		ft_free_str_array(parts);
		return (error_msg(ERR_RGB, ERR_RGB_COUNT, 0));
	}
	ret = validate_rgb_values(parts, color);
	ft_free_str_array(parts);
	return (ret);
}

/**
 * @brief	Validates a texture file path.
 * 			Ensures that the given texture path:
 * 			- Is not empty or NULL
 * 			- Has a valid ".png" file extension
 * 			- Points to an accesible file (via open in read mode)
 * 
 * @param path	The path to the texture file
 * @return	1 on succes, 0 on failure
 */
static int	validate_texture_path(char *path)
{
	char	*ext;
	int		fd;

	if (!path || path[0] == '\0')
		return (error_msg(ERR_TEXTURE_MISSING, NULL, 0));
	ext = ft_strrchr(path, '.');
	if (!ext || ft_strcmp(ext, ".png") != 0 || ext[4] != '\0')
		return (error_msg(ERR_TEXTURE_FORMAT, path, 0));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg(NULL, path, 0));
	return (1);
}

/**
 * @brief	Validates all configuration data after parsing.
 * 			Checks the validity of:
 * 			- All texture paths
 * 			- Ceiling and floor RGB color values
 * 
 * @param game	Pointer to the main game structure 
 * @return	1 on succes, 0 on failure
 */

int	validate_config(t_game *game)
{
	if (!validate_texture_path(game->map->no_texture))
		return (0);
	if (!validate_texture_path(game->map->so_texture))
		return (0);
	if (!validate_texture_path(game->map->ea_texture))
		return (0);
	if (!validate_texture_path(game->map->we_texture))
		return (0);
	if (!validate_color_str(game->info->c_color_raw, &game->map->ceiling_color))
		return (0);
	if (!validate_color_str(game->info->f_color_raw, &game->map->floor_color))
		return (0);
	return (1);
}
