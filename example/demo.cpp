#include "../src/core/engine.h"
#include "glad/gl.h"

static engine obj;
unsigned int VBO, VAO;

static void stopEngine(engine* engine) {
    engine->stop();
}

static void enableWireframe(engine* engine) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void disableWireframe(engine* engine) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void init(engine* engine) {
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

static void render(engine* engine) {
    engine->getShader("triangle")->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void stop(engine* engine) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int main() {
    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    keybind noWire(GLFW_KEY_1, GLFW_PRESS, disableWireframe);
    keybind wire(GLFW_KEY_2, GLFW_PRESS, enableWireframe);
    obj.addKeybind(esc);
    obj.addKeybind(noWire);
    obj.addKeybind(wire);

    shader triangle("resources/shaders/triangle.vsh", "resources/shaders/triangle.fsh");
    obj.addShader("triangle", &triangle);

    obj.addInitFunction(init);
    obj.addRenderFunction(render);
    obj.addStopFunction(stop);

    obj.start("resources/images/icon.png");
}
