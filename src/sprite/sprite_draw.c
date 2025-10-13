/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:22:19 by root              #+#    #+#             */
/*   Updated: 2025/10/13 19:23:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Draws a single column of a sprite onto the screen.
 * 			Iterates over the vertical piexls of a scaled sprite column and
 * 			draws only the visible pixels that are in front of walls and not
 * 			transparent. It maps screen coordinates to the corresponding
 * 			texture coordinates in the sprite frame.
 * 			- Retrieves the current frame of the sprite
 * 			- Iterates over each vertical pixel j from 0 to sprite->height - 1
 * 				- Computes the screen Y coordinate for this row (py) 
 * 				(draw_y: top Y coordinate of the sprite on screen)
 * 			- Checks if the pixel is within the vertical screen bound
 * 			(py >= 0 && py < WIN_HEIGHT) and that the sprite is in front of
 * 			the wall at this screen column (sprite->cam_y z game->rays[px].dist)
 * 				- If both conditions are true, proceed to draw the pixel
 * 			- Compute the corresonding Y coordinate in the sprite texture (tex_y) 
 * 			by proportional mapping
 * 			⚠️ NO SE SI ES NECESARIO LO DE LONG LONG, CHECKEAR TYPE DE MLX_IMAGE_T->HEIGHT
 * 				- Clamp tex_y to [0, frame->height - 1] to avoid out-of-bound
 * 				access
 * 			- Gets the color of the sprite texture at (tex_x, tex_y)
 * 			- Checks the alpha channel of teh color; if it is transparent, 
 * 			doesn't draw it
 * 			- Draw the pixel
 * 
 * @param game	Pointer to the main structure of the game
 * @param sprite	Pointer to the sprite being drawn
 * @param px	Screen X coordinate (column) where this sprite column is drawn
 * @param tex_x	Texture X coordinate corresponding to this column 
 */

static void	draw_sprite_col(t_game *game, t_sprite *sprite, int px, int tex_x)
{
	int			j;
	int			py;
	int			tex_y;
	mlx_image_t	*frame;
	uint32_t	color;

	frame = sprite->frames[sprite->curr_frame];
	j = 0;
	while (j < sprite->height)
	{
		py = sprite->draw_y + j;
		if (py >= 0 && py < WIN_HEIGHT && sprite->cam_y < game->rays[px].dist)
		{
			tex_y = (int)((long long)j * frame->height / sprite->height);
			if (tex_y < 0)
				tex_y = 0;
			if ((uint32_t)tex_y >= frame->height)
				tex_y = frame->height - 1;
			color = get_tex_color(frame, tex_x, tex_y);
			if (((color >> 24) & 0xFF) != 0)
				mlx_put_pixel(game->img, px, py, color);
		}
		j ++;
	}
}

/**
 * @brief	Blits/renders a sclaed sprite image onto the screen with depth and
 * 			transparency checks.
 * 			- Early exit if the frame has zero dimensions or the sprite is
 * 			entirely off-screen
 * 			- Iterate over each horizontal pixel i (0 .. sprite->width - 1) of
 * 			the scaled sprite:
 * 				- Compute screen X coordinate (px)
 * 				- Skip the column if px is outside screen bounds
 * 			- Compute the corresponding texture X coordinate (tex_x) by 
 * 			proportional mapping
 * 				- Clamp tex_x to [0, frame->width - 1] to avoid out-of-bounds
 * 				access
 * 			- Call draw_sprite_col() to draw the column
 * 
 * @param game	Pointer to the game context
 * @param sprite	Pointer to the sprite being drawn
 */

static void	blit_sprite_scaled(t_game *game, t_sprite *sprite)
{
	int			i;
	int			px;
	int			tex_x;
	mlx_image_t	*frame;

	frame = sprite->frames[sprite->curr_frame];
	if (frame->width <= 0 || frame->height <= 0
		|| sprite->draw_x + sprite->width < 0 || sprite->draw_x >= WIN_WIDTH
		|| sprite->draw_y + sprite->height < 0 || sprite->draw_y >= WIN_HEIGHT)
		return ;
	i = 0;
	while (i < sprite->width)
	{
		px = sprite->draw_x + i;
		if (px >= 0 && px < WIN_WIDTH)
		{
			tex_x = (int)((long long)i * frame->width / sprite->width);
			if (tex_x < 0)
				tex_x = 0;
			if ((uint32_t)tex_x >= frame->width)
				tex_x = frame->width - 1;
			draw_sprite_col(game, sprite, px, tex_x);
		}
		i ++;
	}
}

/**
 * @brief	Calculate the on-screen size (width/height) and draw position
 * 			of a sprite based on camera depth.
 * 			- Compute the sprite's width on screen using perspective scaling:
 * 				- the factor 0.75 is a scaling constant specific for our
 * 				sprite images
 * 			- Compute the aspect ratio of the sprite texture
 * 			- Compute the sprite's height on screen while preserving the
 * 			texture's aspect ratio
 * 			- Clamp width and height to prevent execisvelly large sprites
 * 			- Compute horizontal draw position to center the sprite around 
 * 			sprite->screen_x
 * 			- Compute vertical draw position to align the bottom of the
 * 			sprite with the floor (y = 1 in world space)
 * 				- this formula ensures that closer sprites appear lower on 
 * 				screen, simulating perspective
 * 
 * @param sprite	Pointer to the sprite to update
 * @param frame		Pointer to the current sprite texture frame
 */
static void	calc_sprite_size(t_sprite *sprite, mlx_image_t *frame)
{
	double	aspect_ratio;
	double	tmp_cam_y;

	tmp_cam_y = sprite->cam_y;
	if (tmp_cam_y < 0.0001)
		tmp_cam_y = 0.0001;
	sprite->width = (int)((WIN_WIDTH / tmp_cam_y) * 0.75);
	aspect_ratio = (double)frame->height / (double)frame->width;
	sprite->height = (int)(sprite->width * aspect_ratio);
	if (sprite->height > WIN_HEIGHT * 4)
		sprite->height = WIN_HEIGHT * 4;
	if (sprite->width > WIN_WIDTH * 4)
		sprite->width = WIN_WIDTH * 4;
	sprite->draw_x = (int)(sprite->screen_x - sprite->width / 2);
	sprite->draw_y = (WIN_HEIGHT / 2) * (1 + 1 / sprite->cam_y) - (sprite->height);
}

/**
 * @brief	Draws a single sprite on the screen.
 * 			- Checks if the sprite exists
 * 			- Updates the sprite's animation frame based on elapsed time
 * 			- Retrieves the current frame texture of the sprite
 * 			- Projects the sprite into camera space and computes screen-space
 * 			X coordinate
 * 				- Returns if the sprite is behind the camera
 * 			- Calculares the sprite's on-screen size (width/height) and
 * 			top-left position (draw_x, draw_y)
 * 			- Performs scren bounds checks
 * 			- Blits/renders the scaled sprite, performing depth and 
 * 			transparency tests
 * 
 * @param game	Pointer to the game context
 * @param sprite	Pointer to the sprite to draw
 */

void	draw_sprite(t_game *game, t_sprite *sprite)
{
	mlx_image_t	*frame;

	if (sprite->x == 0) //significa que no hay sprite
		return ; //siempre va a haber sprite xk inicializo desde el ppio --> REPASAR ESO! HE CAMBIADO INICIALIZACIONES DE SITIO
	update_sprite(sprite); //actualiza el frame en el que estamos
	frame = sprite->frames[sprite->curr_frame];
	if (!frame)
		return ;
	if (!project_sprite(sprite, game->player))
		return ;
	calc_sprite_size(sprite, frame);
	if (sprite->draw_x + sprite->width < 0 || sprite->draw_x >= WIN_WIDTH)
		return ;
	if (sprite->draw_y + sprite->height < 0 || sprite->draw_y >= WIN_HEIGHT)
		return ;
	blit_sprite_scaled(game, sprite);
}

/**
 * @brief	Draws all sprites in the game
 * 			Iterates over the game's sprite array and calls draw_sprite() for
 * 			each sprite.
 * 
 * @param game	Pointer to the game structure
 */
void	draw_all_sprites(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->sprite_count)
	{
		draw_sprite(game, game->sprite[i]);
		i ++;
	}
}
