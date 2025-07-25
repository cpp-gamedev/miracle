#include <enemy.hpp>
#include <algorithm>
#include "glm/geometric.hpp"
#include "kvf/color.hpp"
#include "util/random.hpp"

namespace miracle {
Enemy::Enemy(gsl::not_null<le::ServiceLocator const*> services, Params const& params)
	: m_services(services), m_target_pos(params.target_pos), m_move_speed(params.move_speed), m_diameter(util::random_range(40.0f, 60.0f)) {
	m_sprite.create(m_diameter, kvf::red_v);
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	auto const radius = static_cast<float>(std::max(framebuffer_size.x, framebuffer_size.y)) / 2.0f;

	m_sprite.transform.position = util::get_random_location_on_radius(radius);
	// TODO: add proper textures
}

void Enemy::render(le::Renderer& renderer) const {
	if (can_render) { m_sprite.draw(renderer); }
}

void Enemy::translate(kvf::Seconds const dt) {
	glm::vec2 const direction = glm::normalize(m_target_pos - m_sprite.transform.position);
	glm::vec2 const movement = direction * m_move_speed * dt.count();
	m_sprite.transform.position += movement;
}

auto Enemy::get_collision_info() const -> CollisionInfo { return {.pos = m_sprite.transform.position, .diameter = m_diameter}; }

void Enemy::take_damage(std::size_t dmg) {
	can_render = true;
	m_health = (dmg >= m_health) ? 0 : (m_health - dmg);
}

} // namespace miracle
