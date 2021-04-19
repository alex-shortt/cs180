#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        for(int i = 0; i < num_nodes; i++) {
            bool is_pinned = std::find(pinned_nodes.begin(), pinned_nodes.end(), i) != pinned_nodes.end();
            Vector2D position = start + ((end - start) * i / (num_nodes - 1));
            Mass *mass = new Mass(position, node_mass, is_pinned);
            masses.push_back(mass);
        }

        for (int i = 0; i < num_nodes - 1; i++) {
            Spring *spring = new Spring(masses.at(i), masses.at(i + 1), k);
            springs.push_back(spring);
        }
    }

    Vector2D hooke(Spring *s, bool b_to_a) {
        Mass *b = b_to_a ? s->m2 : s->m1;
        Mass *a = b_to_a ? s->m1 : s->m2;

        Vector2D diff = b->position - a->position;
        double norm = diff.norm();
        return s->k * (diff / norm) * (norm - (float) s->rest_length); 
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity) {
        for (auto &s : springs) {
            s->m1->forces += hooke(s, true);
            s->m2->forces += hooke(s, false);
        }

        for (auto &m : masses) {
            if (!m->pinned) {
                Vector2D acceleration = (m->forces / m->mass) + Vector2D(0, -9.8);

                //explicit
                // m->position += m->velocity * delta_t;
                // m->velocity += acceleration * delta_t;

                //implicit
                m->velocity += acceleration * delta_t;
                m->position += m->velocity * delta_t;
            }

            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity) {
        for (auto &s : springs) {
            float offset_dist = (s->m2->position - s->m1->position).norm() - s->rest_length;
            Vector2D offset_dir = (s->m2->position - s->m1->position).unit();

            if(s->m1->pinned){
                s->m2->position -= offset_dir * offset_dist;
            } else if (s->m2->pinned){
                s->m1->position += offset_dir * offset_dist;
            } else {
                s->m1->position += offset_dir * offset_dist / 2; 
                s->m2->position -= offset_dir * offset_dist / 2;
            }
        }

        for (auto &m : masses) {
            if (!m->pinned) {
                float DAMPING = 0.00005;

                Vector2D temp_position = m->position;
                m->position = m->position + (1. - DAMPING) * (m->position - m->last_position) + Vector2D(0, -9.8) * delta_t * delta_t;
                m->last_position = temp_position;
            }
        }
    }


}
