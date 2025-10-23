#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

/**
 * essa classe define o conjunto de objetos
 * que estão presentes em um cenário
 */
class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    // O(n)
    void clear() { objects.clear(); }

    // adiciona um objeto no cenário
    // O(1)
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    // itera sobre todos os objetos do cenário
    // pra cada objeto, roda um teste de interseção
    // O(n)
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif