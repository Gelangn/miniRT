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