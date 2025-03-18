#include <SDL3/SDL.h>
#include <GL/glew.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

static bool init(SDL_Window*& window, SDL_GLContext& context)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    window = SDL_CreateWindow(
        "Hello, World!",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!window) return false;

    context = SDL_GL_CreateContext(window);
    if (!context) return false;

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;
    //SDL_GL_SetSwapInterval(1); // V-sync

    return true;
}

static void render()
{
    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void close(SDL_Window*& window, SDL_GLContext &context)
{
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_GLContext context;
    if (!init(window, context)) return -1;

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        render();
        SDL_GL_SwapWindow(window);
    }

    close(window, context);
    return 0;
}