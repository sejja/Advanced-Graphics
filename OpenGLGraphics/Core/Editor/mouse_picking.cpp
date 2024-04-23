#include "mouse_picking.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"



glm::vec3 MousePicking::calculateRayDirection(glm::vec2 mousePos, int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam) {
    float mouseXNorm = (2.0f * mousePos.x) / viewportWidth - 1.0f;
    float mouseYNorm = 1.0f - (2.0f * mousePos.y) / viewportHeight;

    // Calcular la dirección del rayo en el espacio de la cámara
    glm::vec4 rayClip = glm::vec4(mouseXNorm, mouseYNorm, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(cam->GetProjectionMatrix()) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(cam->GetViewMatrix()) * rayEye));

    return rayWorld;
}



void MousePicking::performRayCasting(glm::vec2 mousePos, int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam) {
    // Calculate the ray direction
    glm::vec3 rayDirection = calculateRayDirection(mousePos, viewportWidth, viewportHeight, cam);

    // Get the scene and its objects
    Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
    const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();

    for (const auto& obj : sceneObjects) {
        glm::vec3 position = obj->GetPosition();
        glm::vec3 scale = obj->GetScale();

        for (const auto& comp : obj->GetAllComponents()) {
            if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp)) {
                auto glbModel = meshComp->GetMesh().lock();
                if (glbModel) {
                    auto boundingBox = glbModel->Get()->getAABB();
                    //std::cout << "Bounding Box: " << boundingBox.mMin.x << " " << boundingBox.mMin.y << " " << boundingBox.mMin.z << std::endl;
                    if (IntersectRayWithMesh(cam->GetPosition(), rayDirection, boundingBox.mMin, boundingBox.mMax)) {
                        std::cout << "Intersected with object: " << obj->GetName() << std::endl;
                    }
                }
            }
        }
        
       


        
    }
}

bool MousePicking::IntersectRayWithMesh(glm::vec3 rayOrigin, glm::vec3 rayDirection, aiVector3D minExtents, aiVector3D maxExtents) {
    float tMin = (minExtents.x - rayOrigin.x) / rayDirection.x;
    float tMax = (maxExtents.x - rayOrigin.x) / rayDirection.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (minExtents.y - rayOrigin.y) / rayDirection.y;
    float tyMax = (maxExtents.y - rayOrigin.y) / rayDirection.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return false;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (minExtents.z - rayOrigin.z) / rayDirection.z;
    float tzMax = (maxExtents.z - rayOrigin.z) / rayDirection.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return false;

    return true;
}
