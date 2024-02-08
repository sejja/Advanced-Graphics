#include "Skybox.h"
#include "Dependencies/STB/stb_image.h"

namespace Core {
    namespace Graphics{
        Skybox* Skybox::sCurrentSky;

        Skybox::Skybox(const std::weak_ptr<Object>& parent) :
            Component(parent) {
            mShaderProgram = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/SkyBox.shader");
            mModel = Singleton<ResourceManager>::Instance().GetResource<Model>("Content/Meshes/cube_face.obj");
            sCurrentSky = this;
        }

        Skybox::~Skybox() {}

    
        void Skybox::LoadCubeMap(std::string path) {
            mCubeMapPath = path;
        }

        void Skybox::CreateCubeMap()
        {
            unsigned MapSize = 512;

            std::string Faces[6] =
            {
                "Content/Textures/right.jpg",
                "Content/Textures/left.jpg",
                "Content/Textures/top.jpg",
                "Content/Textures/bottom.jpg",
                "Content/Textures/front.jpg",
                "Content/Textures/back.jpg"
            };

            glGenTextures(1, &mMapHandle);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mMapHandle);

            int Width = 0;
            int Height = 0;
            int Format = 0;

            stbi_set_flip_vertically_on_load(0);

            //Load 6 images for the map
            for (unsigned int i = 0; i < 6; i++)
            {
                
                stbi_uc* Surface = stbi_load((mCubeMapPath + Faces[i]).c_str(), &Width, &Height, &Format, 4);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Surface);
                stbi_image_free(Surface);
            }

            stbi_set_flip_vertically_on_load(1);

            //Texture parameters
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }

        void Skybox::Render(Core::Primitives::Camera& cam) {
            glDepthFunc(GL_LEQUAL);
            //Disable depth mask, enable it later
            glDepthMask(GL_FALSE);
            glDisable(GL_CULL_FACE);

            // Bind the program and this object's VAO
            mShaderProgram->Get()->Bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mMapHandle);

            glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
            mShaderProgram->Get()->SetShaderUniform("uTransform", &projection);
            mShaderProgram->Get()->SetShaderUniform("uView", &view);

            glBindVertexArray(mModel->Get()->GetHandle());
            glDrawArrays(GL_TRIANGLES, 0, mModel->Get()->GetVertexCount());

            //Enable depth mask & culling again
            glEnable(GL_CULL_FACE);
            glDepthMask(GL_TRUE);

            glDepthFunc(GL_LESS);
        }

        void Skybox::UploadSkyboxCubeMap() {
            glActiveTexture(GL_TEXTURE9);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mMapHandle);
        }
    }
}