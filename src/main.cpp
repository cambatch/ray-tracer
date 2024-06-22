#include <cstring>

#include <glm/glm.hpp>
#include <stb_image_write.h>

#include "world.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "renderer.hpp"


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

int main(int argc, char** argv) {
    uint32_t width = 1920;
    uint32_t height = 1080;
    int32_t maxDepth = 10;
    World world;

    float camFov = 20.0f;
    glm::vec3 camPos = {13, 2, 3};
    glm::vec3 camLookAt = {0, 0, 0};
    glm::vec3 camUp = {0, 1, 0};
    float camDefocusAngle = 0.6f;
    float camDefocusDist = 10.0f;
    int samplesPerPixel = 10;
    float pixelSampleScale = 1.0 / samplesPerPixel;

    {
        // int32_t groundMat = world.AddLambertianMaterial({0.8f, 0.8f, 0.0f});
        // int32_t centerMat = world.AddLambertianMaterial({0.1f, 0.2f, 0.5f});
        // int32_t leftMat = world.AddDielectricMaterial(1.5f);
        // int32_t bubbleMat = world.AddDielectricMaterial(1.0f / 1.5f);
        // int32_t rightMat = world.AddMetalMaterial({0.8f, 0.6f, 0.2f}, 1.0f);

        // world.AddSphere({0, -100.5, -1}, 100.0f, groundMat);
        // world.AddSphere({0, 0, -1.2}, 0.5f, centerMat);
        // world.AddSphere({-1.0f, 0.0f, -1.0f}, 0.5f, leftMat);
        // world.AddSphere({-1.0f, 0.0f, -1.0f}, 0.4f, bubbleMat);
        // world.AddSphere({1.0f, 0.0f, -1.0f}, 0.5f, rightMat);
    }

    {
        // float R = glm::cos(PI / 4);
        // int32_t matLeft = world.AddLambertianMaterial({0.0f, 0.0f, 1.0f});
        // int32_t matRight = world.AddLambertianMaterial({1.0f, 0.0f, 0.0f});

        // world.AddSphere({-R, 0.0f, -1.0f}, R, matLeft);
        // world.AddSphere({ R, 0.0f, -1.0f}, R, matRight);
    }


    // random spheres
    {
        int32_t groundMat = world.AddLambertianMaterial({0.5f, 0.5f, 0.5f});
        world.AddSphere({0, -1000, 0}, 1000, groundMat);

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto chooseMat = RandomFloat();
                glm::vec3 center(a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat());

                if (glm::length(center - glm::vec3{4, 0.2f, 0}) > 0.9f) {
                    int32_t sphereMat;

                    if (chooseMat < 0.8f) {
                        glm::vec3 albedo = RandomVec3() * RandomVec3();
                        sphereMat = world.AddLambertianMaterial(albedo);
                        world.AddSphere(center, 0.2f, sphereMat);
                    } else if (chooseMat < 0.95f) {
                        glm::vec3 albedo = RandomVec3(0.5, 1);
                        float fuzz = RandomFloat(0, 0.5f);
                        sphereMat = world.AddMetalMaterial(albedo, fuzz);
                        world.AddSphere(center, 0.2f,sphereMat);
                    } else {
                        sphereMat = world.AddDielectricMaterial(1.5f);
                        world.AddSphere(center, 0.2f, sphereMat);
                    }
                }
            }
        }

        int32_t mat1 = world.AddDielectricMaterial(1.5f);
        world.AddSphere({0, 1, 0}, 1.0f, mat1);

        int32_t mat2 = world.AddLambertianMaterial({0.4f, 0.2f, 0.1f});
        world.AddSphere({-4, 1, 0}, 1.0f, mat2);

        int32_t mat3 = world.AddMetalMaterial({0.7f, 0.6f, 0.5f}, 0.0f);
        world.AddSphere({4, 1, 0}, 1.0f, mat3);
    }

    Camera camera(camPos, camLookAt, camUp, camFov, width, height);

    Renderer renderer(width, height,samplesPerPixel, maxDepth);
    renderer.Render(camera, world);

    // Flip buffer to turn into image.
    // FlipBufferVertical(buffer, width, height);
    // stbi_write_png("assets/renders/test2.png", width, height, 3, buffer.data(), sizeof(uint8_t) * width * 3);
}
