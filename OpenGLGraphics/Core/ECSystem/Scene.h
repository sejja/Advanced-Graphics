//
//	Scene.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include <memory>
#include <algorithm>
#include <execution>
#include "Parser.h"
#include "Graphics/Primitives/Renderables.h"

namespace Core {
	class Renderable;
	class Scene {
#pragma region //Functions
	public:
		void CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Object>& obj)>);
		void Tick();

		template<typename PIPE>
		void UploadObjectsToPipeline(PIPE& pipe);
#pragma endregion	

#pragma region //Variables
	private:
		static SceneParser sParser;
		std::vector<std::shared_ptr<Object>> mObjects;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Resource
	*
	*   Gets a resource given the name
	*/ // ---------------------------------------------------------------------
	template<typename PIPE>
	void Scene::UploadObjectsToPipeline(PIPE& pipe) {
		using namespace Core;

		std::for_each(std::execution::unseq, mObjects.begin(), mObjects.end(), [&pipe](const std::shared_ptr<Object>& obj) {
			std::for_each(std::execution::unseq, obj->mComponents.begin(), obj->mComponents.end(), [&pipe](const std::shared_ptr<Component>& comp) {
				std::shared_ptr<Renderable> renderable = std::dynamic_pointer_cast<Renderable>(comp);

				//If the object is a renderable
				if (renderable) pipe.AddRenderable(std::dynamic_pointer_cast<Renderable>(comp));
				});
			});
	}
}

#endif