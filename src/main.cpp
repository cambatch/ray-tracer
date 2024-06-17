#include <limits>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>

#include "glDraw.hpp"
#include <glad/gl.h>
#include <stb_image_write.h>

#include "color.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "timer.hpp"


void FlipBufferVertical(std::vector<uint8_t>& buffer, uint32_t width, uint32_t height) {
    uint32_t rowSize = width * 3;
    std::vector<uint8_t> tempRow(rowSize);

    for (uint32_t y = 0; y < height / 2; ++y) {
        uint32_t topRow = y * rowSize;
        uint32_t bottomRow = (height - y - 1) * rowSize;

        std::memcpy(tempRow.data(), &buffer[topRow], rowSize);
        std::memcpy(&buffer[topRow], &buffer[bottomRow], rowSize);
        std::memcpy(&buffer[bottomRow], tempRow.data(), rowSize);
    }
}


bool HitSphere(const Ray& r, double tMin, double tMax, HitRecord& rec, const World& world) {
    bool hit = false;
    double closest = tMax;
    HitRecord temp;

    for (const auto& s : world.Spheres) {
        if (s.Hit(r, tMin, closest, temp)) {
            hit = true;
            closest = temp.T;
            rec = temp;
        }
    }
    return hit;
}


Colorf RayColor(const Ray& ray, const World& world) {
    HitRecord rec;

    if (HitSphere(ray, 0, std::numeric_limits<double>::infinity(), rec, world)) {
        Colorf color =  0.5f * (rec.Normal + Colorf(1.f, 1.f, 1.f));
        return color;
    }


    glm::vec3 unitDir = glm::normalize(ray.Direction);
    float a = 0.5f * (unitDir.y + 1.0);
    Colorf color = (1.f - a) * Colorf(1.f, 1.f, 1.f) + a * Colorf(0.5f, 0.7f, 1.0f);
    return color;
}


int main(int argc, char** argv) {
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::vector<uint8_t> buffer(width * height * 3);
    bool doneRendering = false;
    int32_t y = 0;
    int32_t x = 0;

    World world;
    world.AddSphere({0, 0, -1}, 0.5f);
    world.AddSphere({0, -100.5, -1}, 100.f);

    Camera camera({0, 0, 0}, width, height);

    InitDrawing(width, height);

    int samplesPerPixel = 10;
    float pixelSampleScale = 1.0 / samplesPerPixel;

    Timer timer;

    while (!glfwWindowShouldClose(window)) {
        if (!doneRendering) {
            x = 0;
            for (; x < width; ++x) {
                auto pixelCenter = camera.Pixel100Loc + ((float)x * camera.PixelDeltaU) + ((float)y * camera.PixelDeltaV);
                auto rayDir = pixelCenter - camera.CamCenter;

                Colorf colorf(0, 0, 0);

                for (int i = 0; i < samplesPerPixel; ++i) {
                    Ray ray = camera.GetRay(x, y);
                    colorf += RayColor(ray, world);
                }

                Color color = ConvertColorToInt(colorf * pixelSampleScale);

                buffer[(y * width + x) * 3 + 0] = color.r;
                buffer[(y * width + x) * 3 + 1] = color.g;
                buffer[(y * width + x) * 3 + 2] = color.b;
            }

            UpdateTexture(0, y, width, 1, buffer.data());

            ++y;
            if (y == height) {
                doneRendering = true;
                std::cout << "Finished in " << timer.ElapsedSeconds() << " seconds\n";
            }
        }

        Draw();
    }

    // Flip buffer to turn into image.
    FlipBufferVertical(buffer, width, height);
    stbi_write_png("assets/renders/test2.png", width, height, 3, buffer.data(), sizeof(uint8_t) * width * 3);

    TerminateDrawing();
    glfwTerminate();
}
