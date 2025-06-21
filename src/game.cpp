#include <game.hpp>
#include <glm/gtx/norm.hpp>
#include <le2d/context.hpp>
#include <algorithm>
#include <cstddef>
#include <vector>
#include "enemy.hpp"
#include "kvf/time.hpp"
#include "lighhouse.hpp"
#include "util/random.hpp"

namespace miracle {
Game::Game(gsl::not_null<le::ServiceLocator const*> services) : m_services(services), m_lighthouse(services) {
	m_circle.create(70.0f);
	spawn_wave();
}

void Game::on_cursor_pos(le::event::CursorPos const& cursor_pos) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_cursor_pos = cursor_pos.normalized.to_target(framebuffer_size);
}

void Game::tick([[maybe_unused]] kvf::Seconds const dt) {
	if (!m_running) { return; }
	m_time_since_last_wave_spawn += dt;
	if (m_time_since_last_wave_spawn >= m_wave_interval) {
		spawn_wave();
		m_time_since_last_wave_spawn = kvf::Seconds{};
	}
	for (auto& enemy : m_enemies) {
		enemy.check_collision(m_circle.transform.position, 50.0f);
		enemy.translate(dt);
	}
	std::erase_if(m_enemies, [](Enemy const& enemy) { return !enemy.get_health(); });
	m_circle.transform.position = m_cursor_pos;
	m_lighthouse.rotate_towards_cursor(m_cursor_pos);
}

void Game::render(le::Renderer& renderer) const {
	m_circle.draw(renderer);
	m_lighthouse.render(renderer);
	for (auto const& enemy : m_enemies) { enemy.render(renderer); }
}

void Game::spawn_wave() {
	++m_wave_count;
	m_wave_interval += kvf::Seconds{5};
	std::vector<Enemy> new_wave;
	std::size_t const wave_size = m_wave_count * 3;
	new_wave.reserve(wave_size);
	for (std::size_t i = 0; i < wave_size; ++i) { new_wave.emplace_back(m_services, glm::vec2{0.0f, 0.0f}, util::random_range(35.0f, 65.0f)); }
	m_enemies.insert(m_enemies.end(), std::make_move_iterator(new_wave.begin()), std::make_move_iterator(new_wave.end()));
}

} // namespace miracle
