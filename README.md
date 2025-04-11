# Esquema de flujo de MiniRT

Este esquema refleja el orden de ejecución:

## 1. Inicialización
- **main** → Punto de entrada, verifica argumentos
  - **check_file_extension** → Valida que sea archivo .rt
  - **init** → Inicializa MLX y estructura global
  - **init_scene** → Prepara memoria para esferas, planos y cilindros

## 2. Parsing de la escena
- **read_scene** → Procesa el archivo .rt
  - **parse_ambient** → Configura luz ambiental
  - **parse_camera** → Configura cámara (posición, orientación, FOV)
  - **parse_light** → Configura fuentes de luz
  - **parse_objects** → Lee esferas, planos y cilindros

## 3. Proceso de Ray Tracing
- **render** → Coordina el proceso de renderizado
  - **trace_all_rays** → Para cada píxel de la pantalla:
    - **cal_ray** → Calcula un rayo para el píxel
      - **get_ray_direction** → Determina dirección del rayo
      - **find_closest_intersec** → Busca intersecciones con objetos
        - **check_sp_intersecs** → Prueba intersecciones con esferas
        - **check_pl_intersecs** → Prueba intersecciones con planos
        - **check_cy_intersecs** → Prueba intersecciones con cilindros
  - **render_all_pixels** → Colorea píxeles según las intersecciones

## 4. Salida y visualización
- **save_bmp** → Guarda imagen en formato BMP
  - **write_bmp_header** → Escribe encabezado del archivo
  - **write_bmp_row** → Escribe filas de píxeles
- **mlx_put_image_to_window** → Muestra imagen en ventana

## 5. Interacción y cierre
- **set_hooks** → Configura manejadores de eventos
- **mlx_loop** → Inicia bucle principal
- **close_window** → Limpia recursos al cerrar
