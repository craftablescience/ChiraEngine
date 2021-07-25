#pragma once

#include <glm/glm.hpp>

enum soundType {
    MUSIC,
    EFFECT,
    UI
};

class abstractSound {
public:
    virtual bool init(const std::string& filename, float pitch_, float gain_, const glm::vec3& position_, soundType type_, bool loop_, bool is3d_) = 0;
    virtual void play() = 0;
    virtual void update() = 0;
    virtual void stop() = 0;
    virtual void discard() = 0;

    void setPitch(float newPitch) {
        this->pitch = newPitch;
    }
    [[nodiscard]] float getPitch() const {
        return this->pitch;
    }
    void setGain(float newGain) {
        this->gain = newGain;
    }
    [[nodiscard]] float getGain() const {
        return this->gain;
    }
    [[nodiscard]] soundType getType() const {
        return this->type;
    }
    [[nodiscard]] bool is3D() const {
        return this->is3d;
    }
    [[nodiscard]] bool isPlaying() const {
        return this->playing;
    }

    void setPosition(const glm::vec3& newPosition) {
        if (!this->is3d) {
            return;
        }
        this->position.x = newPosition.x;
        this->position.y = newPosition.y;
        this->position.z = newPosition.z;
    }
    glm::vec3 getPosition() {
        if (!this->is3d) {
            return glm::vec3{};
        }
        return this->position;
    }
protected:
    float pitch;
    float gain;
    glm::vec3 position;
    soundType type;
    bool loop;
    bool is3d;
    bool playing = false;
};
