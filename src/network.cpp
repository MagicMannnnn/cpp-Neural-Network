#include "network.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>

static std::vector<std::string> split(std::string s, std::string delimiter) {
	std::vector<std::string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
		
	}
	tokens.push_back(s);

	return tokens;
}

static double mse(double val, double pred) {
	return (val - pred) * (val - pred);
}
static double mse(std::vector<double> vals, std::vector<double> preds) {
	double sum = 0;
	for (int i = 0;i < vals.size();i++) {
		sum += mse(vals[i], preds[i]);
	}
	return sum / (double)vals.size();
}

static double mse_prime(double val, double pred) {
	return (pred - val) * 2;
}
static std::vector<double> mse_prime(std::vector<double> vals, std::vector<double> preds) {
	std::vector<double> sum;
	for (int i = 0;i < vals.size();i++) {
		sum.push_back(mse_prime(vals[i], preds[i]));
	}
	return sum;
}


void Network::addLayer(std::string layer) {
	if (layer == "Sigmoid") {
		m_layers.emplace_back(new Sigmoid());

	}else if (layer == "Tanh") {
		m_layers.emplace_back(new Tanh());

	} else if (layer == "Relu") {
		m_layers.emplace_back(new Relu());

	}
	else {
		std::cout << "error in creating layer" << layer;
	}
}

void Network::addLayer(std::string layer, int inputs, int outputs) {
	if (layer == "Dense") {
		m_layers.emplace_back(new Dense(inputs, outputs));
	}
	else {
		std::cout << "error in creating layer " << layer;
	}
}

std::vector<double> Network::forwardPass(std::vector<double> input) {
	m_inputs.clear();
	for (std::unique_ptr<Layer> &layer : m_layers) {
		m_inputs.push_back(input);
		input = (*layer).forwardPass(input);
	}
	m_inputs.push_back(input);
	return input;
}

void Network::train(int episodes, float l_rate, const std::vector<std::vector<double>>& X, const std::vector<std::vector<double>>& Y, bool show) {
	float error = 0;
	for (int e = 0;e < episodes;e++) {
		error = 0;
		for (int i = 0;i < X.size();i++) {
			std::vector<double> output = forwardPass(X[i]);
			error += mse(Y[i], output);

			std::vector<double> gradient = mse_prime(Y[i], output);
			for (int j = m_layers.size()-1; j > -1; j--) {
				gradient = (*m_layers[j]).backwardPass(gradient, l_rate);
			}
		}
		error /= X.size();
		if (e == 0 && show) {
			std::cout << "start: " <<  error << std::endl;
		}
		if (show && e % (episodes > 9 ? (episodes / 10) : episodes) == 0) {
			std::cout << "episode: " << e << ", error: " << error << std::endl;
		}
		
	}
	if (show) {
		std::cout << "episode: " << episodes << ", error: " << error << std::endl;
		std::cout << "end: " << error << std::endl << std::endl;
	}
	
}


void Network::mutate(float l_rate, float mutation_chance) {
	for (std::unique_ptr<Layer>& layer : m_layers) {
		if ((*layer).m_type == "Dense") {
			(*layer).mutate(l_rate, mutation_chance);
		}
	}
}

void Network::load(std::string path) {
	m_layers.clear();
	std::string currentLine;

	std::ifstream readFile(path);

	int emptyLineCounter = 0;
	bool getIO = false;
	bool getWeights = false;
	bool getBias = false;
	std::vector<std::vector<double>> weights;
	std::vector<double> nodeWeights;
	std::vector<double> bias;
	std::string currentCharacter;
	while (std::getline(readFile, currentLine)) {
		if (currentLine == "Dense") {
			getIO = true;
		}
		else if (getIO) {
			std::vector<std::string> ios = split(currentLine, " ");
			m_layers.emplace_back(new Dense(std::stoi(ios[0]), std::stoi(ios[1])));

			getIO = false;
			getWeights = true;
			weights.clear();
		}
		else if (getWeights) {
			if (currentLine == " ") {
				getWeights = false;
				getBias = true;
				bias.clear();
			}
			else {
				std::vector<std::string> stringWeights = split(currentLine, ",");
				nodeWeights.clear();
				for (std::string weight : stringWeights) {
					nodeWeights.push_back(std::stod(weight));
				}
				weights.push_back(nodeWeights);
			}
		}
		else if (getBias) {
			std::vector<std::string> stringbias = split(currentLine, ",");
			for (std::string bias_d : stringbias) {
				bias.push_back(std::stod(bias_d));
			}
			getBias = false;
			(*m_layers.back()).setWeights(weights);
			(*m_layers.back()).setBias(bias);
		}
		else if (currentLine == "Sigmoid") {
			m_layers.emplace_back(new Sigmoid());
		}
		else if (currentLine == "Tanh") {
			m_layers.emplace_back(new Tanh());
		}
		else if (currentLine == "Relu") {
			m_layers.emplace_back(new Relu());
		}
	}
	readFile.close();
}

void Network::save(std::string path) {
	FileHandler::clearFile(path);
	for (std::unique_ptr<Layer>& layer : m_layers) {
		//layer type
		FileHandler::saveLine(path, (*layer).m_type);
		//layer data
		if ((*layer).m_type == "Dense"){
			std::string io = "";
			io += std::to_string((*layer).getInputs());
			io += " ";
			io += std::to_string((*layer).getOutputs());
			FileHandler::saveLine(path, io);
			std::string line;
			std::vector<std::vector<double >> weights = (*layer).getWeights();
			for (std::vector<double> node : weights) {
				line = "";
				for (double weight : node) {
					line += std::to_string(weight);
					line += ",";
				}
				line.pop_back();
				FileHandler::saveLine(path, line);
			}
			line = "";
			FileHandler::saveLine(path, " ");
			std::vector<double> bias = (*layer).getBias();
			for (double b : bias) {
				line += std::to_string(b);
				line += ",";
			}
			line.pop_back();
			FileHandler::saveLine(path, line);
			
		}
		FileHandler::saveLine(path, " ");
	}
	
}


Network::~Network() {
	m_layers.clear();
	m_inputs.clear();
}