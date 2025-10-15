/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:04:35 by root              #+#    #+#             */
/*   Updated: 2025/10/13 19:22:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((double)tv.tv_sec + (double)tv.tv_usec / 1e6);
}
//actualizar sprite
//solo puedo usar gettimeofday

void	update_sprite(t_sprite *sprite)
{
	double	now;

	now = get_time();
	if (now - sprite->last_update >= sprite->frame_time)
    {
		sprite->curr_frame = (sprite->curr_frame + 1) % sprite->frame_count;
		sprite->last_update = now;
	}
}

/**
 * @brief	TRANSFORMA COORDENADAS DE SPRITE A COORDENADAS RELATIVAS A
 * 			CÁMARA
 * 			Projects a world-space sprite into camera (view) space and computes
 * 			its screen X.
 * 			Transforms the sprite's world coordinates into the camera 
 * 			coordinate system used for rendering, computes whether the sprite is
 * 			in front of the camera, and calculates the horizontal screen
 * 			position where the sprite should be drawn.
 * 
 * 					⚠️ QUÉ SON WORLD COORDINATES??? REVISAR EXPLICACIÓN Y ENTENDER BIEN
 * 					NECESITO DIBUJO EXPLICATIVO DE ESTO !!!
 * 			- Compute the vector from the player to the sprite in world
 * 			coordinates:
 * 				sprite_x = sprite->x - player->pos_x
 * 				sprite_y = sprite->y - player->pos_y
 *			- Compute the 2+2 determinant (det) of the camera transformation
 *			matrix. This det is used to invert the matrix [dir_x plane_x; dir_y
 *			plane_y]. If det is nearly 0, the camera bases vectors are 	
 *			degenerate and the projection cannot be performed realiably. 
 *			- Invert the det (inv_det) to obtain the matrix inverse scalar 
 *			- Transform the sprite vector into camera coordinates:
 * 				cam_x: horizontal pos in camera space (positive = to the right
 * 				of the camera)
 * 				cam_y: depth / distance from the camera along the view direction
 * 				(positive = in front)
 * 			- If cam_y <= 0.0 the sprite lies behind the camera, the function
 * 			returns 0 (not visible)
 * 			- Compute the horizontal screen coordinate (screen_x) where the
 * 			sprite's center should appear. Thismaps camera-space X normalized
 * 			by depth into pixel coordinates:
 * 				- when cam_x == 0 the sprite is centered and 
 * 				screen_x == WIN_WIDTH / 2
 * 				- the formula applies the common perspective divisio (x/z) and
 * 				shifts/scales to the screen pixel range [0, WIN_WIDTH]
 * 
 * @param game	Pointer to the main game structure
 * @param sprite	Pointer to the sprite structure. On succes, the function 
 * 					sets:
 * 					- cam_x/y: sprite X and Y in camera space (relative
 * 					horizontal and depth)
 * 					- screen_x: horizontal pixel coordinate on the screen
 * @param player	Pointer to the player / camera structure
 * 					- pos_x/y: player world position
 * 					- dir_x/y: normalized view vector
 * 					- plane_x/y: camera plane (perpendicular to dir) used to 
 * 					build the camera coordinate basis
 * @return	1 on succes and if the sprite is in front of the camera, 0 if the
 * 			projection cannot be computed (near-zero determinant) or if the 
 * 			sprite is behind the camera
 */

int	project_sprite(t_sprite *sprite, t_player *player)
{
	double		sprite_x;
	double		sprite_y;
	double		inv_det;
	double		det;

	sprite_x = sprite->x - player->pos_x;
	sprite_y = sprite->y - player->pos_y;
	det = (player->plane_x * player->dir_y - player->dir_x * player->plane_y);
	if (fabs(det) < 1e-9)
		return (0);
	inv_det = 1.0 / det;
	sprite->cam_x = inv_det * (player->dir_y * sprite_x
			- player->dir_x * sprite_y);
	sprite->cam_y = inv_det * (-player->plane_y * sprite_x
			+ player->plane_x * sprite_y);
	if (sprite->cam_y <= 0.0)
		return (0);
	sprite->screen_x = (WIN_WIDTH / 2.0)
		* (1.0 + sprite->cam_x / sprite->cam_y);
	return (1);
}
