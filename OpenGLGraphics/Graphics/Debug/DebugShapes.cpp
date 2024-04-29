//
//	DebugShapes.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "DebugShapes.h"

namespace Graphics {
    namespace Debug {
        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        DebugSystem::DebugSystem() :
            mShader(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DebugLines.shader")) {
            mPoint.InitPoint();
            //Segment
            mSegment.InitSegment();

            //AABB
            mAABB.InitAABB();

            //Plane
            mPlane.InitPlane();

            //Sphere (just 1 circle)
            mSphere.InitSphereDisc();

            //Frustum
            mFrustum.InitFrustum();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawPoint(const glm::vec3& pt, const glm::vec4& color, 
            const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms:
            {
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(pt));
                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }

            mPoint.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawSegment(const glm::vec3& s, const glm::vec3& e, 
            const glm::vec4& color, const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms:
            {
                //Build the model to viewport matrix
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), s) * glm::scale(glm::mat4(1.0f), e - s);
                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }

            mSegment.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& color, const Core::Primitives::Camera& cam) {
            mTriangle.InitTriangle(a, b, c);

            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms:
            {
                //Model = identity (not required)
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix();

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }
            mTriangle.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawAABB(const glm::vec3& c, const glm::vec3& size, const glm::vec4& color, const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms:
            {
                //Build the model to viewport matrix
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), c) * glm::scale(glm::mat4(1.0f), size);
                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }

            mAABB.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawPlane(const glm::vec3& pt, const glm::vec3& n, float scale, const glm::vec4& color, const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            // Model: scale 2D, translate to the point, lookAt
            // following the normal
            const glm::vec3 Normal = glm::normalize(n);
            const glm::mat4 Model = glm::translate(glm::mat4(1.0f), pt) * 
                glm::lookAt(pt, Normal, glm::cross(glm::cross(Normal, 
                glm::vec3(1.0f, 0.0f, 0.0f)), Normal)) * glm::scale(glm::mat4(1.0f), 
                glm::vec3(scale));

            //Set uniforms:
            {
                //Model = identity (not required)
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx() * Model;

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }

            //Draw
            mPlane.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawSphere(const glm::vec3& c, float r, const glm::vec4& color, const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms based on which disc to draw
            //1st disc
            {
                //Build the model to viewport matrix
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), c) * glm::scale(glm::mat4(1.0f), glm::vec3(r));

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);

                mSphere.Draw();
            }


            //2nd disc
            {
                //Rotate to aligh with XZ plane
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), c) * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.0f, 0.f)) * glm::scale(glm::mat4(1.0f), glm::vec3(r));

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);

                //Draw
                mSphere.Draw();
            }


            //3rd disc
            {
                //Rotate to align with YZ plane
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), c) * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.f, 0.0f, 0.f)) * glm::scale(glm::mat4(1.0f), glm::vec3(r));

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);

                //Draw
                mSphere.Draw();
            }

            //Horizon disc
            //4th: horizon disk
            // Calculate the horizon ring's radius
            // and center
            glm::vec3 View = { 0.f, 1.f, 0.f };// = mCamera->mPosition - c;
            const float Dist = length(View);
            const float HorizonR = (r * sqrt(Dist * Dist - r * r)) / Dist;
            const float T = sqrt(r * r - HorizonR * HorizonR);

            //Compute lookAt matrix so it constantly looks to the camera

            //Set uniforms:
            {
                //Model: translate by the center, scale time the radius
                //and apply look matrix
                //Build the model to viewport matrix
                glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::translate(glm::mat4(1.0f), c + View * T) * glm::inverse(glm::lookAt(glm::vec3(0.0f), View,
                    glm::cross(View, glm::cross(View, glm::vec3(0.f, 1.0f, 0.f))))) * glm::scale(glm::mat4(1.0f), glm::vec3(HorizonR)); // = mCamera->GetProjMtx() * mCamera->GetCamMtx();

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }
            mSphere.Draw();
        }

        // ------------------------------------------------------------------------
        /*! Upadte
        *
        *   Updates the Perspective Matrix
        */ //----------------------------------------------------------------------
        void DebugSystem::DrawFrustum(const glm::mat4& vp, const glm::vec4& color, const Core::Primitives::Camera& cam) const {
            //Enable shader
            Core::Graphics::ShaderProgram* shader = mShader->Get();
            shader->Bind();

            //Set uniforms:
            {
                //Build the model to viewport matrix
                //Model: use inverse of givel world-to-perspective matrix
                const glm::mat4 MVP = cam.GetProjectionMatrix() * cam.GetViewMatrix() * glm::inverse(vp);

                glm::vec4 col = color;

                shader->SetShaderUniform("uniform_mvp", &MVP);
                shader->SetShaderUniform("uniform_color", &col);
            }

            //Draw
            mFrustum.Draw();
        }
    }
}