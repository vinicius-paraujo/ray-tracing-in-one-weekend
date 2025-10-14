#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "../vectors/vec3.h"
#include <memory>

class plane : public hittable {
  public:
    // recebe um ponto (P0), vetor normal e material
    plane(const point3& p, const vec3& n, std::shared_ptr<material> m)
      : point_on_plane(p), normal(unit_vector(n)), mat(m) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        // testa se o raio é paralelo
        // se for,
        // (produto escalar entre a direção do raio e a normal do plano = 0)
        // ou seja, não há interseção
        // (d * n)
        double denominator = dot(normal, r.direction());
        if (std::abs(denominator) < 1e-8) {
            return false;
        }

        // Calcula o t da interseção usando a fórmula raio-plano
        // ((P0 - Q)*n)/(d*n)
        double t = dot(point_on_plane - r.origin(), normal) / denominator;

        // Verifica se a interseção está dentro do intervalo válido [tmin, tmax]
        if (t <= ray_tmin || t > ray_tmax) {
            return false;
        }

        // Se a interseção é válida, declara colisão
        rec.t = t;
        rec.p = r.at(t);
        
        rec.mat = mat;

        // Define a normal. A normal deve sempre apontar "contra" o raio que a atingiu.
        // Se o produto escalar da direção do raio e da normal for positivo,
        // o raio está atingindo o plano por "trás", então invertemos a normal do registro.
        if (dot(r.direction(), normal) > 0.0) {
            rec.normal = -normal;
        } else {
            rec.normal = normal;
        }

        return true;
    }

  private:
    point3 point_on_plane;
    vec3 normal;
    std::shared_ptr<material> mat;
};

#endif