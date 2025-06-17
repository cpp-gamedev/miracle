#include <game.hpp>
#include <glm/gtx/norm.hpp>
#include <le2d/context.hpp>
#include "lighhouse.hpp"

namespace miracle {
Game::Game(gsl::not_null<le::ServiceLocator const*> services) : m_services(services), m_lighthouse(services) { m_circle.create(50.0f); }

void Game::on_cursor_pos(le::event::CursorPos const& cursor_pos) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_cursor_pos = cursor_pos.normalized.to_target(framebuffer_size);
}

void Game::tick([[maybe_unused]] kvf::Seconds const dt) {
	m_circle.transform.position = m_cursor_pos;
	m_lighthouse.rotate_towards_cursor(m_cursor_pos);
}

void Game::render(le::Renderer& renderer) const {
	m_circle.draw(renderer);
	m_lighthouse.render(renderer);
}
} // namespace miracle
