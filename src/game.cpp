#include <game.hpp>
#include <glm/gtx/norm.hpp>
#include <le2d/context.hpp>
#include <cmath>

namespace miracle {
Game::Game(gsl::not_null<le::ServiceLocator const*> services) : m_services(services) {
	m_triangle.vertices = {
		le::Vertex{.position = {-50.0f, -50.0f}},
		le::Vertex{.position = {+50.0f, -50.0f}},
		le::Vertex{.position = {+0.0f, +75.0f}},
	};
	m_circle.create(50.0f);
}

void Game::on_cursor_pos(le::event::CursorPos const& cursor_pos) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_cursor_pos = cursor_pos.normalized.to_target(framebuffer_size);
}

void Game::tick([[maybe_unused]] kvf::Seconds const dt) {
	m_circle.transform.position = m_cursor_pos;

	auto const dist_sq = glm::length2(m_cursor_pos);
	if (dist_sq > 0.1f) {
		auto const dist = std::sqrt(dist_sq);
		auto const normalized = m_cursor_pos / dist;
		static constexpr auto up_v = glm::vec2{0.0f, 1.0f};
		auto const dot = glm::dot(normalized, up_v);
		auto const angle = glm::degrees(std::acos(dot));
		m_triangle.transform.orientation = m_cursor_pos.x > 0.0f ? -angle : angle;
	}
}

void Game::render(le::Renderer& renderer) const {
	m_triangle.draw(renderer);
	m_circle.draw(renderer);
}
} // namespace miracle
