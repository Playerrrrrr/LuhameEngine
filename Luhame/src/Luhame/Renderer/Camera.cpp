#include "pch.h"
#include "Camera.h"

#include "Luhame/Core/Input.h"

#include <glfw/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#define M_PI 3.14159f

namespace Luhame {
	camera::camera(const glm::mat4& projection_matrix)
		: m_projection_matrix(projection_matrix)
	{
		m_position = { -20, 20, 20 };
		m_rotation = glm::vec3(90.0f, 0.0f, 0.0f);

		m_focal_point = glm::vec3(0.0f);
		m_distance = glm::distance(m_position, m_focal_point);

		m_yaw = 3.0f * (float)M_PI / 4.0f;
		m_pitch = M_PI / 4.0f;
	}

	void camera::focus()
	{
	}

	std::pair<float, float> camera::pan_speed() const
	{
		float x = std::min(m_viewport_width / 1000.0f, 2.4f); // max = 2.4f
		float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewport_height / 1000.0f, 2.4f); // max = 2.4f
		float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { x_factor, y_factor };
	}

	float camera::rotation_speed() const
	{
		return 0.8f;
	}

	float camera::zoom_speed() const
	{
		float distance = m_distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void camera::update(const time_step& ts)
	{
		if (input::is_key_pressed(GLFW_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ input::get_mouse_x(), input::get_mouse_y() };
			glm::vec2 delta = mouse - m_initial_mouse_position;
			m_initial_mouse_position = mouse;

			delta *= ts.get_seconds();

			if (input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_MIDDLE))
				mouse_pan(delta);
			else if (input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
				mouse_rotate(delta);
			else if (input::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT))
				mouse_zoom(delta.y);
		}

		m_position = calculate_position();

		glm::quat orientation = get_orientation();
		m_rotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
		
		m_view_matrix = glm::lookAt(m_position, m_focal_point, glm::vec3{ 0,1,0 });
		//m_view_matrix = glm::inverse(m_view_matrix);
	}

	void camera::mouse_pan(const glm::vec2& delta)
	{
		auto [x_speed, y_speed] = pan_speed();
		m_focal_point += -get_right_direction() * delta.x * x_speed * m_distance;
		m_focal_point += get_up_direction() * delta.y * y_speed * m_distance;
	}

	void camera::mouse_rotate(const glm::vec2& delta)
	{
		float yaw_sign = get_up_direction().y < 0 ? -1.0f : 1.0f;
		m_yaw += yaw_sign * delta.x * rotation_speed();
		m_pitch += delta.y * rotation_speed();
	}

	void camera::mouse_zoom(float delta)
	{
		m_distance -= delta * zoom_speed();
		if (m_distance < 1.0f)
		{
			m_focal_point += get_forward_direction();
			m_distance = 1.0f;
		}
	}

	glm::vec3 camera::get_up_direction()
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 camera::get_right_direction()
	{
		return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 camera::get_forward_direction()
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 camera::calculate_position()
	{
		return m_focal_point - get_forward_direction() * m_distance;
	}

	glm::quat camera::get_orientation()
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}
}
#undef M_PI