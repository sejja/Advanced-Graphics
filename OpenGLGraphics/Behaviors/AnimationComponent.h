#ifndef _ANIMATION_COMPONENT__H_
#define _ANIMATION_COMPONENT__H_

#include "Core/ECSystem/Scene.h"
#include "Parser.h"

namespace Behaviors {
	class AnimationComponent : public Core::Component {
	public:
		AnimationComponent(std::weak_ptr<Core::Object> parent) : 
		Component(parent), mElapsed(0), mOriginal(parent.lock()->GetPosition()) {}
		~AnimationComponent() = default;
		void Update() override;
	
	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent2 : public Core::Component {
	public:
		AnimationComponent2(std::weak_ptr<Core::Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->GetPosition()) {}
		~AnimationComponent2() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent3 : public Core::Component {
	public:
		AnimationComponent3(std::weak_ptr<Core::Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->GetPosition()) {}
		~AnimationComponent3() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent4 : public Core::Component {
	public:
		AnimationComponent4(std::weak_ptr<Core::Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->GetPosition()) {}
		~AnimationComponent4() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};
}

#endif