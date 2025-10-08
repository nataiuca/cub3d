/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:33:52 by root              #+#    #+#             */
/*   Updated: 2025/10/08 17:28:49 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

/**
 * archivos texturas: tienen que ser .xml (es lo que 
 * se carga con mlx_xpm_file_to_image()...)
 * validar que:
 *  - existe el archivo
 * 	- es accesible para lectura
 * 	- es un archivo, no un directorio
 * 
 * Valores rgb
 * 
 */

//validate_texture_path


int	validate_rgb_values(char **parts, int color[3])
{
	int	i;
	int	j;
	int	val;

	i = 0;
	while (parts[i])
	{
		j = 0;
		while(parts[i][j])
		{
			if (!ft_isdigit(parts[i][j]))
				return (error_msg(ERR_RGB, ERR_RGB_NUM, 0));
			j ++;
		}
		val = atoi(parts[i]);
		if (val < 0 || val > 255)
			return (error_msg(ERR_RGB, ERR_RGB_RANGE, 0));
		color[i] = val;
		i ++;
	}
	return (1);
}

int	validate_color_str(char *color_str, int color[3])
{
	char	**parts;
	int		ret;
	int		count;

	parts = ft_split(color_str, ' ');
	if (!parts)
		return (error_msg(ERR_MALLOC, NULL, 0)); //error de memoria
	count = 0;
	while (parts[count])
		count ++;
	if (count != 3)
	{
		ft_free_str_array(parts);
		return(error_msg(ERR_RGB, ERR_RGB_COUNT, 0));
	}
	ret = validate_rgb_values(parts, color);
	ft_free_str_array(parts);
	return (ret);
}

int	validate_texture_path(char *path)
{
	char	*ext;
	int		fd;

	if (!path || path[0] == '\0')
		return(error_msg(ERR_TEXTURE_MISSING, NULL, 0));
	//validar extensión (xml)
	ext = ft_strrchr(path, '.');
	ft_printf("DEBUG: extensión del path: %s\n", ext);
	if (!ext || ft_strcmp(ext, ".xmp") != 0 || ext[4] != '\0')
		return(error_msg(ERR_TEXTURE_FORMAT, path, 0));
	//Intentar abrir archivo en modo lectura
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return(error_msg(NULL, path, 0));
	return(0);
}

int	validate_config(t_mapinfo *mapinfo)
{
	printf("DEBUF: validate_config\n");

	if(!validate_texture_path(mapinfo->no_path))
		return (0);
	if(!validate_texture_path(mapinfo->so_path))
		return (0);
	if(!validate_texture_path(mapinfo->ea_path))
		return (0);
	if(!validate_texture_path(mapinfo->we_path))
		return (0);
	if(!validate_color_str(mapinfo->c_color_raw, mapinfo->c_color))
		return (0);
	if(!validate_color_str(mapinfo->f_color_raw, mapinfo->f_color))
		return (0);
	return (1);
}
