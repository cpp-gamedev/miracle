#include <glm/gtc/matrix_transform.hpp>
#include <lighthouse.hpp>
#include <resources.hpp>
#include "glm/gtx/norm.hpp"

namespace miracle {
Lighthouse::Lighthouse(gsl::not_null<le::ServiceLocator const*> services) : m_services(services) {
	m_sprite.create(m_hitbox_diameter);
	auto& resources = services->get<Resources>();
	m_sprite.texture = resources.load<le::ITexture>("images/lighthouse.png");
}

void Lighthouse::rotate_towards_cursor(glm::vec2 cursor_pos) {
	auto const dist_sq = glm::length2((cursor_pos));
	if (dist_sq > 0.1f) {
		auto const dist = std::sqrt(dist_sq);
		auto const normalized = cursor_pos / dist;
		static constexpr auto up_v = glm::vec2(0.0f, 1.0f);
		auto const dot = glm::dot(normalized, up_v);
		auto const angle = glm::degrees(std::acos(dot));
		m_sprite.transform.orientation = cursor_pos.x > 0.0f ? -angle : angle;
	}
}

void Lighthouse::check_visibility_range(Enemy& enemy) {
	auto [enemy_pos, enemy_diameter] = enemy.get_collision_info();
	auto const dist_sq = glm::length2(m_sprite.transform.position - enemy_pos);
	auto const viz_dist = (m_visibility_diameter + enemy_diameter) * 0.5f;
	if (dist_sq < viz_dist * viz_dist) {
		enemy.can_render = true;
		// No need to check for damage taken if enemy isn't already in visibility radius
		check_damage_taken(enemy_diameter, dist_sq);
	}
}
void Lighthouse::check_damage_taken(float const enemy_diameter, float const dist_sq) {
	auto const hitbox_dist = (m_hitbox_diameter + enemy_diameter) * 0.5f;
	if (dist_sq < hitbox_dist * hitbox_dist) { take_damage(enemy_diameter * 0.005f); } // magic numbers
}

void Lighthouse::take_damage(float dmg) { m_health -= dmg; }

float Lighthouse::get_health() const { return m_health; }

void Lighthouse::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }
} // namespace miracle
