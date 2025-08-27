#pragma once
#include <kvf/time.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/input/action.hpp>
#include <le2d/input/scoped_mapping.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include <memory>
#include "enemy.hpp"
#include "le2d/drawable/text.hpp"
#include "le2d/resource/font.hpp"
#include "light.hpp"
#include "lighthouse.hpp"

namespace miracle {
class Game {
  public:
	explicit Game(gsl::not_null<le::ServiceLocator const*> services);

	void tick(kvf::Seconds dt);
	void render(le::Renderer& renderer) const;
	void update_score(int points);
	void update_health_text();
	void spawn_wave();

  private:
	void on_look(le::input::action::Value value);

	gsl::not_null<le::ServiceLocator const*> m_services;

	le::input::ScopedActionMapping m_action_mapping;
	le::input::action::Cursor m_look_action{};

	Lighthouse m_lighthouse;
	Light m_light;

	std::unique_ptr<le::IFont> m_font{};
	le::drawable::Text m_score_text{};
	le::drawable::Text m_health_text{};
	int m_score{};
	std::string m_score_str;
	std::string m_health_str;
	glm::vec2 m_cursor_pos{};
	int m_wave_count{};
	bool m_running{true};
	kvf::Seconds m_wave_interval{};
	kvf::Seconds m_time_since_last_wave_spawn{};
	std::vector<Enemy> m_enemies{};
};
} // namespace miracle
