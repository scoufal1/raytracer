// Raytracer framework from https://raytracing.github.io by Peter Shirley, 2018-2020
// alinen 2021, modified to use glm and ppm_image class

#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "box.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

using namespace glm;
using namespace agl;
using namespace std;

color ray_color(const ray& r, const hittable_list& world, int depth, 
                const color& background_color_1, const color& background_color_2)
{
   hit_record rec;
   if (depth <= 0)
   {
      return color(0);
   }

   if (world.hit(r, 0.001f, infinity, rec))
   {
      ray scattered;
      color attenuation;
      if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      {
         color recurseColor = ray_color(scattered, world, depth - 1, background_color_1, background_color_2);
         return attenuation * recurseColor;
      }
      return attenuation;
   }
   vec3 unit_direction = normalize(r.direction());
   auto t = 0.5f * (unit_direction.y + 1.0f);
   return (1.0f - t) * background_color_1 + t * background_color_2;
}

color normalize_color(const color& c, int samples_per_pixel)
{
   // todo: implement me!
   float scale = 1.0f / samples_per_pixel;
   float r = std::min(0.999f, std::max(0.0f, c.r * scale));
   float g = std::min(0.999f, std::max(0.0f, c.g * scale));
   float b = std::min(0.999f, std::max(0.0f, c.b * scale));

   // apply gamma correction 
   r = sqrt(r);
   g = sqrt(g);
   b = sqrt(b);

   return color(r, g, b);
}

//need to list base points either clockwise/counterclockwise
void pyramid(hittable_list& world, const point3& b0, const point3& b1, 
            const point3& b2,const point3& b3, const point3& point, shared_ptr<material> mat_ptr) 
{
   //base
   world.add(make_shared<triangle>(b0, b1, b2, mat_ptr));
   world.add(make_shared<triangle>(b0, b2, b3, mat_ptr));

   world.add(make_shared<triangle>(b0, b1, point, mat_ptr));
   world.add(make_shared<triangle>(b1, b2, point, mat_ptr));
   world.add(make_shared<triangle>(b2, b3, point, mat_ptr));
   world.add(make_shared<triangle>(b3, b0, point, mat_ptr));
}

void ray_trace(ppm_image& image)
{
   // Image
   int height = image.height();
   int width = image.width();
   float aspect = width / float(height);
   int samples_per_pixel = 10; // higher => more anti-aliasing
   int max_depth = 10; // higher => less shadow acne

   // Camera
   vec3 camera_pos(0, 0, 6);
   //vec3 camera_pos(0, 0, 3);
   float viewport_height = 2.0f;
   float focal_length = 4.0; 
   camera cam(camera_pos, viewport_height, aspect, focal_length);

   //other angles
   //camera cam(point3(-3,1,3), point3(0,0,-1), vec3(0,1,0), 40, aspect);
   //camera cam(point3(0.5,6,2), point3(0,0,-1), vec3(0,1,0), 40, aspect);

   // World
   shared_ptr<material> gray = make_shared<lambertian>(color(0.5f));
   shared_ptr<material> matteGreen = make_shared<lambertian>(color(0, 0.5f, 0));
   shared_ptr<material> matteDarkGreen = make_shared<lambertian>(color(0, 0.3f, 0));
   shared_ptr<material> matteRed = make_shared<lambertian>(color(0.65f, 0, 0));
   shared_ptr<material> matteBrown = make_shared<lambertian>(color(0.36f, 0.20f, 0.04f));
   shared_ptr<material> metalRed = make_shared<metal>(color(1, 0, 0), 0.3f);
   shared_ptr<material> metalWhite = make_shared<metal>(color(0.96f, 0.93f, 0.93f), 0.3f);
   shared_ptr<material> metalBlue = make_shared<metal>(color(0.1f, 0.1f, 0.93f), 0.3f);
   shared_ptr<material> metalBrown = make_shared<metal>(color(0.07f, 0.04f, 0.01f), 0.3f);
   shared_ptr<material> glass = make_shared<dielectric>(1.5f);
   shared_ptr<material> tintedGlass = make_shared<dielectric>(1.5f, color(0.9f, 0.9f, 1.0f));
   shared_ptr<material> phongDefault = make_shared<phong>(camera_pos);
   shared_ptr<material> phongGreen = make_shared<phong>(camera_pos,color(0, 0.5f, 0));
   shared_ptr<material> phongRed = make_shared<phong>(camera_pos,color(0.7f, 0, 0));

   hittable_list world;

   // required material demonstration
   world.add(make_shared<sphere>(point3(-2.25, 0, -1), 0.5f, phongDefault));
   world.add(make_shared<sphere>(point3(-0.75, 0, -1), 0.5f, glass));
   world.add(make_shared<sphere>(point3(2.25, 0, -1), 0.5f, metalRed));
   world.add(make_shared<sphere>(point3(0.75, 0, -1), 0.5f, matteGreen));
   world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, gray));

   //primitive demonstration

   //plane
   //world.add(make_shared<plane>(point3(0,-1000,-1), vec3(0,1,0.01), matteGreen));

   //triangle
   //world.add(make_shared<triangle>(point3(-1, -2, -3), point3(-3, 2, -5), point3(2, -1, -3), metalBlue));

   //box

   //camera movement
   //camera cam(point3(0,4,3), point3(0,0,-1), vec3(0,1,0.5), 50, aspect);
   //world.add(make_shared<box>(vec3(0,0.25,-1), vec3(1,0,0.5), vec3(0,1,0.5), vec3(-0.5,0,1), 0.75, 1, 0.75, matteRed));

   //pyramid
   //pyramid(world, point3(-2,-0.5,-1), point3(0,-0.5,2), point3(2,-0.5,-1), point3(0,-0.5,-2), point3(0,1.5,-1), phongDefault);

   //unique image
   /*
   //tree
   
   point3 trunk(-1.25, 0, -1);
   float half_wid = 0.1f;
   float half_hgt = 0.8f;
   float half_depth = 0.1f;
   world.add(make_shared<box>(trunk, vec3(1,0,0), vec3(0,1,0), 
                              vec3(0,0,1), half_wid, half_hgt, half_depth, matteBrown));
   world.add(make_shared<box>(trunk + vec3(0.3f,0.5f,0), vec3(1,-1,0), vec3(0,1,0), 
                              vec3(0,0,1), half_wid/2, half_hgt/3, half_depth/2, matteBrown));                           
   world.add(make_shared<box>(trunk + vec3(-0.3f,0.5f,0), vec3(1,1,0), vec3(0,1,0), 
                              vec3(0,0,1), half_wid/2, half_hgt/3, half_depth/2, matteBrown));                           

   world.add(make_shared<sphere>(trunk + vec3(0, 1, 0), 0.4f, matteGreen));
   world.add(make_shared<sphere>(trunk + vec3(0.5, 0.8, 0), 0.35f, matteGreen));
   world.add(make_shared<sphere>(trunk + vec3(-0.5, 0.8, 0), 0.35f, matteGreen));


   //house

   point3 house_center(1, 0, -1);
   half_wid = 0.8f;
   half_hgt = 0.8f;
   half_depth = 0.8f;

   world.add(make_shared<box>(house_center, vec3(1,0,0), vec3(0,1,0), 
                              vec3(0,0,1), half_wid, half_hgt, half_depth, metalWhite));
   //door                           

   world.add(make_shared<box>(house_center + vec3(0,-half_hgt/2,half_depth), vec3(1,0,0), 
                              vec3(0,1,0), vec3(0,0,1), half_wid/3, half_hgt, 0, metalBrown));   

   //windows
   world.add(make_shared<box>(house_center + vec3(-half_wid,half_hgt/2,-half_depth/2.5), vec3(1,0,0), 
                              vec3(0,1,0), vec3(0,0,1), 0, half_hgt/5, half_depth/5, tintedGlass));
   world.add(make_shared<box>(house_center + vec3(-half_wid,half_hgt/2,half_depth/2.5), vec3(1,0,0), 
                              vec3(0,1,0), vec3(0,0,1), 0, half_hgt/5, half_depth/5, tintedGlass));                           
   
   //roof
   point3 b0 = house_center + vec3(-half_wid, half_hgt, half_depth);
   point3 b1 = house_center + vec3(half_wid, half_hgt, half_depth);
   point3 b2 = house_center + vec3(half_wid, half_hgt, -half_depth);
   point3 b3 = house_center + vec3(-half_wid, half_hgt, -half_depth);
   point3 point = house_center + vec3(0, 2*half_hgt, 0);
   pyramid(world, b0, b1, b2, b3, point, phongRed);
   
   //grass
   
   world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, matteDarkGreen));
   */

   // Ray trace
   for (int j = 0; j < height; j++)
   {
      for (int i = 0; i < width; i++)
      {
         color c(0, 0, 0);
         for (int s = 0; s < samples_per_pixel; s++) // antialias
         {
            float u = float(i + random_float()) / (width - 1);
            float v = float(height - j - 1 - random_float()) / (height - 1);

            ray r = cam.get_ray(u, v);
            c += ray_color(r, world, max_depth, color(1,1,1), color(0.5f, 0.7f, 1.0f));
            //c += ray_color(r, world, max_depth, color(0.5f,0.8f,0.5f), color(0.71f, 0.95f, 0.24f)); // for background demonstration
            //c += ray_color(r, world, max_depth, color(0.4f,0.4f,1), color(0.25f, 0.7f, 1.0f)); // for house image

         }
         c = normalize_color(c, samples_per_pixel);
         image.set_vec3(j, i, c);
      }
   }

   image.save("materials.png");
}
