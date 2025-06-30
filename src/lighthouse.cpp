#include <lighhouse.hpp>
#include "glm/geometric.hpp"
#include "le2d/asset_loader.hpp"
#include "le2d/data_loader.hpp"

namespace miracle {
Lighthouse::Lighthouse(gsl::not_null<le::ServiceLocator const*> services) : m_services(services) {
	m_sprite.create(m_hitbox_diameter);
	auto const& data_loader = services->get<le::IDataLoader>();
	auto const& context = services->get<le::Context>();
	auto const asset_loader = le::AssetLoader{&data_loader, &context};
	m_texture = asset_loader.load_texture("images/lighthouse.png");
	m_sprite.texture = &m_texture.value();
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
	auto [enemy_pos, enemy_diameter] = enemy.get_collision_params();
	if (glm::distance(m_sprite.transform.position, enemy_pos) < (m_visibility_diameter + enemy_diameter) / 2.0f) {
		enemy.can_render = true;
		// No need to check for damage taken if enemy isn't already in visibility radius
		check_damage_taken(enemy);
	}
}
void Lighthouse::check_damage_taken(Enemy& enemy) {
	auto [enemy_pos, enemy_diameter] = enemy.get_collision_params();
	if (glm::distance(m_sprite.transform.position, enemy_pos) < (m_hitbox_diameter + enemy_diameter) / 2.0f) {
		take_damage(enemy_diameter / 200);
	} // magic numbers
}

void Lighthouse::take_damage(float dmg) { m_health -= dmg; }

float Lighthouse::get_health() const { return m_health; }
void Lighthouse::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }
} // namespace miracle
