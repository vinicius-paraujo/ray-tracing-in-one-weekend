#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <limits>

#include "src/colors/color.h"
#include "src/ray/ray.h"
#include "src/vectors/vec3.h"
#include "src/objects/hittable_list.h"
#include "src/objects/hittable.h"
#include "src/objects/sphere.h"
#include "src/objects/plano.h"
#include "src/material/material.h"

// reflete v em torno de n
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;

    double tmin = 0.001;
    if (!world.hit(r, tmin, std::numeric_limits<double>::infinity(), rec))
        return color(0, 0, 0);

    point3 light_pos(0, 60, -30);

    // intensidade luz ambiente
    color I_A(0.3, 0.3, 0.3);
    // intensidade da fonte pontual
    color I_F(0.7, 0.7, 0.7);

    // componente ambiente
    color ambient_color = I_A * rec.mat->k_ambient;
    
    // verificação de sombra
    vec3 l = unit_vector(light_pos - rec.p);
    double light_distance = (light_pos - rec.p).length();

    point3 shadow_origin = rec.p + rec.normal * tmin;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    if (world.hit(shadow_ray, tmin, light_distance, shadow_rec))
        return ambient_color;

    vec3 n = rec.normal;
    vec3 v = unit_vector(-r.direction());
    vec3 rfl = reflect(-l, n);

    double diff = std::max(0.0, dot(l, n));
    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);

    color I_d = I_F * rec.mat->k_diffuse * diff;
    color I_e = I_F * rec.mat->k_specular * spec;

    return ambient_color + I_d + I_e;
}

int main() {
    // propriedades da janela
    auto wJanela = 60.0;
    auto hJanela = 60.0;
    auto dJanela = 30.0;

    // olho do pintor
    auto zoio = point3(0, 0, 0);

    int nCol = 500;
    int nLin = 500;

    // tela de mosquito
    auto Dx = wJanela / nCol;
    auto Dy = hJanela / nLin;
    
    // arquivo ppm
    std::cout << "P3\n" << nCol << ' ' << nLin << "\n255\n";

    // mundo e objetos
    hittable_list mundo;

    auto material_esfera = std::make_shared<material>(
        // coeficiente ambiente
        color(0.7,0.2,0.2),
        // coeficiente difuso (a esfera é vermelha)
        color(0.7, 0.2, 0.2),
        // coeficiente especular
        color(0.7, 0.2, 0.2),
        // expoente especular
        10
    );

    auto mat_chao = std::make_shared<material>(
        color(0.2, 0.7, 0.2), color(0.2, 0.7, 0.2), color(0.0, 0.0, 0.0), 1);

    auto mat_fundo = std::make_shared<material>(
        color(0.3, 0.3, 0.7), color(0.3, 0.3, 0.7), color(0.0, 0.0, 0.0), 1);

    double R_esfera = 40.0;
    mundo.add(
        std::make_shared<sphere>(point3(0,0,-100.0), R_esfera, material_esfera)
    );
    mundo.add(std::make_shared<plane>(point3(0, -R_esfera, 0), vec3(0, 1, 0), mat_chao));
    mundo.add(std::make_shared<plane>(point3(0, 0, -200), vec3(0, 0, 1), mat_fundo));

    for (int l = 0; l < nLin; ++l) {
        std::clog << "\rLinhas restantes: " << (nLin - l) << ' ' << std::flush;
        for (int c = 0; c < nCol; ++c) {
            auto x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            auto y =  hJanela / 2.0 - Dy / 2.0 - l * Dy;
            
            auto ponto_na_janela = point3(x, y, -dJanela);

            auto ray_direction = ponto_na_janela - zoio;

            ray r(zoio, ray_direction);
            
            color pixel_color = ray_color(r, mundo);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rConcluído.                  \n";
}