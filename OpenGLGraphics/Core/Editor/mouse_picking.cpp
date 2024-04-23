#include "mouse_picking.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include <algorithm>
#include <windows.h>
#include "Core/Editor/Editor.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"







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

    //std::cout << "Ray Direction: " << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;    

    // Get the scene and its objects
    Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
    const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();

    std::shared_ptr<Core::Object> selectedObject;
    float minDistance = std::numeric_limits<float>::infinity();

    for (const auto& obj : sceneObjects) {
        glm::vec3 position = obj->GetPosition();
        glm::vec3 scale = obj->GetScale();

        for (const auto& comp : obj->GetAllComponents()) {
            if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp)) {
                auto glbModel = meshComp->GetMesh().lock();
                if (glbModel) {
                    auto boundingBox = glbModel->Get()->getAABB();

                    //std::cout << "NAME: " << obj->GetName() << " Bounding Box: " << boundingBox.mMin.x << " " << boundingBox.mMin.y << " " << boundingBox.mMin.z << std::endl;
                    float iDistance = IntersectRayWithMesh(cam->GetPosition(), rayDirection, boundingBox.mMin, boundingBox.mMax, scale, position);
                    if (iDistance >= 0) {
                        std::cout << "Intersected with object: " << obj->GetName() << std::endl;

                        if (iDistance < minDistance) {
							minDistance = iDistance;
							selectedObject = obj;
						}

                    }
                }
            }
        } 
    }
    SelectedObj& selectedObjIns = Singleton<Editor>::Instance().GetSelectedObj();
    selectedObjIns.SetSelectedObject(selectedObject);
    selectedObjIns.SetSelectedComponent(nullptr);
}

float intersectionMax(float a, float b, float c) {
    if (a > b && a > c) {
        return a;
    }
    else if (b > c) {
        return b;
    }
    else {
        return c;
    }
}

float MousePicking::IntersectRayWithMesh(glm::vec3 rayOrigin, glm::vec3 rayDirection, aiVector3D mMin, aiVector3D mMax, glm::vec3 scale, glm::vec3 position) {
    float txMin = (mMin.x*scale.x - rayOrigin.x + position.x) / rayDirection.x;
    float txMax = (mMax.x*scale.x - rayOrigin.x + position.x) / rayDirection.x;

    float tyMin = (mMin.y*scale.y - rayOrigin.y + position.y) / rayDirection.y;
    float tyMax = (mMax.y*scale.y - rayOrigin.y + position.y) / rayDirection.y;

    float tzMin = (mMin.z * scale.z - rayOrigin.z + position.z) / rayDirection.z;
    float tzMax = (mMax.z * scale.z - rayOrigin.z + position.z) / rayDirection.z;

    if (txMin > txMax) std::swap(txMin, txMax);
    if (tyMin > tyMax) std::swap(tyMin, tyMax);
    if (tzMin > tzMax) std::swap(tzMin, tzMax);


    if ((txMin > tyMax) || (tyMin > txMax)) return -1; // la distancia no puede ser negativa asike quiere decir no intersección

    if (tyMin > txMin) txMin = tyMin;
    if (tyMax < txMax) txMax = tyMax;

    if ((txMin > tzMax) || (tzMin > txMax)) return -1;

    float tIntersection = intersectionMax(txMin, tyMin, tzMin);
    glm::vec3 IntersectionP = rayOrigin + tIntersection * rayDirection;
    float distIntersection = glm::distance(IntersectionP, rayOrigin);

    return distIntersection;
}
