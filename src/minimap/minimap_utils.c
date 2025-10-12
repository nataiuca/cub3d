/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:13:26 by root              #+#    #+#             */
/*   Updated: 2025/10/12 17:53:24 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_minimap(t_minimap *minimap)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < minimap->img->height)
	{
		x = 0;
		while (x < minimap->img->width)
		{
			mlx_put_pixel(minimap->img, x, y, 0x00000000);
			x ++;
		}
		y ++;
	}
}

int	get_minimap_cell_color(char cell)
{
	if (cell == '1') //wall
		return (0xFF7F50FF); //coral
	else if (cell == '0') //floor
		return (0x556B2FFF); //kaki
	else if (ft_strchr("NSEWC", cell))
		return (0x556B2FFF); //kaki para el suelo
	return (0x222222FF); //dark grey - vacíos
}


