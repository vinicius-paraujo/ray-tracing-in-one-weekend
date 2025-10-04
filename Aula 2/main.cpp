#include <fstream>
#include <string>
#include <iostream>

#include "src/colors/color.h"
#include "src/ray/ray.h"
#include "src/vectors/vec3.h"

// retorna um double agr
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

// reflete v em torno de n
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

color ray_color(const ray& r, const point3& centro, double rEsfera) {
    auto t = hit_sphere(centro, rEsfera, r);

    // cor do fundo se n tiver interseção
    if (t <= 0.0) return color(0.5, 0.5, 0.5);

    // ponto interseção
    point3 PI = r.at(t);

    // cálculo da normal
    vec3 n = unit_vector(PI - centro);

    // Fonte de luz
    point3 light_pos(0, 5, 0);
    color I_F(0.7, 0.7, 0.7);

    // Vetor luz (direção da luz até o ponto)
    vec3 l = unit_vector(light_pos - PI);

    // Vetor observador (do ponto até o olho)
    vec3 v = unit_vector(-r.direction());

    // vetor refletido
    vec3 rfl = reflect(-l, n);

    // coeficiente difuso do material
    color K_d(1.0, 0.0, 0.0);
    // coeficiente especular (brilho)
    color K_s(1.0, 1.0, 1.0);

    // Expoente especular
    int m = 50;

    // Termos difuso e especular
    double diff = std::max(0.0, dot(l, n));
    double spec = pow(std::max(0.0, dot(v, rfl)), m);

    color I_d = I_F * K_d * diff;
    color I_e = I_F * K_s * spec;

    return I_d + I_e;
}

int main() {
    // propriedades da janela
    auto wJanela = 10.0;
    auto hJanela = 10.0;
    auto dJanela = 5.0;

    // olho do pintor
    auto zoio = point3(0, 0, 0);

    int nCol = 512;
    int nLin = 512;

    // tela de mosquito
    auto Dx = wJanela / nCol;
    auto Dy = hJanela / nLin;
    
    // arquivo ppm
    std::cout << "P3\n" << nCol << ' ' << nLin << "\n255\n";

    // esfera
    double rEsfera = 3.0;
    point3 centroEsfera = point3(0, 0, -9.0); 

    for (int l = 0; l < nLin; ++l) {
        std::clog << "\rLinhas restantes: " << (nLin - l) << ' ' << std::flush;
        for (int c = 0; c < nCol; ++c) {
            auto x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            auto y =  hJanela / 2.0 - Dy / 2.0 - l * Dy;
            
            auto ponto_na_janela = point3(x, y, -dJanela);

            auto ray_direction = ponto_na_janela - zoio;

            ray r(zoio, ray_direction);
            
            color pixel_color = ray_color(r, centroEsfera, rEsfera);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rConcluído.                  \n";
}