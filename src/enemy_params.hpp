#pragma once
#include "glm/ext/vector_float2.hpp"

struct EnemyParams {
	glm::vec2 target_pos{};
	float move_speed{};
};
