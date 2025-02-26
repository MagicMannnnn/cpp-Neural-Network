#pragma once

#include "layer.h"
#include "fileHandler.h"

#include <vector>
#include <memory>
#include <string>

class Network {
public:
	Network() = default;
	std::vector<double> forwardPass(std::vector<double> inputs);
	void train(int episodes, float l_rate, const std::vector<std::vector<double>>& X, const std::vector<std::vector<double>>& Y, bool show = false);
	void addLayer(std::string layer);
	void addLayer(std::string layer, int inputs, int outputs);
	void mutate(float l_rate, float mutation_chance);
	void load(std::string path);
	void save(std::string path);
	~Network();
private:
	std::vector<std::unique_ptr<Layer>> m_layers;
	std::vector < std::vector<double>> m_inputs;
};