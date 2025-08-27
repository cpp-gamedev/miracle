#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include "enemy.hpp"

namespace miracle {
class Lighthouse {
  public:
	explicit Lighthouse(gsl::not_null<le::ServiceLocator const*> services);

	void rotate_towards_cursor(glm::vec2 cursor_pos);
	void render(le::Renderer& renderer) const;
	void check_visibility_range(Enemy& enemy);
	void check_damage_taken(float enemy_diameter, float dist_sq);
	void take_damage(float dmg);
	[[nodiscard]] float get_health() const;

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	float m_hitbox_diameter{150.0f};
	float m_visibility_diameter{250.0f};
	le::drawable::Circle m_sprite{};
	float m_health{100};
};
} // namespace miracle
