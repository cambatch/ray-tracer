#include "renderer.hpp"
#include "sphere.hpp"
#include "glDraw.hpp"
#include "timer.hpp"

#include <thread>
#include <iostream>


Renderer::Renderer(int32_t width, int32_t height, int32_t samples, int32_t maxDepth)
    : m_Width(width), m_Height(height), m_ColorBuffer(width * height * 3), m_DoneRendering(false),
      m_SamplesPerPixel(samples), m_PixelSampleScale(1.0f / samples), m_MaxDepth(maxDepth),
      m_ThreadPool(std::thread::hardware_concurrency())
{
    InitDrawing(width, height);
}

Renderer::~Renderer() {
    TerminateDrawing();
    glfwTerminate();
}

void Renderer::Render(const Camera& camera, const World& world) {

    std::atomic<int32_t> rowsRendered = 0;
    Timer timer;

    for (int i = 0; i < m_Height; ++i) {
        m_ThreadPool.EnqueueTask([=, this, &rowsRendered, &timer] {
            RenderChunk(i, camera, world);
            if (++rowsRendered == m_Height) {
                m_DoneRendering = true;
                std::cout << "Render took " << timer.ElapsedSeconds() << " seconds\n";
            }
        });
    }

    while (!glfwWindowShouldClose(window)) {
     if (!m_DoneRendering) {
        std::unique_lock<std::mutex> lock(m_TaskMutex);
        m_Condition.wait(lock, [=, this] {
        if (m_RowsDone.empty()) {
          return false;
        } else if (m_DoneRendering && m_RowsDone.empty()) {
          m_ThreadPool.Stop();
          return true;
        }

        int32_t yDone = m_RowsDone.front();
        m_RowsDone.pop();

        {
          std::unique_lock<std::mutex> lock(m_BufferMutex);
          UpdateTexture(0, yDone, m_Width, 1, m_ColorBuffer.data());
        }

        return true;
        });
     }

     Draw();
    }
}

void Renderer::RenderChunk(int32_t y, const Camera& camera, const World& world) {
    Colorf colorf = { 0, 0, 0 };

    for (int32_t x = 0; x < m_Width; ++x) {
        colorf = { 0, 0, 0 };
        for (int32_t sample = 0; sample < m_SamplesPerPixel; ++sample) {
            Ray ray = camera.GetRay(x, y);
            colorf += TraceRay(ray, world, m_MaxDepth);
        }

        colorf *= m_PixelSampleScale;
        Color color = ConvertColorToInt(colorf);

        {
            std::unique_lock<std::mutex> lock(m_BufferMutex);
            m_ColorBuffer[(y * m_Width + x) * 3 + 0] = color.r;
            m_ColorBuffer[(y * m_Width + x) * 3 + 1] = color.g;
            m_ColorBuffer[(y * m_Width + x) * 3 + 2] = color.b;
        }
    }

    {
        std::unique_lock<std::mutex> lock(m_TaskMutex);
        m_RowsDone.push(y);
        m_Condition.notify_one();
    }
}

Colorf Renderer::TraceRay(const Ray& ray, const World& world, int32_t depth) {
    if (depth <= 0) return Colorf(0, 0, 0);

    HitRecord record;
    int32_t closestSphere = HitSphere(ray, 0.001f, INF, record, world.Spheres);

    if (closestSphere != -1) {
        Ray scattered;
        Colorf attenuation;

        const auto& s = world.Spheres[closestSphere];
        const auto& m = world.Materials[s.MatIndex];

        switch (m.Type) {
            case MaterialType::LAMBERTIAN: {
                m.LambertianScatter(ray, record, attenuation, scattered);
                break;
            }
            case MaterialType::METAL: {
                m.MetalScatter(ray, record, attenuation, scattered);
                break;
            }
            case MaterialType::DIELECTRIC: {
                m.DielectricScatter(ray, record, attenuation, scattered);
                break;
            }
        }

        return attenuation * TraceRay(scattered, world, depth - 1);
    } else {
        // sky
        glm::vec3 unitDir = glm::normalize(ray.Direction);
        float a = 0.5f * (unitDir.y + 1.0);
        Colorf color = (1.0f - a) * Colorf(1.0f, 1.0f, 1.0f) + a * Colorf(0.5f, 0.7f, 1.0f);
        return color;
    }
}

