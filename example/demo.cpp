#include "../src/core/engine.h"
#include "../src/render/texture2d.h"

unsigned int VBO, VAO, EBO;

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
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    engine->getShader("triangle")->use();
    engine->getShader("triangle")->setUniform("ourTexture", 0);
}

static void render(engine* engine) {
    engine->getTexture("triangle")->use();
    engine->getShader("triangle")->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void stop(engine* engine) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int main() {
    engine engine;

    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    keybind noWire(GLFW_KEY_1, GLFW_PRESS, disableWireframe);
    keybind wire(GLFW_KEY_2, GLFW_PRESS, enableWireframe);
    engine.addKeybind(esc);
    engine.addKeybind(noWire);
    engine.addKeybind(wire);

    shader triangle("resources/shaders/triangle.vsh", "resources/shaders/triangle.fsh");
    engine.addShader("triangle", &triangle);

    texture2d triangleTex("resources/images/crate.jpg", GL_RGB, true);
    engine.addTexture("triangle", &triangleTex);

    engine.addInitFunction(init);
    engine.addRenderFunction(render);
    engine.addStopFunction(stop);

    engine.start("resources/images/icon.png");
}
