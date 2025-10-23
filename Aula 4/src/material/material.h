#ifndef MATERIAL_H
#define MATERIAL_H

#include "../colors/color.h"

/**
 * classe material com base em
 * phong shading
 */
class material {
  public:
    // coeficiente ambiente
    color k_ambient;
    // coeficiente difuso
    color k_diffuse;
    // coeficiente especular
    color k_specular;
    // expoente especular
    int shininess;

    material(const color& ka, const color& kd, const color& ks, int m)
      : k_ambient(ka), k_diffuse(kd), k_specular(ks), shininess(m) {}
};

#endif