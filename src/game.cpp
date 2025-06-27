#include <game.hpp>
#include <glm/gtx/norm.hpp>
#include <le2d/context.hpp>
#include <cstddef>
#include <format>
#include <iterator>
#include <string>
#include <vector>
#include "enemy.hpp"
#include "enemy_params.hpp"
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
	if (!m_running) { return; }
	m_time_since_last_wave_spawn += dt;
	if (m_time_since_last_wave_spawn >= m_wave_interval) {
		spawn_wave();
		m_time_since_last_wave_spawn = kvf::Seconds{};
	}
	for (auto& enemy : m_enemies) {
		m_light.check_enemy_collision(enemy);
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
}

void Game::spawn_wave() {
	++m_wave_count;
	m_wave_interval += kvf::Seconds{5};
	std::vector<Enemy> new_wave;
	std::size_t const wave_size = m_wave_count * 3;
	new_wave.reserve(wave_size);
	for (std::size_t i = 0; i < wave_size; ++i) {
		new_wave.emplace_back(m_services, EnemyParams{.target_pos = glm::vec2{0.0f, 0.0f}, .move_speed = util::random_range(35.0f, 65.0f)});
	}
	m_enemies.insert(m_enemies.end(), std::make_move_iterator(new_wave.begin()), std::make_move_iterator(new_wave.end()));
}

void Game::update_score(int points) {
	auto const framebuffer_size = m_services->get<le::Context>().framebuffer_size();
	m_score_text.transform.position.y = static_cast<float>(framebuffer_size.y) / 2.0f - 50.0f;
	m_score += points;
	m_score_str.clear();
	std::format_to(std::back_inserter(m_score_str), "Score: {}", m_score);
	m_score_text.set_string(m_font, m_score_str);
}
} // namespace miracle
