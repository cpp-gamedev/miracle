#pragma once
#include <glm/gtx/norm.hpp>
#include <kvf/time.hpp>
#include <le2d/context.hpp>
#include <le2d/drawable/shape.hpp>
#include <le2d/event.hpp>
#include <le2d/renderer.hpp>
#include <le2d/service_locator.hpp>
#include "glm/ext/vector_float2.hpp"
#include "gsl/pointers"
#include "le2d/texture.hpp"
namespace miracle {
class Lighthouse {
  public:
	explicit Lighthouse(gsl::not_null<le::ServiceLocator const*> services);
	void rotate_towards_cursor(glm::vec2 const& cursor_pos);
	void render(le::Renderer& renderer) const;

  private:
	le::drawable::Circle m_sprite{};
	std::optional<le::Texture> m_texture;
	gsl::not_null<le::ServiceLocator const*> m_services;
};
} // namespace miracle
