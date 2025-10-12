/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:04:35 by root              #+#    #+#             */
/*   Updated: 2025/10/12 17:43:53 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_sprite(t_sprite *sprite)
{
	sprite->frames = NULL;
	sprite->frame_count = 6;
	sprite->curr_frame = 0;
	sprite->last_update = 0.0;
	sprite->frame_time = 0.5; //elegir numero segun lo que tenga sentido
	sprite->x = 0;
	sprite->y = 0;
	sprite->cam_x = 0;
	sprite->cam_y = 0;
	sprite->screen_x = 0;
	sprite->height = 0;
	sprite->width = 0;
}

int	load_sprite(t_game *game, t_sprite *sprite)
{
	int			i;
	char		*sprite_paths[sprite->frame_count];

	sprite->frames = malloc(sizeof(mlx_image_t *)  *sprite->frame_count);
	if (!sprite->frames)
		return(error_msg(NULL, NULL, 0));
	sprite_paths[0] = "./sprites/cat_1.png";
	sprite_paths[1] = "./sprites/cat_2.png";
	sprite_paths[2] = "./sprites/cat_3.png";
	sprite_paths[3] = "./sprites/cat_4.png";
	sprite_paths[4] = "./sprites/cat_5.png";
	sprite_paths[5] = "./sprites/cat_6.png";
	i = 0;
	while (i < sprite->frame_count)
	{
		sprite->frames[i] = load_texture(game->mlx, sprite_paths[i]);
		if (!sprite->frames[i])
		{
			while (--i >= 0)
				mlx_delete_image(game->mlx, sprite->frames[i]);
			free(sprite->frames);
			return (0);
		}
		i ++;
	}
	return (1);
}

double	get_time(void)
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
 * @brief	Proyecta las coordenadas del sprite en el espacio de la cámara
 * 			Devuelve 0 si sprite no está delante, 1 si todo ok
 */

static int	project_sprite(t_game *game, t_sprite *sprite)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	det;

	sprite_x = sprite->x - game->player->pos_x;
	sprite_y = sprite->y - game->player->pos_y;
	det = (game->player->plane_x * game->player->dir_y - game->player->dir_x * game->player->plane_y);
	if (fabs(det) < 1e-9)
		return (0);
	inv_det = 1.0 / det;
	sprite->cam_x = inv_det * (game->player->dir_y * sprite_x - game->player->dir_x * sprite_y);
	sprite->cam_y = inv_det * (-game->player->plane_y * sprite_x + game->player->plane_x * sprite_y);
	if (sprite->cam_y <= 0.0)
		return (0);
	sprite->screen_x = (WIN_WIDTH / 2.0) * (1.0 + sprite->cam_x / sprite->cam_y);
	return (1);
}

/**
 * @brief	Dibuja la textura 'frame' escalada a sprite_w x sprite_h en game->img
 * 			en la posición draw_x, draw_y, manteniendo transparencias
 */

static void	blit_sprite_scaled(t_game *game, t_sprite *sprite, mlx_image_t *frame)
{
	int	i;
	int	j;
	int	px;
	int	py;
	int	tex_x;
	int	tex_y;
	uint32_t color;

	if (frame->width <= 0 || frame->height <= 0)
		return ;
	if (sprite->draw_x + sprite->width < 0 || sprite->draw_x >= WIN_WIDTH)
		return ;
	if (sprite->draw_y + sprite->height < 0 || sprite->draw_y >= WIN_HEIGHT)
		return ;
	i = 0;
	while (i < sprite->width)
	{
		px = sprite->draw_x + i;
		if (px < 0 || px >= WIN_WIDTH)
		{
			i ++;
			continue ;
		}
		tex_x = (int)((long long)i * frame->width / sprite->width);
		if (tex_x < 0)
			tex_x = 0;
		if ((uint32_t)tex_x >= frame->width)
			tex_x = frame->width - 1;
		j = 0;
		while (j < sprite->height)
		{
			py = sprite->draw_y + j;
			if (py < 0 || py >= WIN_HEIGHT)
			{
				j ++;
				continue ;
			}
			if (sprite->cam_y >= game->rays[px].perp_wall_d)
			{
				j ++;
				continue ;
			}
			tex_y = (int)((long long)j * frame->height / sprite->height);
			if (tex_y < 0)
				tex_y = 0;
			if ((uint32_t)tex_y >= frame->height)
				tex_y = frame->height - 1;
			color = get_tex_color(frame, tex_x, tex_y);
			if (((color >> 24) & 0xFF) != 0)
				mlx_put_pixel(game->img, px, py, color);
			j ++;
		}
		i ++;
	}
}
 
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
	sprite->draw_y = (WIN_HEIGHT / 2) - (sprite->height / 2);
}

void	draw_sprite(t_game *game, t_sprite *sprite)
{
	printf("DEBUG: draw sprite\n");
	mlx_image_t	*frame;

	if (sprite->x == 0) //significa que no hay sprite
		return ; //siempre va a haber sprite xk inicializo desde el ppio
	update_sprite(sprite);
	frame = sprite->frames[sprite->curr_frame];
	if (!frame)
		return ;
	if (!project_sprite(game, sprite))
		return ;
	
	calc_sprite_size(sprite, frame);
	if (sprite->draw_x + sprite->width < 0 || sprite->draw_x >= WIN_WIDTH)
		return ;
	if (sprite->draw_y + sprite->height < 0 || sprite->draw_y >= WIN_HEIGHT)
		return ;
	blit_sprite_scaled(game, sprite, frame);
}
