#pragma once
#include "glm/vec2.hpp"

struct EnemyParams {
	glm::vec2 target_pos{};
	float move_speed{};
};
