#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include <cstddef>
#include <memory>
#include "le2d/resource/texture.hpp"

namespace miracle {
class Enemy {
  public:
	struct Params {
		glm::vec2 target_pos{};
		float move_speed{};
	};

	struct CollisionInfo {
		glm::vec2 pos{};
		float diameter{};
	};

	explicit Enemy(gsl::not_null<le::ServiceLocator const*> services, Params const& params);

	void render(le::Renderer& renderer) const;
	void translate(kvf::Seconds dt);
	void take_damage(std::size_t dmg);
	[[nodiscard]] std::size_t get_health() const { return m_health; }
	[[nodiscard]] CollisionInfo get_collision_info() const;
	bool can_render{false};

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	std::unique_ptr<le::ITexture> m_texture;
	le::drawable::Circle m_sprite{};
	glm::vec2 m_target_pos{};
	float m_move_speed{};
	float m_diameter{};
	std::size_t m_health{100};
};
} // namespace miracle
