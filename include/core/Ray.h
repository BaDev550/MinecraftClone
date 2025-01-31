#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <limits>
#include <core/Camera.h>

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : origin(origin), direction(glm::normalize(direction)) {}
};

class Raycaster {
public:
    Raycaster(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
        : projectionMatrix(projectionMatrix), viewMatrix(viewMatrix) {}
    Raycaster(){}

    void init(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
        this->projectionMatrix = projectionMatrix;
        this->viewMatrix = viewMatrix;
    }
    Ray castRay(float mouseX, float mouseY, int screenWidth, int screenHeight);
    Ray castFromCamera(float mouseX, float mouseY, int screenWidth, int screenHeight, Camera& camera);
    bool intersectAABB(const Ray& ray, const glm::vec3& minBounds, const glm::vec3& maxBounds, float& t);

private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};