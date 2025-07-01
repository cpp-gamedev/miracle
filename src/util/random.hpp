#pragma once
#include <glm/ext/vector_float2.hpp>
#include <klib/assert.hpp>
#include <klib/concepts.hpp>
#include <cmath>
#include <numbers>
#include <random>
namespace miracle::util {
/// \brief Wrapper that reuses the same random engine for all calls.
class Random {
  public:
	template <klib::NumberT Type>
	[[nodiscard]] static auto in_range(Type const min, Type const max) -> Type {
		if constexpr (std::integral<Type>) {
			auto dist = std::uniform_int_distribution<Type>{min, max};
			return dist(m_engine);
		} else {
			auto dist = std::uniform_real_distribution<Type>{min, max};
			return dist(m_engine);
		}
	}

  private:
	inline static std::default_random_engine m_engine{std::random_device{}()};
};

/// \returns Random value in the range [min, max].
template <klib::NumberT Type>
[[nodiscard]] auto random_range(Type const min, Type const max) -> Type {
	return Random::in_range(min, max);
}

/// \returns Random index in the range [0, size - 1].
/// \pre size must be greater than 0.
[[nodiscard]] inline auto random_index(std::size_t const size) -> std::size_t {
	KLIB_ASSERT(size > 0);
	return Random::in_range(0uz, size - 1);
}
// Returns a random coordinate on the specified radius
[[nodiscard]] inline auto get_random_location_on_radius(float radius) -> glm::vec2 {
	float const angle = random_range(0.0f, 2.0f * std::numbers::pi_v<float>);
	return {radius * std::cos(angle), radius * std::sin(angle)};
}
} // namespace miracle::util
