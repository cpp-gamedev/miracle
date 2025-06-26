#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include <cstddef>
#include <optional>
#include "enemy_params.hpp"
#include "glm/vec2.hpp"
#include "le2d/texture.hpp"

namespace miracle {
class Enemy {
  public:
	explicit Enemy(gsl::not_null<le::ServiceLocator const*> services, EnemyParams const& params);

	void render(le::Renderer& renderer) const;
	void translate(kvf::Seconds dt);
	void take_damage(std::size_t dmg);
	[[nodiscard]] std::size_t get_health() const { return m_health; }
	[[nodiscard]] std::pair<glm::vec2, float> get_pos_diameter() const;
	bool m_can_render{false};

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	std::optional<le::Texture> m_texture;
	le::drawable::Circle m_sprite{};
	glm::vec2 m_target_pos{};
	float m_move_speed{};
	float m_diameter{};
	std::size_t m_health{100};
};
} // namespace miracle
