# Infinite-City
![Infinite City Pic 1](https://github.com/runouw/Infinite-City/blob/master/demo_img/demo1.jpg)

[See on Youtube](https://www.youtube.com/watch?v=AiF9kTjQlr4)

School project to create an infinite city. Uploading source so others can benefit.

Created with Visual Studio 2013 using glew for OpenGL. There was code provided by the school that we had to use, which is located in the "gsim" directory as well as "ogl_tools.h" and "ogl_tools.cpp".

# Features

## Infinite procedurally generated buildings
The buildings were designed as modular pieces and the pieces are randomly assembled at runtime. Buildings are given their unique shapes by effectively using their position as a seed. The appearance of an infinite span of buildings is achieved by rendering buildings inside the frustum with a fog effect applied so you cannot see the end.

## Cars
The cars follow a series of straight lines and bezier curves while also taking random turns. If you hit 'SPACEBAR' while running the demo, you can see a car's first-person view.

## Clouds
The clouds use an experimental shader that ray-traces a cube-shaped volumetric density cloud. The strange appearence of the center of clouds being more invisible than the edges is an intentional design.

## Lighting
The lighting is achieved by applying a common technique called deferred lighting, in which the scene is rendered to a geometry buffer with the channels "color", "normals", and "position". After the render pass, a pass to calculate the lighting data is performed. This is done by approximating the lights as spheres (with backface rendering), and calculating the effect that each light would have on the scene and drawing it to the final backbuffer.

In the demo, you can press the keys 1, 2, 3, and 4 to view the geometry buffer channels.

## Bloom
A common effect called "bloom" is also applied to the scene. The effect is simply a gaussian blur applied to the bloom channel and then later is composited with the final scene in order to produce the final image. This effect allows the glowing lights and digital clouds to appear even brighter because the bright colors bleed out.


## Boids
The last experimental feature of this demo is the random cubes flying around using a modified version of [Boids](https://en.wikipedia.org/wiki/Boids)

# Contributors
* **Robert Hewitt** - *Building Procedure, Cars, Clouds, Lighting* - [Runouw](https://github.com/runouw)
* **Kevin Rodriguez** - *Building design, design, ideas, Boids* - [KevRod](https://github.com/kevrod)

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE.md](LICENSE.md) file for details
