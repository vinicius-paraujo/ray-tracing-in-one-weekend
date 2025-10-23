#ifndef RAY_H
#define RAY_H

#include "../vectors/vec3.h"

// classe que representa o raio de luz
class ray {
    public:
        ray() {}
        
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        point3 at(double t) const {
            return orig + t*dir;
        }

    private:
        point3 orig;
        vec3 dir;
};

#endif