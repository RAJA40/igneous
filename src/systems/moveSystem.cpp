#include "moveSystem.h"
#include <glm/gtx/euler_angles.hpp> 

void move(float dt, entt::registry<> &registry)
{
	registry.view<Transformation>().each([dt](const auto, auto &transformation)
	{
		glm::vec3 tranform = glm::vec3(transformation.mtx[3]);
		transformation.mtx *= glm::yawPitchRoll(dt + tranform.x * 0.000021f, dt + tranform.y * 0.000037f, 0.0f);
	});
}