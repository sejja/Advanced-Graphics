#include "Renderables.h"
#include "ShaderProgram.h"
#include "Graphics/Camera.h"
#include "Graphics/OpenGLPipeline.h"


namespace Core {
	namespace Graphics {
		class Skybox : public Component {
		public:
			std::string mCubeMapPath = "";
			Skybox(const std::weak_ptr<Object>& parent);
			~Skybox();

			void LoadCubeMap(std::string path);
			void CreateCubeMap();
			void Render(Core::Primitives::Camera& cam, Core::Graphics::OpenGLPipeline& pipeline);
			void UploadSkyboxCubeMap();
			static Skybox* sCurrentSky;
		private:
			Asset<ShaderProgram> mShaderProgram;
			Asset<Model> mModel;
			GLuint mMapHandle;
		};
	};
}