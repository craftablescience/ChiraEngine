#include "engine.h"

#if __has_include(<windows.h>)
#include <windows.h>
#endif

static void debugPrint(const std::string &string) {
#if DEBUG
    std::cout << string << std::endl;
#endif
}

input engine::inputManager;

void engine::error_callback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void engine::key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
    for (const keybind& i : engine::inputManager.keybinds) {
        if (i.getButton() == key && i.getAction() == action) {
            i.fire();
        }
    }
}

void engine::start() {
#ifdef WIN32
#if RELEASE
    FreeConsole();
#endif
#endif

    if (!glfwInit()) {
        std::cerr << "Error: GLFW not defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->window = glfwCreateWindow(640, 480, "Basic Game Engine", nullptr, nullptr);
    if (!this->window) {
        std::cerr << "Error: Window creation failed" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    glfwSetKeyCallback(this->window, engine::key_callback);

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    printf("Using GLFW v%d.%d.%d \n", major, minor, rev);
#endif

    gladLoadGL(glfwGetProcAddress);

    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->render();

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    this->stop();
}

void engine::render() {
    // Empty now
}

void engine::stop() const {
    debugPrint("Gracefully exiting...");
    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
