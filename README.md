Enlace MLX42: https://github.com/42-Fundacion-Telefonica/MLX42?tab=readme-ov-file#download-and-build---mlx42  

⚠️  Por ahora estos son los archivos que hay, cualquier otro archivo que haya en el github es de alguna versión antigua y no está compilando en el Makefile - hay que ir borrándolos ⚠️

⚠️ Las funciones que se usen solo dentro del archivo en el que se encuentran, ponerlas static y quitarlas del .h

NORMINETTE + COMENTARIOS estilo doxygen + CORRECTA UBICACIÓN EN EL .H
- cub3d.h
- Makefile
- main.c
controls:
- events.c 
core:
- cleanup_game.c
- cleanup_info.c
- cleanup_sprites.c
- init_game.c
- init_graphics.c
engine:
- raycasting.c
- raycasting_utils.c
- player.c
- moves.c
- collision.c
graphics:
- draw.c ✅
- textures.c
minimap:
- init_minimap.c ✅
- minimap_draw.c ✅
- minimap_utils.c ✅
- minimap_rays.c ✅
parsing:
- read_file.c
- config_parser.c
- map_parser.c
- config_validator.c
- map_validator.c
- parser_utils.c
sprite:
- init_sprite.c
- sprite_load.c
- sprite_draw.c
- sprite_update.c
utils:
- errors.c

