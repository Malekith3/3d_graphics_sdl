
#include "external/SDL2/include/SDL.h"
#include "external/fmt/include/fmt/core.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "CommonDefines.h"
#include "Display.h"
#include "Vectors.h"
#include "Mesh.h"

namespace
{
    vect3_t<float> cameraPosition{0.0f,0.0f,-5.0f};
    std::vector<triangle_t> trianglesToRender;
    Mesh globalMesh;

}

int InitWindow(SDL_Renderer*& renderer, SDL_Window*& window)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr)
    {
        std::cerr << fmt::format("SDL_Init Error: {}", SDL_GetError()) << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;

}

void processInput(bool& isQuitEvent)
{
    SDL_Event e;
    auto getKeySymbol= [](const SDL_Event& event){ return event.key.keysym.sym;};
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            isQuitEvent = true;
            break;
        case SDL_KEYDOWN:
            isQuitEvent = (getKeySymbol(e) == SDLK_ESCAPE);
            break;
        default:
            break;
        }
    }
}

void update()
{
    static Uint64 prevFrameTime;
    const auto currentFrameTime = SDL_GetTicks64();
    const auto deltaTime = currentFrameTime - prevFrameTime;
    const auto timeToWait = FRAME_TIME - deltaTime;

    trianglesToRender.clear();

    if (timeToWait > 0 && timeToWait <=FRAME_TIME)
    {
        SDL_Delay(timeToWait);
    }

    prevFrameTime = SDL_GetTicks64();
    globalMesh.rotation.x += +0.01;
    globalMesh.rotation.y += +0.01;
    globalMesh.rotation.z += +0.01;
    static auto offsetIndex = [](const int index){return index - 1;};
    for (const auto& [aFaceVert, bFaceVert, cFaceVert] : globalMesh.faces)
    {
        std::array<vect3_t<float>,3> faceVert{{
                globalMesh.vertices[offsetIndex(aFaceVert)],
                globalMesh.vertices[offsetIndex(bFaceVert)],
                globalMesh.vertices[offsetIndex(cFaceVert)]
            }};

        triangle_t projectedTriangle;

        std::ranges::for_each(faceVert,[&projectedTriangle](auto& vert)
        {
            static size_t cnt;
            auto transformedVert = vert.rotateX(globalMesh.rotation.x);
            transformedVert = transformedVert.rotateY(globalMesh.rotation.y);
            transformedVert = transformedVert.rotateZ(globalMesh.rotation.z);

            transformedVert.z -= cameraPosition.z;

            projectedTriangle.points[cnt] = (Render::project(transformedVert) + vect2_t<float>(WINDOW_WIDTH/2,WINDOW_HEIGHT/2));
            ++cnt;
            cnt = (cnt > 2u) ? 0u : cnt;
        });

        trianglesToRender.push_back(projectedTriangle);
    }

}

void render(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    auto renderColorBuffer = [&]()
    {
      SDL_UpdateTexture(colorBufferTexture, nullptr, &colorBuffer, static_cast<int>(WINDOW_WIDTH * sizeof(uint32_t)));
      SDL_RenderCopy(renderer, colorBufferTexture, nullptr, nullptr);
    };

    for (auto& [points] : trianglesToRender)
    {
        std::ranges::for_each(points,[&colorBuffer](const auto& point)
        {
            Render::drawRect(colorBuffer,point.x,point.y,3,3,0xFFFFFF00);
        });

        auto [point1, point2, point3] = points;
        Render::drawRect(colorBuffer,{point1.x, point1.y}, {point2.x,point2.y}, {point3.x,point3.y});
    }
    renderColorBuffer();
    colorBuffer.fill(0xFF000000);
    SDL_RenderPresent(renderer);

}

void loadCubeMesh()
{
    std::ranges::copy(cubeMeshVert, std::back_inserter(globalMesh.vertices));
    std::ranges::copy(cubeMeshFaces, std::back_inserter(globalMesh.faces));
}

void setup(SDL_Renderer*& renderer, std::array<uint32_t, COLOR_BUFFER_SIZE>& colorBuffer, SDL_Texture*& colorBufferTexture)
{
    std::ranges::fill(colorBuffer, ZERO_VALUE_COLOR_BUFFER);
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);

    loadCubeMesh();
}

void CleanUp(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Texture*& texture)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    SDL_Renderer* renderer{nullptr};
    SDL_Window* window{nullptr};

    if (const auto hasError = InitWindow(renderer, window); hasError != 0)
    {
        return hasError;
    }

    SDL_Texture* colorBufferTexture;
    std::array<uint32_t, COLOR_BUFFER_SIZE> colorBuffer{};
    setup(renderer,colorBuffer,colorBufferTexture);
    auto quit{false};

    while (!quit)
    {
        processInput(quit);
        update();
        render(renderer, colorBuffer, colorBufferTexture);
    }

    CleanUp(window, renderer, colorBufferTexture);

    return 0;
}