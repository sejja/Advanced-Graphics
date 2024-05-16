#include "Renderables.h"
#include "ShaderProgram.h"
#include "Graphics/Camera.h"
#include "Graphics/OpenGLPipeline.h"
#include "GLBModel.h"


namespace Core {
	namespace Graphics {
		class Skybox : public Component {
		public:
			std::string mCubeMapPath = "";
			Skybox(const std::weak_ptr<Object>& parent);
			~Skybox();

			void LoadCubeMap(std::string path);
			void CreateCubeMap();
			inline std::string_view GetPath();
			void Render(Core::Primitives::Camera& cam, Core::Graphics::OpenGLPipeline& pipeline);
			void RenderSpecific(glm::mat4 view, glm::mat4 projection, Core::Graphics::OpenGLPipeline& pipeline);
			void UploadSkyboxCubeMap();
			static Skybox* sCurrentSky;
		private:
			Core::Assets::Asset<ShaderProgram> mShaderProgram;
			Core::Assets::Asset<::Graphics::Primitives::Model> mModel;
			GLuint mMapHandle;
		};

		inline std::string_view Skybox::GetPath()
		{
			return mCubeMapPath;
		}

		
	};
}