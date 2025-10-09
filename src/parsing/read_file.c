/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:14:22 by root              #+#    #+#             */
/*   Updated: 2025/10/09 13:05:21 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Parseo del archivo: que sea formato dup, que se pueda abrir....
 * 
 * Abrir y leer un file.cub -> read()
 * 
 * Parseo:
 * - Que el mapa solo tenga 6 posibles caracteres
 * - Que el mapa esté rodeado de paredes
 * - El contenido del mapa no puede estar separado por una o más líneas,
 * el resto sí
 * - El mapa tiene que estar al final del archivo, el resto de cosas pueden
 * estar en cualquier orden
 * - La infor de cada elemento pued estar separado por uno o varios espacios,
 * el contenido del mapa no
 * - Puede haber espacios en el mapa, se gestionan como queramos.
 * - formato de elementos (N ./path_texture ...)
 *
 * Supongo que quizás meter en una estructura los datos de:
 * - forma del mapa
 * - Starting point del player
 * - orientación del player
 * - textura para paredes (N, S, E, W)
 * - colores del cielo y el suelo
 */

static int	count_file_lines(char *file)
{
	printf("DEBUG: count file lines\n");
	int		fd;
	int		count;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (error_msg(NULL, file, 0));
	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		count ++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

static int	read_file(t_mapinfo *mapinfo, int count_lines)
{
	printf("DEBUG: read_file\n");
	int		row;
	char	*line;
	ssize_t	len;

	mapinfo->file_raw_data = ft_calloc(count_lines + 1, sizeof(char *));
	if (!mapinfo->file_raw_data)
		return (error_msg(ERR_MALLOC, NULL, 0));
	row = 0;
	line = get_next_line(mapinfo->fd);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n') //no guardar saltos de linea
			line[len - 1] = '\0'; //sustituir por end of string
		mapinfo->file_raw_data[row] = ft_strdup(line);
		free(line);
		if (!mapinfo->file_raw_data[row])
			return (error_msg(ERR_MALLOC, NULL, 0));
		line = get_next_line(mapinfo->fd);
		row++;
	}
	mapinfo->file_raw_data[row] = NULL;
	return (1);
}

int load_mapinfo(char *file, t_mapinfo *mapinfo)
{
	int		count_lines;

	mapinfo->fd = open(file, O_RDONLY);
	if (mapinfo->fd < 0) //errno ya está fijado por open
		return (error_msg(NULL, NULL, 0));
	count_lines = count_file_lines(file);
	if (!count_lines)
		return (0);
	if (!read_file(mapinfo, count_lines))
	{
		close(mapinfo->fd);
		return (0);
	}
	mapinfo->cursor = mapinfo->file_raw_data;
	printf("File succesfuly readed\n");
	return (1);
}
