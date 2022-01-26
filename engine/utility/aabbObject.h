#pragma once

#include <utility>
#include <glm/glm.hpp>

namespace chira {
    using AABB = std::pair<glm::vec3, glm::vec3>;

    class AABBObject {
    public:
        AABBObject() = default;
        virtual ~AABBObject() = default;
        virtual AABB getAABB() {
            return this->aabb;
        }
        [[nodiscard]] bool isAABBPoint() const {
            return this->aabb.first == this->aabb.second;
        }
    protected:
        void setAABB(glm::vec3 vec1, glm::vec3 vec2) {
            if (this->aabb.first == vec1 && this->aabb.second == vec2)
                return;
            this->aabb.first = vec1;
            this->aabb.second = vec2;
            if (this->aabb.first.x >= this->aabb.second.x)
                std::swap(this->aabb.second.x, this->aabb.first.x);
            if (this->aabb.first.y >= this->aabb.second.y)
                std::swap(this->aabb.second.y, this->aabb.first.y);
            if (this->aabb.first.z >= this->aabb.second.z)
                std::swap(this->aabb.second.z, this->aabb.first.z);
            this->aabbDirty = false;
        }
        void setAABB(AABB pair) {
            this->setAABB(pair.first, pair.second);
        }
        [[nodiscard]] bool isAABBDirty() const {
            return this->aabbDirty;
        }
        void markAABBDirty() {
            this->aabbDirty = true;
        };
    private:
        AABB aabb;
        bool aabbDirty = true;
    };
}
