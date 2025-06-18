#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include <optional>
#include "le2d/texture.hpp"

namespace miracle {
class Lighthouse {
  public:
	explicit Lighthouse(gsl::not_null<le::ServiceLocator const*> services);

	void rotate_towards_cursor(glm::vec2 cursor_pos);
	void render(le::Renderer& renderer) const;

  private:
	gsl::not_null<le::ServiceLocator const*> m_services;
	std::optional<le::Texture> m_texture;
	le::drawable::Circle m_sprite{};
};
} // namespace miracle
