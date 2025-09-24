#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
    // propriedades da nossa imagem
    int largura_imagem = 512;
    int altura_imagem = 512;

    // renderização da imagem
    std::cout << "P3\n" << largura_imagem << ' ' << altura_imagem << "\n255\n";

    for (int j = 0; j < altura_imagem; j++) {
        std::clog << "\rLinhas restantes: " << (altura_imagem - j) << ' ' << std::flush;
        for (int i = 0; i < largura_imagem; i++) {
            auto pixel_color = color(
                double(i)/(largura_imagem - 1),
                double(j)/(altura_imagem-1),
                0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rFinalizado.              \n";
}