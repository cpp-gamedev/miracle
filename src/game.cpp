#include <game.hpp>
#include <glm/gtx/norm.hpp>
#include <le2d/context.hpp>
#include <algorithm>
#include <cstddef>
#include <format>
#include <iterator>
#include <string>
#include <vector>
#include "enemy.hpp"
#include "kvf/time.hpp"
#include "le2d/asset_loader.hpp"
#include "le2d/data_loader.hpp"
#include "le2d/drawable/text.hpp"
#include "lighhouse.hpp"
#include "util/random.hpp"

namespace miracle {
Game::Game(gsl::not_null<le::ServiceLocator const*> services) : m_services(services), m_lighthouse(services), m_light(services) {
	spawn_wave();
	auto const& data_loader = services->get<le::IDataLoader>();
	auto const& context = services->get<le::Context>();
	auto const asset_loader = le::AssetLoader{&data_loader, &context};
	m_font = asset_loader.load_font("fonts/specialElite.ttf");
}

void Game::on_cursor_pos(le::event::CursorPos const& cursor_pos) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_cursor_pos = cursor_pos.normalized.to_target(framebuffer_size);
}

void Game::tick([[maybe_unused]] kvf::Seconds const dt) {
	m_running = m_lighthouse.get_health() > 0;
	if (!m_running) { return; }

	m_time_since_last_wave_spawn += dt;
	if (m_time_since_last_wave_spawn >= m_wave_interval) {
		spawn_wave();
		m_time_since_last_wave_spawn = kvf::Seconds{};
	}
	for (auto& enemy : m_enemies) {
		m_light.check_enemy_collision(enemy);
		m_lighthouse.check_visibility_range(enemy);
		update_health_text();
		enemy.translate(dt);
	}
	// Keep track of how many enemies were defeated and calculate score
	auto res = std::erase_if(m_enemies, [](Enemy const& enemy) { return !enemy.get_health(); });
	update_score(static_cast<int>(res * 10));
	m_light.set_position(m_cursor_pos);
	m_lighthouse.rotate_towards_cursor(m_cursor_pos);
}

void Game::render(le::Renderer& renderer) const {
	m_light.render(renderer);
	m_lighthouse.render(renderer);
	for (auto const& enemy : m_enemies) { enemy.render(renderer); }
	m_score_text.draw(renderer);
	m_health_text.draw(renderer);
}

void Game::spawn_wave() {
	++m_wave_count;
	m_wave_interval += kvf::Seconds{5};
	std::vector<Enemy> new_wave{};
	int wave_size = m_wave_count * 3;
	new_wave.reserve(static_cast<std::size_t>(wave_size));
	for (; wave_size > 0; --wave_size) {
		auto const params = Enemy::Params{.target_pos = glm::vec2{0.0f, 0.0f}, .move_speed = util::random_range(35.0f, 65.0f)};
		new_wave.emplace_back(m_services, params);
	}
	m_enemies.reserve(m_enemies.size() + new_wave.size());
	std::ranges::move(new_wave, std::back_inserter(m_enemies));
}

void Game::update_score(int points) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_score_text.transform.position.y = static_cast<float>(framebuffer_size.y) / 2.0f - 50.0f;
	m_score += points;
	m_score_str.clear();
	std::format_to(std::back_inserter(m_score_str), "Score: {}", m_score);
	m_score_text.set_string(m_font, m_score_str);
}

void Game::update_health_text() {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	float const x = (static_cast<float>(framebuffer_size.x) * 0.5f) - 150.0f;
	float const y = (static_cast<float>(framebuffer_size.y) * 0.5f) - 50.0f;
	m_health_text.transform.position = {x, y};

	m_health_str.clear();
	if (m_lighthouse.get_health() <= 0.0f) {
		std::format_to(std::back_inserter(m_health_str), "Game Over");
	} else {
		std::format_to(std::back_inserter(m_health_str), "Health: {:.1f}", m_lighthouse.get_health());
	}

	m_health_text.set_string(m_font, m_health_str);
}

} // namespace miracle
