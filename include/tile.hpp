#pragma once

#include <obj_loader.hpp>

enum TILETYPE {
	GRASS = 0,
	FOREST
};

struct Tile {
	Tile() = default;
	Tile(glm::vec3 pos_, TILETYPE type_) {
		pos = pos_;
		type = type_;
	}

	glm::vec3 pos;
	TILETYPE type;
};
