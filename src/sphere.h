// ray.h, from https://raytracing.github.io by Peter Shirley, 2018-2020
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "AGLM.h"
#include "math.h"

class sphere : public hittable {
public:
   sphere() : radius(0), center(0), mat_ptr(0) {}
   sphere(const glm::point3& cen, float r, std::shared_ptr<material> m) : 
      center(cen), radius(r), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override;

public:
   glm::point3 center;
   float radius;
   std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, hit_record& rec) const {

   glm::vec3 el = center - r.origin();
   float len = length(r.direction());
   glm::vec3 d = r.direction()/len; // normalize vector

   float s = dot(el, d);
   float elSqr = dot(el, el);
   float rSqr = radius * radius;
   // check if sphere is behind us and we're not inside sphere
   if(s < 0 && elSqr > rSqr) { 
      return false;
   }

   float mSqr = elSqr - s*s;
   // m is outside sphere
   if(mSqr > rSqr) {
      return false;
   }
   float q = sqrt(rSqr - mSqr);
   float t;
   // we are outside sphere
   if(elSqr > rSqr) {
      t = s - q;
   } else {
      // we are inside sphere
      t = s + q;
   }
   t = t / len;

   // save relevant data in hit record
   rec.t = t; // save the time when we hit the object
   rec.p = r.at(t); // ray.origin + t * ray.direction
   rec.mat_ptr = mat_ptr; 

   // save normal
   glm::vec3 outward_normal = normalize(rec.p - center); // compute unit length normal
   rec.set_face_normal(r, outward_normal);

   return true;
}

#endif

