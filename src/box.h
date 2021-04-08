#ifndef BOX_H_
#define BOX_H_

#include "hittable.h"
#include "AGLM.h"

class box : public hittable {
public:
   box() : c(0), ax(0), ay(0), az(0), hx(0), hy(0), hz(0), mat_ptr(0) {}
   box(const glm::point3& center, 
       const glm::vec3& xdir, const glm::vec3& ydir, const glm::vec3& zdir,
       float halfx, float halfy, float halfz,
       std::shared_ptr<material> m) : c(center), ax(xdir), ay(ydir), az(zdir), 
          hx(halfx), hy(halfy), hz(halfz), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {

      float tmax = std::numeric_limits<float>::infinity();
      float tmin = -tmax;

      glm::point3 p = c - r.origin();
      glm::vec3 normals[3] = {ax, ay, az};
      float distances[3] = {hx, hy, hz};

      float eps = 0.00001f;
      float e,f,t1,t2;
      for(int i = 0; i < 3; i++) {
         e = dot(normals[i], p);
         f = dot(normals[i], r.direction());
         if(abs(f) > eps) {
            t1 = (e + distances[i]) / f;
            t2 = (e - distances[i])/ f;
            if (t1 > t2) std::swap(t1, t2);
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
            if (tmin > tmax) return false;
            if (tmax < 0) return false;
         } else if (-e-distances[i] > 0 || -e+distances[i] < 0) return false;
      }
      float t;
      if (tmin > 0) t = tmin;
      else t =  tmax;

      // save relevant data in hit record
      rec.t = t; // save the time when we hit the object
      rec.p = r.at(t); // ray.origin + t * ray.direction
      rec.mat_ptr = mat_ptr; 

      glm::point3 point_on_side;
      glm::vec3 parallel_vec, n;

      for(int i = 0; i < 3; i++) {
         
         point_on_side = c + (distances[i]*normals[i]);
         parallel_vec = point_on_side - rec.p;
         if(parallel_vec != glm::vec3(0)) {
            parallel_vec = normalize(parallel_vec);
         }
         
         //point is on side corresponding to normals[i] (dot of normal and parallel = 0 or 1)
         if(glm::dot(normals[i], parallel_vec) == 0 || dot(normals[i], parallel_vec) == 1) {
            n = normals[i];
         }
      }
      glm::vec3 hit_vec = rec.p - c;
      if(glm::dot(n,hit_vec) < 0) {
         n = -n;
      }
      rec.set_face_normal(r, n);
      return true;
   }

public:
   glm::vec3 c;
   glm::vec3 ax;
   glm::vec3 ay;
   glm::vec3 az;
   float hx;
   float hy;
   float hz;
   std::shared_ptr<material> mat_ptr;
};

#endif
