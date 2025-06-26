#include <light.hpp>
#include "kvf/color.hpp"

namespace miracle {
Light::Light(gsl::not_null<le::ServiceLocator const*> services) : m_services(services), m_diameter(100) { m_sprite.create(150.0f, kvf::white_v); }

void Light::check_enemy_collision(Enemy& enemy) {
	auto [pos, diameter] = enemy.get_pos_diameter();
	if (glm::distance(pos, m_sprite.transform.position) < (diameter + m_diameter)) {
		enemy.take_damage(1);
	} else {
		enemy.can_render = false;
	}
}
void Light::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }
void Light::set_position(glm::vec2 cursor_pos) { m_sprite.transform.position = cursor_pos; }
} // namespace miracle
