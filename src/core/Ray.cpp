#include <core/Ray.h>

Ray Raycaster::castRay(float mouseX, float mouseY, int screenWidth, int screenHeight)
{
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(viewMatrix) * rayEye));

    glm::vec3 rayOrigin = glm::vec3(glm::inverse(viewMatrix)[3]);
    return Ray(rayOrigin, rayWorld);
}

Ray Raycaster::castFromCamera(float mouseX, float mouseY, int screenWidth, int screenHeight, Camera& camera)
{
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::normalize(camera.Front);

    glm::vec3 rayOrigin = camera.Position;
    return Ray(rayOrigin, rayWorld);
}

bool Raycaster::intersectAABB(const Ray& ray, const glm::vec3& minBounds, const glm::vec3& maxBounds, float& t)
{
    glm::vec3 invDir = 1.0f / ray.direction;
    if (glm::any(glm::isinf(invDir))) {
        return false;
    }

    glm::vec3 t1 = (minBounds - ray.origin) * invDir;
    glm::vec3 t2 = (maxBounds - ray.origin) * invDir;

    glm::vec3 tMin = glm::min(t1, t2);
    glm::vec3 tMax = glm::max(t1, t2);

    float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
    float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

    if (tFar < 0 || tNear > tFar) return false;

    t = (tNear > 0) ? tNear : tFar;
    return t < 100.0f;
}
