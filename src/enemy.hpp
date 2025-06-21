#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include <optional>
#include "glm/ext/vector_float2.hpp"
#include "le2d/texture.hpp"

namespace miracle {
class Enemy {
  public:
	explicit Enemy(gsl::not_null<le::ServiceLocator const*> services, glm::vec2 target_pos, float move_speed);

	void render(le::Renderer& renderer) const;
	void translate(kvf::Seconds dt);

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	std::optional<le::Texture> m_texture;
	le::drawable::Circle m_sprite{};
	glm::vec2 m_target_pos{};
	float m_move_speed{};
};
} // namespace miracle
