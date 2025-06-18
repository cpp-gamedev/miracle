#include <enemy.hpp>
#include <algorithm>
#include <iostream>
#include "glm/ext/vector_float2.hpp"
#include "kvf/color.hpp"

namespace miracle {
Enemy::Enemy(gsl::not_null<le::ServiceLocator const*> services, glm::vec2 target_pos) : m_services(services), m_target_pos(target_pos) {
	m_sprite.create(50.0f, kvf::red_v);
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	std::cout << framebuffer_size.x;
	m_sprite.transform.position = generate_spawn_pos(framebuffer_size);
	// TODO: add proper textures
}

void Enemy::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }

void move() {}

 glm::vec2 Enemy::generate_spawn_pos(glm::vec2 screen_size) {
	float radius = std::min(screen_size.x, screen_size.y) / 2.0f;
	// this is temporary and will be replaced with a more robust random number generator
	float angle = rand() / RAND_MAX * 2.0f * M_PI;
	return {radius * std::cos(angle), radius * std::sin(angle)};
}

} // namespace miracle
