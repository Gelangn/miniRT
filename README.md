# Optimized Ray Tracer (miniRT)

This diagram reflects the execution flow with all implemented optimizations:

## 1. Initialization
- **main** → Entry point, verifies arguments
  - **check_file_extension** → Validates that it's an .rt file
  - **init** → Initializes MLX and global structure
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
  - **precal_rays** → Pre-calculates all rays directions for efficiency
  - **trace_all_rays** → For each screen pixel:
    - **find_closest_intersec** → Finds intersections using global structure
      - **check_obj_intersecs** → Tests intersections with all object types
        - **col_sp** → Calculates intersection with a sphere
        - **col_pl** → Calculates intersection with a plane
        - **col_cy** → Calculates intersection with a cylinder
  - **render_all_pixels** → Colors pixels according to intersections
    - **cal_lighting** → Calculates color with illumination
      - **get_surface_normal** → Obtains normal at intersection point
      - **cal_shadow** → Checks if point is in shadow
      - **cal_ambient/diffuse/specular** → Light components

## 4. Output and Visualization
- **mlx_put_image_to_window** → Displays initial image in window
- **set_hooks** → Sets up event handlers
  - **handle_keypress** → Handles keys for movement and saving image
    - **handle_movement_keys** → Camera translation (arrow keys)
    - **handle_rotation_keys** → Camera rotation (WASD, QE)
    - **handle_zoom_keys** → Adjusts FOV (+/-)
    - **handle_screenshot** → Saves screenshot (P key)
  - **handle_mouse_move** → Camera rotation with mouse
  - **window_close_handler** → Proper window closing

## 5. Event Handling and Finalization
- **mlx_loop** → Starts main event loop
- **finish** → Cleans resources and exits program
  - **free_global** → Frees all allocated memory
    - **free_scene** → Frees scene objects
    - **mlx_destroy_image/window/display** → Cleans MLX resources

## 6. Mathematical Formulas Used

### Ray Equation
```
Ray(t) = Origin + t * Direction
```

### Ray-Sphere Intersection
```
(Direction·Direction)t² + 2(Origin-Center·Direction)t + (Origin-Center)·(Origin-Center) - radius² = 0
```

### Ray-Plane Intersection
```
t = ((PlanePoint - RayOrigin)·PlaneNormal) / (RayDirection·PlaneNormal)
```

### Ray-Cylinder Intersection
For the lateral surface:
```
a = |Direction_perpendicular|²
b = 2(Direction_perpendicular·OriginToCenter_perpendicular)
c = |OriginToCenter_perpendicular|² - radius²
discriminant = b² - 4ac
```

### Vector Rotation (Rodrigues Formula)
```
v_rot = v*cos(θ) + (k×v)*sin(θ) + k(k·v)(1-cos(θ))
```
Where:
- v is the vector to rotate
- k is the normalized axis of rotation
- θ is the angle of rotation

### Lighting Calculation
- **Ambient**: `Color * AmbientIntensity`
- **Diffuse**: `Color * LightIntensity * max(0, Normal·LightDirection)`
- **Specular**: `White * LightIntensity * max(0, ViewDirection·ReflectionDirection)^shininess`

## 7. Camera Controls

- **Arrow keys**: Move camera position (forward/backward/left/right)
- **Space/B**: Move camera position up/down
- **WASD**: Rotate camera (pitch/yaw)
- **Q/E**: Roll camera (rotation around viewing direction)
- **+/-**: Zoom in/out (adjusts field of view)
- **Mouse drag**: Look around
- **P**: Take screenshot
- **ESC**: Exit program

## 8. Advanced Technical Features

- **Orthonormal basis vectors**: Camera uses three perpendicular unit vectors (right_axis, up_axis, forward_axis) for precise orientation
- **Ray precomputation**: All ray directions are calculated once for efficiency
- **Roll angle preservation**: Camera maintains roll orientation between renders
- **Dynamic resolution**: Supports different window sizes (1024x768 recommended)
- **Robust intersection testing**: Handles edge cases for all geometric primitives
- **BMP screenshot capability**: Saves renders as bitmap images

# Expresiones Matemáticas Utilizadas en MiniRT

Aquí están las expresiones matemáticas precisas de las fórmulas utilizadas en el ray tracer:

## 1. Ecuación Paramétrica del Rayo
Para un punto $P(t)$ en el rayo a una distancia $t$:
$$P(t) = O + t \cdot \vec{D}$$
Donde:
- $O$ es el origen del rayo (posición de la cámara)
- $\vec{D}$ es el vector dirección normalizado
- $t$ es el parámetro de distancia (distancia desde el origen)

## 2. Intersección Rayo-Esfera
La intersección se calcula resolviendo:
$$|\vec{D}|^2 t^2 + 2\vec{D} \cdot (\vec{O} - \vec{C})t + |\vec{O} - \vec{C}|^2 - r^2 = 0$$
Donde:
- $\vec{O}$ es el origen del rayo
- $\vec{D}$ es la dirección del rayo
- $\vec{C}$ es el centro de la esfera
- $r$ es el radio de la esfera

## 3. Intersección Rayo-Plano
$$t = \frac{(\vec{P_0} - \vec{O}) \cdot \vec{N}}{\vec{D} \cdot \vec{N}}$$
Donde:
- $\vec{P_0}$ es un punto en el plano
- $\vec{N}$ es el vector normal del plano
- $\vec{O}$ es el origen del rayo
- $\vec{D}$ es la dirección del rayo

## 4. Intersección Rayo-Cilindro
Para la superficie lateral, resolvemos la ecuación cuadrática:
$$at^2 + bt + c = 0$$

Donde:
$$a = |\vec{D_{perp}}|^2$$
$$b = 2(\vec{D_{perp}} \cdot \vec{OC_{perp}})$$
$$c = |\vec{OC_{perp}}|^2 - r^2$$
$$\vec{D_{perp}} = \vec{D} - (\vec{D} \cdot \vec{A})\vec{A}$$
$$\vec{OC_{perp}} = \vec{OC} - (\vec{OC} \cdot \vec{A})\vec{A}$$

- $\vec{A}$ es el vector eje normalizado del cilindro
- $\vec{OC}$ es el vector desde el origen del rayo hasta la base del cilindro
- $r$ es el radio del cilindro
- $\vec{D_{perp}}$ y $\vec{OC_{perp}}$ son las componentes perpendiculares al eje

## 5. Fórmula de Rotación de Rodrigues
$$\vec{v}_{rot} = \vec{v}\cos(\theta) + (\vec{k} \times \vec{v})\sin(\theta) + \vec{k}(\vec{k} \cdot \vec{v})(1-\cos(\theta))$$

Donde:
- $\vec{v}$ es el vector a rotar
- $\vec{k}$ es el eje de rotación normalizado
- $\theta$ es el ángulo de rotación

## 6. Cálculo de Iluminación

### Iluminación Ambiente
$$I_a = k_a \cdot C_{obj}$$

### Iluminación Difusa (Ley de Lambert)
$$I_d = k_d \cdot C_{obj} \cdot \max(0, \vec{N} \cdot \vec{L})$$

### Iluminación Especular (Modelo de Phong)
$$I_s = k_s \cdot C_{white} \cdot \max(0, \vec{V} \cdot \vec{R})^n$$

Donde:
- $k_a$, $k_d$, $k_s$ son los coeficientes de iluminación
- $C_{obj}$ es el color del objeto
- $\vec{N}$ es el vector normal de la superficie
- $\vec{L}$ es el vector hacia la fuente de luz
- $\vec{V}$ es el vector hacia el observador
- $\vec{R}$ es el vector de reflexión
- $n$ es el exponente de brillo (shininess)

## 7. Vector Normalizado
$$\vec{v}_{norm} = \frac{\vec{v}}{|\vec{v}|} = \frac{\vec{v}}{\sqrt{v_x^2 + v_y^2 + v_z^2}}$$

## 8. Producto Cruz (Vector)
$$\vec{a} \times \vec{b} = \begin{pmatrix} a_y b_z - a_z b_y \\ a_z b_x - a_x b_z \\ a_x b_y - a_y b_x \end{pmatrix}$$

## 9. Producto Punto (Escalar)
$$\vec{a} \cdot \vec{b} = a_x b_x + a_y b_y + a_z b_z$$