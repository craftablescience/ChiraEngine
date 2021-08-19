#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "abstractCamera.h"
#include "light.h"
#include "../core/engine.h"

class world {
public:
    world(class engine* e, abstractCamera* camera);
    virtual ~world();
    [[nodiscard]] abstractCamera* getCamera() const;
    void setCamera(abstractCamera* newCamera);
    void addMesh(const std::shared_ptr<mesh>& mesh_);
    void render();
private:
    bool compiled = false;
    engine* enginePtr;
    abstractCamera* camera = nullptr;
    std::vector<std::shared_ptr<mesh>> meshes{};
    std::vector<std::unique_ptr<abstractLight>> lights;
};
