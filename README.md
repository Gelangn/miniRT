# Esquema de flujo optimizado de MiniRT

Este esquema refleja el orden de ejecución con las optimizaciones implementadas:

## 1. Inicialización
- **main** → Punto de entrada, verifica argumentos
  - **check_file_extension** → Valida que sea archivo .rt
  - **init** → Inicializa MLX y estructura global
  - **init_scene** → Prepara memoria para objetos (esferas, planos, cilindros)

## 2. Parsing de la escena
- **read_scene** → Procesa el archivo .rt
  - **parse_ambient** → Configura luz ambiental
  - **parse_cam** → Configura cámara (posición, orientación, FOV)
  - **parse_light** → Configura fuentes de luz
  - **parse_sphere/plane/cylinder** → Lee y configura objetos
  - **check_scene** → Verifica que la escena sea válida

## 3. Proceso de Ray Tracing
- **render** → Coordina el proceso de renderizado
  - **trace_all_rays** → Para cada píxel de la pantalla:
    - **cal_ray** → Calcula un rayo para el píxel
      - **global->c_ray.origin = cam.pos** → Almacena origen en estructura global
      - **global->c_ray.dir = get_ray_direction()** → Almacena dirección en estructura global
      - **find_closest_intersec** → Busca intersecciones usando la estructura global
        - **check_sp_intersecs** → Prueba intersecciones con esferas
          - **col_sp** → Calcula intersección con una esfera
        - **check_pl_intersecs** → Prueba intersecciones con planos
          - **col_pl** → Calcula intersección con un plano
        - **check_cy_intersecs** → Prueba intersecciones con cilindros
          - **col_cy** → Calcula intersección con un cilindro
  - **render_all_pixels** → Colorea píxeles según las intersecciones
    - **cal_lighting** → Calcula color con iluminación
      - **get_surface_normal** → Obtiene normal en punto de intersección
      - **Guarda/restaura estado del rayo** → Para cálculos de sombra

## 4. Salida y visualización
- **mlx_put_image_to_window** → Muestra imagen inicial en ventana
- **set_hooks** → Configura manejadores de eventos
  - **handle_keypress** → Maneja teclas para movimiento y guardar imagen
    - **move_camera/rotate_camera** → Actualiza cámara
    - **save_bmp** → Guarda captura de pantalla (tecla P)
  - **mouse_press_hook/mouse_release_hook** → Control de ratón
  - **handle_mouse_move** → Rotación de cámara con ratón
  - **window_close_handler** → Cierre correcto de ventana

## 5. Manejo de eventos y finalización
- **mlx_loop** → Inicia bucle principal de eventos
- **finish** → Limpia recursos y sale del programa
  - **free_global** → Libera toda la memoria asignada
    - **free_scene** → Libera objetos de la escena
    - **mlx_destroy_image/window/display** → Limpia recursos de MLX

## 6. Optimizaciones implementadas
- **Estructura global centralizada** → Almacena rayo actual
- **Parámetros simplificados** → Funciones de colisión usan t_global
- **Sistema de guardado a petición** → Capturas cuando el usuario lo solicite
- **Movimiento fluido de cámara** → Controles de movimiento mejorados