#!/bin/bash

# setup_textures.sh
# Script para configurar las texturas de cub3d

echo "🎮 Configurando texturas para cub3d..."

# Crear directorio de texturas si no existe
if [ ! -d "textures" ]; then
    echo "📁 Creando directorio textures/"
    mkdir textures
fi

# Verificar si existe un archivo XPM de ejemplo
XPM_FILE=""
if [ -f "texture.xpm" ]; then
    XPM_FILE="texture.xpm"
elif [ -f "wall.xpm" ]; then
    XPM_FILE="wall.xpm"
else
    # Buscar cualquier archivo .xpm en el directorio actual
    XPM_FILE=$(ls *.xpm 2>/dev/null | head -1)
fi

if [ -z "$XPM_FILE" ]; then
    echo "❌ No se encontró ningún archivo .xpm en el directorio actual"
    echo "   Por favor, coloca tu archivo XPM en este directorio"
    exit 1
fi

echo "📄 Usando archivo: $XPM_FILE"

# Copiar el archivo XPM para todas las direcciones
echo "📋 Copiando texturas..."
cp "$XPM_FILE" textures/wall_north.xpm
cp "$XPM_FILE" textures/wall_south.xpm
cp "$XPM_FILE" textures/wall_east.xpm
cp "$XPM_FILE" textures/wall_west.xpm

# Establecer permisos correctos
echo "🔧 Estableciendo permisos..."
chmod 644 textures/*.xpm

# Verificar que todo está correcto
echo ""
echo "✅ Configuración completada!"
echo ""
echo "📂 Contenido del directorio textures/:"
ls -la textures/

echo ""
echo "🎯 Ahora puedes ejecutar:"
echo "   ./cub3d maps/map2.cub"