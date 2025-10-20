Enlace MLX42: https://github.com/42-Fundacion-Telefonica/MLX42?tab=readme-ov-file#download-and-build---mlx42  

⚠️  Por ahora estos son los archivos que hay, cualquier otro archivo que haya en el github es de alguna versión antigua y no está compilando en el Makefile - hay que ir borrándolos ⚠️

⚠️ Las funciones que se usen solo dentro del archivo en el que se encuentran, ponerlas static y quitarlas del .h

NORMINETTE + COMENTARIOS estilo doxygen + CORRECTA UBICACIÓN EN EL .H
- cub3d.h ✅
- Makefile✅
- main.c ✅

controls:
- events.c ✅
 
core:
- cleanup_game.c ✅
- cleanup_utils.c ✅
- init_game.c ✅ 
- init_graphics.c ✅

engine:
- raycasting.c ✅ 
- raycasting_utils.c ✅
- player.c ✅ 
- moves.c ✅ 
- collision.c ✅

graphics:
- draw.c ✅
- textures.c ✅

minimap:
- init_minimap.c ✅
- minimap_draw.c ✅
- minimap_utils.c ✅
- minimap_rays.c ✅

parsing:
- config_parser.c ✅
- config_validator.c ✅
- map_parser.c ✅ 
- map_validator.c ✅ 
- parser_utils.c ✅ 
- read_file.c ✅

sprite:
- init_sprite.c ✅
- sprite_load.c ✅
- sprite_draw.c ✅
- sprite_update.c ✅

utils:
- errors.c ✅

## Debugging

**Compile with flags:**

    -g -fsanitize=address -fno-omit-frame-pointer

**Execute:**

    make
    export ASAN_OPTIONS="verbosity=2:detect_leaks=1"
    ./cub3D ...

- detect_leaks=1: detecta fugas de memoria que quedan sin liberar al finalizar el programa
- verbosity=2: imprime mensajes internos adicionales, cuando malloc/free son interceptados, cuando LSan realiza barrido, infor sobre memoria...

⚠️ IMPORTANTE! SI COMENTAMOS EL CLEANUP() DEL MAIN PARA PROBAR SI DETECTA LEAKS O NO, TENEMOS QUE CERRAR EL JUEGO CON LA CRUZ DE LA VENTANA, NO CON ESCAPE, PORQUE ESCAPE HACE SU PROPIO CLEANUP, NO DARÁ LEAKS  

⚠️ IMPORTANTE! NO USAR VALGRIND Y FSANITIZER A LA VEZ

--> Cómo detectar fds abiertos sin valgrind? 
- Ejecutar programa en segundo plano y capturar PID

     ./cub3D ... &
     echo $!

- Ver qué archivos tiene abiertos el proceso (mientras está runeando, ejecutar en otra terminal):

      ls -l /proc/<pid>/fd

Pero esto no te dice cuáles quedan abiertos una vez termina el programa... hay que hacer cosas en el main

  Errores con <unknown module> o sin rutas tipo /home/user/cub3D... significa que el leak no está en tu código compilado con símbolos de depuración, sino en una librería precompilada o sin símbolos
