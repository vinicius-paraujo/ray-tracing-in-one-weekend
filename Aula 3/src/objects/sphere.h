#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "../material/material.h"
#include "../vectors/vec3.h"
#include <memory>

class sphere : public hittable {
  public:
    sphere(
      const point3& center, 
      double radius,
      // ponteiro para o material do objeto
      std::shared_ptr<material> m
    ) : center(center), radius(std::fmax(0,radius)), mat(m) {}

    // teste de interseção da esfera
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        // equação da esfera
        // at^2−2ht+c=0
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // testa interseção mais próxima do observador
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            // caso nenhuma esteja dentro do ponto visivel
            // então o raio não atinge a esfera em nenhum
            // ponto válido
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        // distância do raio até o ponto de colisão
        rec.t = root;
        // Q + td
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;
        rec.mat = mat;

        return true;
    }

  private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif