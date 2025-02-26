#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


namespace Utils {
    void screenToWorldRay(
        int mouseX, int mouseY,
        int screenWidth, int screenHeight,
        const glm::mat4& projectionMatrix,
        const glm::mat4& viewMatrix,
        glm::vec3& rayOrigin,
        glm::vec3& rayDirection) {

        // Step 1: Convert mouse to NDC
        float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
        float ndcY = 1.0f - (2.0f * mouseY) / screenHeight;
        glm::vec4 clipRay(ndcX, ndcY, -1.0f, 1.0f);

        // Step 2: Transform NDC to view space
        glm::mat4 invProjectionMatrix = glm::inverse(projectionMatrix);
        glm::vec4 viewRay = invProjectionMatrix * clipRay;
        viewRay.z = -1.0f; // Set z to -1 for direction
        viewRay.w = 0.0f;  // No translation for direction

        // Step 3: Transform view ray to world space
        glm::mat4 invViewMatrix = glm::inverse(viewMatrix);
        glm::vec4 worldRay = invViewMatrix * viewRay;

        rayDirection = glm::normalize(glm::vec3(worldRay));
        rayOrigin = glm::vec3(invViewMatrix[3]); // Extract camera position
    }

    // Function to find the intersection of the ray with a horizontal plane at a given Y position
    glm::vec3 rayPlaneIntersection(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDirection,
        float planeY) {

        // Ray-plane intersection formula: t = (planeY - rayOrigin.y) / rayDirection.y
        float t = (planeY - rayOrigin.y) / rayDirection.y;

        // Compute intersection point
        glm::vec3 intersectionPoint = rayOrigin + t * rayDirection;

        return intersectionPoint;
    }
}
