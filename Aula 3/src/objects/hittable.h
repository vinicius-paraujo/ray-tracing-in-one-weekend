#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray/ray.h"
#include "../material/material.h"
#include <memory>

/**
 * Armazena informações locais do ponto
 * atingindo pelo raio.
 */
class hit_record {
  public:
    // Ponto de interseção
    point3 p;
    vec3 normal;
    double t;
    std::shared_ptr<material> mat;
};

class hittable {
  public:
    virtual ~hittable() = default;

    // [tmin, tmax]
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif