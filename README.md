# Optimized Ray Tracer (miniRT)

## Compilación y Ejecución

### Requisitos del Sistema
- **Linux**: X11, Xext libraries
- **macOS**: OpenGL, AppKit frameworks
- **Compilador**: gcc con soporte para C99
- **Make**: GNU Make o compatible

### Comandos de Compilación

```bash
# Compilación normal
make

# Compilación con AddressSanitizer (para debugging)
make sanitize

# Compilación en modo debug
make debug

# Limpiar archivos objeto
make clean

# Limpiar completamente (archivos objeto + ejecutable)
make fclean

# Recompilar desde cero
make re
```

### Ejecución

```bash
# Ejecutar con archivo de escena
./miniRT scenes/example.rt

# Ejemplo con escena básica
./miniRT scenes/sphere.rt
```

### Formato de Archivos de Escena (.rt)

Los archivos de escena deben seguir este formato:

```
A 0.2 255,255,255                    # Luz ambiente: intensidad R,G,B
C -50,0,20 0,0,1 70                  # Cámara: posición orientación FOV
L -40,0,30 0.7 255,255,255           # Luz: posición intensidad R,G,B

sp 0,0,20 20 255,0,0                 # Esfera: centro radio R,G,B
pl 0,0,0 0,1,0 255,255,255           # Plano: punto normal R,G,B
cy 50,0,20.6 0,0,1.0 14.2 21.42 10,0,255  # Cilindro: centro eje diámetro altura R,G,B
```

### Controles Interactivos

- **Flechas**: Mover cámara (adelante/atrás/izquierda/derecha/arriba/abajo)
- **Espacio/B**: Mover adelante/atrás
- **WASD**: Rotar cámara (pitch/yaw)
- **Q/E**: Rotar cámara (roll)
- **+/-**: Zoom (ajustar campo de visión)
- **Ratón + clic izq**: Mirar alrededor
- **Scroll del ratón**: Zoom
- **P**: Capturar pantalla (guarda como BMP)
- **ESC**: Salir del programa

### Resolución de Problemas

#### Error de librerías en Linux:
```bash
sudo apt-get install libx11-dev libxext-dev
```

#### Error de compilación MLX:
```bash
# Limpiar y recompilar
make fclean
make
```

#### Problemas de permisos:
```bash
chmod +x miniRT
```

# Optimized Ray Tracer (miniRT)

This diagram reflects the actual execution flow after code optimization and cleanup:

## 1. Initialization
- **main** → Entry point, verifies arguments
  - **check_file_extension** → Validates that it's an .rt file
  - **init** → Initializes MLX and global structure
    - **total_pixels** → Calculates and stores (WIN_W - MARGIN) * (WIN_H - MARGIN)
  - **init_scene** → Prepares memory for objects (spheres, planes, cylinders)

## 2. Scene Parsing
- **read_scene** → Processes the .rt file
  - **parse_ambient** → Configures ambient light
  - **parse_cam** → Configures camera (position, orientation, FOV)
  - **parse_light** → Configures light sources
  - **parse_sphere/plane/cylinder** → Reads and configures objects
  - **check_scene** → Verifies that the scene is valid

## 3. Ray Tracing Process
- **render** → Coordinates the rendering process
  - **precal_camera_axis** → Calculates camera's orthonormal basis vectors
  - **precal_rays** → Pre-calculates all ray directions for efficiency
    - **cal_ray_for_pixel** → Calculates ray data for each pixel
    - **get_ray_direction** → Computes world-space ray direction (with FOV caching)
  - **trace_all_rays** → For each screen pixel:
    - **find_closest_isec** → Finds intersections using optimized structure
      - **check_obj_isecs** → Tests intersections with all object types (unified function)
        - **col_sp** → Calculates intersection with spheres
        - **col_pl** → Calculates intersection with planes
        - **col_cy** → Calculates intersection with cylinders
  - **render_all_pixels** → Colors pixels according to intersections
    - **render_single_pixel** → Processes individual pixel
    - **cal_lighting** → Calculates color with illumination
      - **get_surface_normal** → Obtains normal at intersection point
      - **cal_shadow** → Checks if point is in shadow
      - **cal_ambient/diffuse/specular** → Light components
      - **clamp_color** → Ensures RGB values stay within [0-255] range
  - **print_info** → Displays camera and scene statistics (centralized output)

## 4. Output and Visualization
- **new_window** → Creates MLX window and image buffer
- **pixel_put** → Places individual pixels with boundary checking
- **mlx_put_image_to_window** → Displays rendered image
- **set_hooks** → Sets up event handlers
  - **handle_keypress** → Handles keyboard input
    - **handle_movement_keys** → Camera translation (arrow keys, space/B)
    - **handle_rotation_keys** → Camera rotation (WASD, QE)
      - **apply_standard_rotation** → Pitch/yaw rotation
      - **apply_roll_rotation** → Roll rotation
    - **handle_zoom_keys** → Adjusts FOV (+/-)
    - **handle_screenshot** → Saves screenshot (P key)
  - **handle_mouse_move** → Camera rotation with mouse
  - **mouse_press_hook/mouse_release_hook** → Mouse button handling
  - **handle_mouse_scroll** → FOV adjustment with scroll wheel
  - **window_close_handler** → Proper window closing

## 5. Color Operations
- **color_scale** → Multiplies RGB components by a factor
- **color_add** → Combines two colors component-wise
- **get_object_color** → Retrieves base color from intersected object
- **save_ray_state/restore_ray_state** → Ray state preservation for shadows
- **rgb_to_int** → Converts RGB color to integer format

## 6. Event Handling and Finalization
- **mlx_loop** → Starts main event loop
- **finish** → Cleans resources and exits program
  - **free_global** → Frees all allocated memory
    - **free_scene** → Frees scene objects
    - **mlx_destroy_image/window/display** → Cleans MLX resources

## 7. Camera Controls

- **Arrow keys**: Move camera position (forward/backward/left/right/up/down)
- **Space/B**: Move camera position forward/backward
- **WASD**: Rotate camera (pitch/yaw)
- **Q/E**: Roll camera (rotation around viewing direction)
- **+/-**: Zoom in/out (adjusts field of view)
- **Mouse drag**: Look around (with left button pressed)
- **Mouse scroll**: Zoom in/out
- **P**: Take screenshot (saves as BMP)
- **ESC**: Exit program

## 8. Code Optimizations Implemented

- **Unified intersection testing**: Single `check_obj_isecs` function handles all object types
- **Ray precomputation**: All ray directions calculated once in `precal_rays`
- **Centralized pixel counting**: `total_pixels` stored in global structure
- **Memory management**: Proper allocation/deallocation in render cycle
- **Static variables**: Cached calculations in `get_ray_direction` for performance
- **Modular architecture**: Separated concerns into focused modules
- **Dead code elimination**: Removed unused functions and redundant implementations
- **Orthonormal basis vectors**: Camera uses three perpendicular unit vectors for precise orientation
- **Roll angle preservation**: Camera maintains roll orientation between renders

## 9. Project Structure

### Source Files (src/)
- **Core**: `minirt.c`, `init_scene.c`, `render.c`
- **Ray Tracing**: `raytracer_core.c`, `raytracer_render.c`, `raytracer_intersecs.c`
- **Objects**: `raytracer_objects.c`, `raytracer_cylinder.c`, `raytracer_cylinder_caps.c`
- **Graphics**: `raytracer_color.c`, `raytracer_lighting.c`, `raytracer_normals.c`
- **Parsing**: `scene_reader.c`, `parser_scene.c`, `parser_objects.c`, `parser_scene_utils.c`
- **Events**: `events_handle.c`, `events_mouse.c`, `events_rotation.c`, `events_utils.c`
- **Utilities**: `utils.c`, `color.c`, `matrix.c`, `matrix_operations.c`
- **System**: `window.c`, `camera_controls.c`, `freeing.c`, `signal_handlers.c`
- **Export**: `save.c`, `events_screenshot.c`
- **Effects**: `shadows.c`

### Headers (inc/)
- **minirt.h**: Main definitions and constants
- **struct.h**: All data structures
- **prototype.h**: Function prototypes

## 10. Technical Features

- **Dynamic memory management**: Efficient allocation/deallocation
- **Cross-platform compatibility**: Supports Linux, macOS, FreeBSD
- **Interactive camera**: Full 6DOF movement and rotation
- **Real-time rendering**: Optimized for smooth interaction
- **Screenshot capability**: BMP export functionality
- **Robust error handling**: Comprehensive validation and cleanup
- **Mathematical precision**: Proper floating-point comparisons with epsilon
- **Modular design**: Clean separation of responsibilities
- **Centralized information display**: All debug info in `print_info` function

# Mathematical Expressions Used in miniRT

Below are the precise mathematical expressions of the formulas used in the ray tracer:

## 1. Parametric Ray Equation
For a point P(t) on the ray at distance t:
```
P(t) = O + t·D
```
Where:
- O is the ray origin (camera position)
- D is the normalized direction vector
- t is the distance parameter (distance from origin)

## 2. Ray-Sphere Intersection
The intersection is calculated by solving:
```
|D|²·t² + 2D·(O-C)·t + |O-C|² - r² = 0
```
Where:
- O is the ray origin
- D is the ray direction
- C is the sphere center
- r is the sphere radius

## 3. Ray-Plane Intersection
```
t = ((P₀-O)·N) / (D·N)
```
Where:
- P₀ is a point on the plane
- N is the plane normal vector
- O is the ray origin
- D is the ray direction

## 4. Ray-Cylinder Intersection
For the lateral surface, we solve the quadratic equation:
```
a·t² + b·t + c = 0

Where:
a = |Dₚₑᵣₚ|²
b = 2(Dₚₑᵣₚ·OCₚₑᵣₚ)
c = |OCₚₑᵣₚ|² - r²

Dₚₑᵣₚ = D - (D·A)·A
OCₚₑᵣₚ = OC - (OC·A)·A
```
Where:
- A is the normalized axis vector of the cylinder
- OC is the vector from ray origin to cylinder base
- r is the cylinder radius
- Dₚₑᵣₚ and OCₚₑᵣₚ are components perpendicular to the axis

## 5. Rodrigues Rotation Formula
```
v_rot = v·cos(θ) + (k×v)·sin(θ) + k(k·v)(1-cos(θ))
```
Where:
- v is the vector to rotate
- k is the normalized rotation axis
- θ is the rotation angle

## 6. Lighting Calculation

### Ambient Lighting
```
I_a = k_a · C_obj
```

### Diffuse Lighting (Lambert's Law)
```
I_d = k_d · C_obj · max(0, N·L)
```

### Specular Lighting (Phong Model)
```
I_s = k_s · C_white · max(0, V·R)^n
```
Where:
- k_a, k_d, k_s are lighting coefficients
- C_obj is the object color
- N is the surface normal
- L is the vector toward the light source
- V is the vector toward the viewer
- R is the reflection vector
- n is the shininess exponent

## 7. Normalized Vector
```
v_norm = v/|v| = v/sqrt(v_x² + v_y² + v_z²)
```

## 8. Cross Product (Vector)
```
a × b = (a_y·b_z - a_z·b_y, a_z·b_x - a_x·b_z, a_x·b_y - a_y·b_x)
```

## 9. Dot Product (Scalar)
```
a · b = a_x·b_x + a_y·b_y + a_z·b_z
```