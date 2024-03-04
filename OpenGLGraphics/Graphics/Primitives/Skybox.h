#include "Renderables.h"
#include "ShaderProgram.h"
#include "Graphics/Camera.h"
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
			void Render(Core::Primitives::Camera& cam);
			void UploadSkyboxCubeMap();
			static Skybox* sCurrentSky;
		private:
			Asset<ShaderProgram> mShaderProgram;
			Asset<::Graphics::Primitives::GLBModel> mModel;
			GLuint mMapHandle;
		};
	};
}