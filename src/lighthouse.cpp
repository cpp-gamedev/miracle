#include <lighhouse.hpp>
#include "le2d/asset_loader.hpp"
#include "le2d/data_loader.hpp"

namespace miracle {

Lighthouse::Lighthouse(gsl::not_null<le::ServiceLocator const*> services) : m_services(services) {
	m_sprite.create(200.0f);

	auto const& data_loader = services->get<le::IDataLoader>();
	auto const& context = services->get<le::Context>();
	auto asset_loader = le::AssetLoader{&data_loader, &context};
	m_texture = asset_loader.load_texture("images/lighthouse.png");
	m_sprite.texture = &m_texture.value();
}
void Lighthouse::rotate_towards_cursor(glm::vec2 const& cursor_pos) {
	auto const dist_sq = glm::length2((cursor_pos));
	if (dist_sq > 0.1f) {
		auto const dist = std::sqrt(dist_sq);
		auto const normalized = cursor_pos / dist;
		static constexpr auto up_v = glm::vec2(0.01f, 1.0f);
		auto const dot = glm::dot(normalized, up_v);
		auto const angle = glm::degrees(std::acos(dot));
		m_sprite.transform.orientation = cursor_pos.x > 0.0f ? -angle : angle;
	}
}

void Lighthouse::render(le::Renderer& renderer) const { m_sprite.draw(renderer); }
} // namespace miracle