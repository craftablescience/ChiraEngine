#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "../render/abstractCamera.h"
#include "../render/light.h"
#include "engine.h"

class world : public compilable {
public:
    world(class engine* e, abstractCamera* camera);
    virtual ~world();
    [[nodiscard]] abstractCamera* getCamera() const;
    void setCamera(abstractCamera* newCamera);
    void addMesh(const std::string& mesh);
    unsigned int addLight(abstractLight* light);
    abstractLight* getLight(unsigned int lightId);
    void compile() override;
    void discard() override;
    void render();
private:
    bool compiled = false;
    bool lightsDirty = false;
    engine* enginePtr;
    abstractCamera* camera = nullptr;
    std::vector<std::string> meshes{};
    std::vector<std::unique_ptr<abstractLight>> lights;
};
