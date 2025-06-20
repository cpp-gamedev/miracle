#include <enemy.hpp>
#include <algorithm>
#include <numbers>
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include "kvf/color.hpp"
#include "util/random.hpp"

namespace miracle {
Enemy::Enemy(gsl::not_null<le::ServiceLocator const*> services, glm::vec2 target_pos, float move_speed)
	: m_services(services), m_target_pos(target_pos), m_move_speed(move_speed) {
	m_sprite.create(50.0f, kvf::red_v);
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_sprite.transform.position = generate_spawn_pos(framebuffer_size);
	// TODO: add proper textures
}

void Enemy::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }

void Enemy::move(kvf::Seconds const dt) {
	glm::vec2 direction = glm::normalize(m_target_pos - m_sprite.transform.position);
	glm::vec2 movement = direction * m_move_speed * dt.count();
	m_sprite.transform.position += movement;
}

glm::vec2 Enemy::generate_spawn_pos(glm::vec2 screen_size) {
	float radius = std::max(screen_size.x, screen_size.y) / 2.0f;
	float angle = util::random_range(0.0f, 2.0f * std::numbers::pi_v<float>);
	return {radius * std::cos(angle), radius * std::sin(angle)};
}
} // namespace miracle
