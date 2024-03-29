//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Scene.hpp"
#include "Renderer.hpp"


inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(-1, 5, 10);
    int m = 0;
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
             // generate primary ray direction
           float x = ((i / (float) scene.width * 2.0) - 1.0) * scale * imageAspectRatio;
           float y = ((j / (float) scene.height * 2.0) - 1.0) * scale;

            // Don't forget to normalize this direction!
            Vector3f dir = normalize(Vector3f(x, -y, -1)); 
            Ray ray = Ray(eye_pos, dir, 0);
            framebuffer[m] = scene.castRay(ray, 0);
            m++;
        }
        UpdateProgress(j / (float)scene.height);
    }
    UpdateProgress(1.f);

    // save framebuffer to file
    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << scene.width << " " << scene.height << "\n255\n";
    for (uint32_t i = 0; i < scene.height * scene.width; ++i) {
        auto r = (uint8_t)(255 * clamp(0, 1, framebuffer[i].x));
        auto g = (uint8_t)(255 * clamp(0, 1, framebuffer[i].y));
        auto b = (uint8_t)(255 * clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }
    ofs.close();
}
