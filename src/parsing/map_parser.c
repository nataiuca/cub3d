/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:13:09 by root              #+#    #+#             */
/*   Updated: 2025/10/08 16:37:00 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* 
	- El mapa tiene que ir después de las configuraciones de texturas y colores
	- No puede haber líneas vacías dentro del mapa  -> ESTA IMPLEMENTACIÓN
	INTERPRETA LÍNEA VACÍA COMO FIN DE MAPA!
	- EL mapa debe parsearse con los espacios que tenga dentro ✅
		- no sé cómo interpretar los espacios, si como chars inválidos o qué ❌
		- los espacios al inicio o final de una linea del mapa también se conservan 
	- El mapa tiene que estar rodeado de paredes (1) ✅
	- Solo puede tener los caracteres 0, 1, N, S, E, W ✅
	- Solo puede haber un player ✅
	- Tiene que haber un player ✅
	- El mapa tiene que ser rectangular ✅
	- No puede haber ninguna línea escrita más después del mapa ✅

		1 - wall
		0 - empty space
		N, S, E, W - player start pos + orientation
*/

int	count_rows(char **array)
{
	int	count;

	count = 0;
	while (array[count])
	{
		if (!*array[count]) //linea vacia -> fin de mapa
			break ;
		count ++;
	}
	return (count);
}

int	parse_map(t_mapinfo *mapinfo)
{
	printf("DEBUF: parse_map\n");
	int		rows;
	int		i;

	rows = count_rows(mapinfo->cursor); //el cursor ahora mismo queda al final del mapa
	mapinfo->map_raw = calloc(rows + 1, sizeof(char *));
	if (!mapinfo->map_raw)
		return (error_msg(ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < rows)
	{
		mapinfo->map_raw[i] = ft_strdup(mapinfo->cursor[i]);
		if (!mapinfo->map_raw[i])
			return(error_msg(ERR_MALLOC, NULL, 0));
		i ++;	
	}
	mapinfo->map_raw[rows] = NULL;
	return (1);
}
