/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:33:52 by root              #+#    #+#             */
/*   Updated: 2025/10/09 15:58:09 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"


/*
	- No pueden faltar configuration keys ✅ 
	- Configuratio keys invalidas ✅ 
	- Puede haber espacios y newlines ✅ 
	- Puede haber espacios entre los args de los colores ✅ 
	- Checkear valores de los args de los colores 0-255 y solo digitos ✅ 
	- valores rgb solo pueden ser 3 dígitos ✅ 

	- NO SE SI ES NECESARIO PASAR LOS RGB A INT PORK LUEGO IGUAL HAY QUE CONVERTIR A OTRA COSA
*/

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
	if (!ext || ft_strcmp(ext, ".png") != 0 || ext[4] != '\0')
		return(error_msg(ERR_TEXTURE_FORMAT, path, 0));
	//Intentar abrir archivo en modo lectura
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return(error_msg(NULL, path, 0));
	return(1);
}

/**HABRÁ QUE CAMBIAR COSAS PORK NO SE DONDE ACABARAN GUARDADOS LOS
 * C_COLOR Y TAL
 */

int	validate_config(t_game *game)
{
	printf("DEBUG: validate_config\n");

	if(!validate_texture_path(game->map->no_texture))
		return (0);
	if(!validate_texture_path(game->map->so_texture))
		return (0);
	if(!validate_texture_path(game->map->ea_texture))
		return (0);
	if(!validate_texture_path(game->map->we_texture))
		return (0);
	if(!validate_color_str(game->mapinfo->c_color_raw, game->mapinfo->c_color))
		return (0);
	if(!validate_color_str(game->mapinfo->f_color_raw, game->mapinfo->f_color))
		return (0);
	return (1);
}
