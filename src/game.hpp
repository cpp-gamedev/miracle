#pragma once
#include <kvf/time.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include "enemy.hpp"
#include "lighhouse.hpp"

namespace miracle {
class Game {
  public:
	explicit Game(gsl::not_null<le::ServiceLocator const*> services);

	void on_cursor_pos(le::event::CursorPos const& cursor_pos);

	void tick(kvf::Seconds dt);
	void render(le::Renderer& renderer) const;

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;

	le::drawable::Circle m_circle{};
	Lighthouse m_lighthouse;
	Enemy m_enemy;
	glm::vec2 m_cursor_pos{};
};
} // namespace miracle
