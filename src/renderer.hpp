#pragma once

#include "world.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "threadPool.hpp"

#include <mutex>
#include <queue>

class Renderer {
public:
    Renderer(int32_t width, int32_t height, int32_t samples, int32_t maxDepth);
    ~Renderer();

    void Render(const Camera& camera, const World& world);

private:
    Colorf TraceRay(const Ray& ray, const World& world, int32_t depth);
    void RenderChunk(int32_t y, const Camera& camera, const World& world);

private:
    int32_t m_Width;
    int32_t m_Height;
    std::vector<uint8_t> m_ColorBuffer;
    bool m_DoneRendering;
    int32_t m_SamplesPerPixel;
    float m_PixelSampleScale;
    int32_t m_MaxDepth;
    std::mutex m_TaskMutex;
    std::mutex m_BufferMutex;
    std::queue<int32_t> m_RowsDone;
    std::condition_variable m_Condition;
    ThreadPool m_ThreadPool;
};
