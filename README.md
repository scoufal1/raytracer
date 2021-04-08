# raytracer

Implements a raytracer based on the book [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). 

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake -G "Visual Studio 16 2019" ..
raytracer/build $ start CS312-Raytracer.sln
```

Your solution file should contain multiple projects, such as `gradient` and `normals`.
To run from the git bash command shell, 

```
raytracer/build $ ../bin/Debug/gradient
raytracer/build $ ../bin/Debug/normals
```

*macOS*

Open terminal to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake ..
raytracer/build $ make
```

To run each program from build, you would type

```
raytracer/build $ ../bin/gradient
raytracer/build $ ../bin/normals
```
## Required Materials

![materials](https://user-images.githubusercontent.com/48161551/114091206-63a92080-9886-11eb-82bf-60e08faad8a2.png)

## Primitives

Triangle:

<img src="https://user-images.githubusercontent.com/48161551/114091313-889d9380-9886-11eb-96e3-b14195eb928c.png"  width="400"/>

Box:

<img src="https://user-images.githubusercontent.com/48161551/114091334-905d3800-9886-11eb-8aad-c851485ae7de.png"  width="400"/>

Plane:

<img src="https://user-images.githubusercontent.com/48161551/114091325-8affed80-9886-11eb-87c8-6c73af52bd0b.png"  width="400"/>

## Unique Features

Background colors can be changed:

<img src="https://user-images.githubusercontent.com/48161551/114091902-4294ff80-9887-11eb-8084-6ecab82a461c.png"  width="400"/>

Camera angle can be changed:

<img src="https://user-images.githubusercontent.com/48161551/114091935-4cb6fe00-9887-11eb-930b-18b652cc53d4.png"  width="400"/>

Pyramid made from triangles:

<img src="https://user-images.githubusercontent.com/48161551/114092366-ccdd6380-9887-11eb-8825-bbea75979e2f.png"  width="400"/>

## Unique Images

![unique_image1](https://user-images.githubusercontent.com/48161551/114092094-7c660600-9887-11eb-8b61-14ba2cfc8342.png)

![unique_image2](https://user-images.githubusercontent.com/48161551/114092096-7c660600-9887-11eb-9dba-46cc068e1ee0.png)

![unique_image3](https://user-images.githubusercontent.com/48161551/114092100-7cfe9c80-9887-11eb-8b5f-801b597b5ab8.png)
