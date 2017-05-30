# Infinite-City
School project to create an infinite city. Uploading source so others can benefit.

Created with Visual Studio 2013 using glew for OpenGL. There was code provided by the school that we had to use which is located in the "gsim" directory as well as "ogl_tools.h" and "ogl_tools.cpp".

# Features

## Infinite procedurally generated buildings
The buildings were designed as modular pieces and the peices are randomly assembled at runtime. Buildings are given their unique shapes by effectively using their position as a seed. The appearance of an infinite span of buildings is acheived by rendering buildings inside the frustum with a fog effect applied so you cannot see the end.

## Cars
The cars follow a series of straight and bezier curve paths and take random turns. If you hit 'SPACEBAR' while running the application, you can see a car from first-person view.

## Clouds
The clouds use an experimental shader that ray-traces a cube-shaped volumetric density cloud. The strange appearence of the center of clouds being more invisible than the edges is an intentional design.

## Lighting
The lighting is achieved by applying a common technique called deferred lighting, in which the scene is rendered to a geometry buffer with the channels "color", "normals", and "position". After the render pass, a pass to calculate the lighting data is performed. This is done by approximating the lights as spheres (with backface rendering), and calculating the effect that each light would have on the scene and drawing it to the final backbuffer.

## Boids
The last experimental feature of this demo is the random cubes flying around using a modified version of [Boids](https://en.wikipedia.org/wiki/Boids)

# Contributors
Robert Hewitt - Building Procedure, Cars, Clouds, Lighting [Runouw](https://github.com/runouw)

Kevin Rodriguez - Building design, design, ideas, Boids
