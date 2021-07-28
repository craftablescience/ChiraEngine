#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "../render/abstractCamera.h"
#include "../utility/renderable.h"
#include "engine.h"

class world : public compilable {
public:
    world(class engine* e, abstractCamera* camera);
    virtual ~world();
    [[nodiscard]] abstractCamera* getCamera() const;
    void setCamera(abstractCamera* newCamera);
    void addMesh(const std::string& mesh);
    void compile() override;
    void discard() override;
    void render();
private:
    bool compiled;
    engine* engine;
    abstractCamera* camera = nullptr;
    std::vector<std::string> meshes;
};
