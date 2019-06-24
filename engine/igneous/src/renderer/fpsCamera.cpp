#include "renderer/fpsCamera.hpp"

#include <bigg.hpp>
#include <bgfx/bgfx.h>

void FPSCamera::update(float dt)
{
	//move
	float speed = 5.0f;
	if (Input::keys[GLFW_KEY_LEFT_SHIFT]) speed = 10.0f;
	glm::vec3 mv;
	if (Input::keys[GLFW_KEY_W]) mv += forward * speed * dt;
	if (Input::keys[GLFW_KEY_A]) mv += -right * speed *  dt;
	if (Input::keys[GLFW_KEY_S]) mv += -forward * speed *  dt;
	if (Input::keys[GLFW_KEY_D]) mv += right * speed *  dt;
	if (Input::keys[GLFW_KEY_SPACE]) mv -= up * speed *  dt;
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv -= -up * speed *  dt;
	view = glm::translate(glm::mat4(), mv) * view;

	//rotate
	double dx = Input::mouseX - last_x;
	double dy = Input::mouseY - last_y;

	glm::quat yaw((float)dx * dt, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat pitch((float)dy * dt, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat tot = pitch * yaw;
	view[2] = tot * view[2] * glm::inverse(tot);

	if (Input::keys[GLFW_KEY_Q]) view = glm::rotate(glm::radians(-5.0f) * speed *  dt, glm::vec3(0.0f, 0.0f, 1.0f)) * view;
	if (Input::keys[GLFW_KEY_E]) view = glm::rotate(glm::radians(5.0f) * speed *  dt, glm::vec3(0.0f, 0.0f, 1.0f)) * view;
	last_x = Input::mouseX;
	last_y = Input::mouseY;
}