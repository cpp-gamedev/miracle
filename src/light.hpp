#pragma once

#include "enemy.hpp"
#include "glm/vec2.hpp"
#include "gsl/pointers"
#include "le2d/drawable/shape.hpp"
#include "le2d/renderer.hpp"
#include "le2d/service_locator.hpp"
#include "le2d/texture.hpp"

namespace miracle {
class Light {
  public:
	explicit Light(gsl::not_null<le::ServiceLocator const*> services);

	void render(le::Renderer& renderer) const;
	void set_position(glm::vec2 cursor_pos);
	void check_enemy_collision(Enemy& enemy);

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	std::optional<le::Texture> m_texture;
	le::drawable::Circle m_sprite{};
	float m_diameter{};
};
} // namespace miracle
