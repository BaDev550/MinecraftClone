#pragma once

#include <glm/glm.hpp>

class FrustumCulling
{
public:
    glm::vec4 planes[6];

    void updateFrustumPlanes(const glm::mat4& vpMatrix);
    bool isBoxInFrustum(const glm::vec3& min, const glm::vec3& max);
};

