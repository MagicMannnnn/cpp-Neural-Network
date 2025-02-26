#pragma once

#include <obj_loader.hpp>
#include <Shader.h>
#include <Camera.h>
#include <Settings.hpp>
#include <tile.hpp>

class Renderer {
public:
	Renderer() = default;
	
	void createDrawData(const std::array<Model, Settings::N_MODELS>& models, std::array<std::array<Tile, Settings::MAP_WIDTH>, Settings::MAP_HEIGHT>& tiles) {

		vertices.clear();
		indices.clear();

		//grass only
		Model& model = models[0];

		for (std::array<Tile, Settings::MAP_WIDTH>& row : tiles) {
			for (Tile& tile : row) {
				if (tile.type == GRASS) {

				}
			}
		}
	}

private:
	unsigned int VAO, EBO, VBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

};