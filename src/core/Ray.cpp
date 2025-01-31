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

bool Raycaster::intersectAABB(const Ray& ray, const glm::vec3& minBounds, const glm::vec3& maxBounds, float& t)
{
    float tmin = (minBounds.x - ray.origin.x) / ray.direction.x;
    float tmax = (maxBounds.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (minBounds.y - ray.origin.y) / ray.direction.y;
    float tymax = (maxBounds.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (minBounds.z - ray.origin.z) / ray.direction.z;
    float tzmax = (maxBounds.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    t = tmin;
    return true;
}
