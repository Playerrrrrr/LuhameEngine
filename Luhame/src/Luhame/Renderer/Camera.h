#pragma once
#include"glm/glm.hpp"
#include"Luhame/Core/TimeStep.h"
namespace Luhame {
	class camera
	{
	public:
		camera(const glm::mat4& projection_matrix);

		void focus();
		void update(const time_step& ts);

		inline float get_distance() const { return m_distance; }
		inline void set_distance(float distance) { m_distance = distance; }

		inline void set_projection_matrix(const glm::mat4& projection_matrix) { m_projection_matrix = projection_matrix; }
		inline void set_viewport_size(uint32_t width, uint32_t height) { m_viewport_width = width; m_viewport_height = height; }

		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
		const glm::mat4& get_view_matrix() const { return m_view_matrix; }

		glm::vec3 get_up_direction();
		glm::vec3 get_right_direction();
		glm::vec3 get_forward_direction();
		const glm::vec3& get_position() const { return m_position; }
	private:
		void mouse_pan(const glm::vec2& delta);
		void mouse_rotate(const glm::vec2& delta);
		void mouse_zoom(float delta);

		glm::vec3 calculate_position();
		glm::quat get_orientation();

		std::pair<float, float> pan_speed() const;
		float rotation_speed() const;
		float zoom_speed() const;
	private:
		glm::mat4 m_projection_matrix, m_view_matrix;
		glm::vec3 m_position, m_rotation, m_focal_point;

		bool m_panning, m_rotating;
		glm::vec2 m_initial_mouse_position;
		glm::vec3 m_initial_focal_point, m_initial_rotation;

		float m_distance;
		float m_pitch, m_yaw;

		uint32_t m_viewport_width = 1280, m_viewport_height = 720;
	};
}

