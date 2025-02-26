#include "layer.h"
#include "random.hpp"
#include <random>
#include <iostream>
#include <math.h>
#include <numeric>


double tanhFunction(double x) {
	return _CMATH_::tanh(x);
}
double tanhFunctionPrime(double x) {
	return 1 - (_CMATH_::tanh(x) * _CMATH_::tanh(x));
}

double sigmoidFunction(double x) {
	return (double)1 / ((double)((double)1 + _CMATH_::exp(-x)));
}
double sigmoidFunctionPrime(double x) {
	double s = sigmoidFunction(x);
	return s * ((double)1 - s);
}

double reluFunction(double x) {
	return std::max(0.0, x);
}
double reluFunctionPrime(double x) {
	if (x > 0) return 1;
	return 0;
}



using namespace Core;



Layer::Layer() {
	m_inputs = 0;
	m_outputs = 0;
}
Layer::Layer(int inputs, int outputs) {
	m_inputs = inputs;
	m_outputs = outputs;
}
std::vector<double> Layer::forwardPass(std::vector<double> input) {
	return input;
}
std::vector<double> Layer::backwardPass(std::vector<double> outputGradient, float l_rate) {
	return outputGradient;
}

void Layer::mutate(float l_rate, float mutation_chance) {

}


std::vector<std::vector<double>> Layer::getWeights() {
	std::vector<std::vector<double>> x;
	return x;
}
std::vector<double> Layer::getBias() {
	std::vector<double> x;
	return x;
}

void Layer::setWeights(std::vector<std::vector<double>>) {

}
void Layer::setBias(std::vector<double>) {

}

int Layer::getInputs() {
	return m_inputs;
}
int Layer::getOutputs() {
	return m_outputs;
}

Layer::~Layer() {
	
}

Dense::Dense(int inputs, int outputs) {
	m_type = "Dense";
	m_inputs = inputs;
	m_outputs = outputs;

	
	for (int node = 0; node < inputs; node++) {
		std::vector<double> node_weights;
		for (int output = 0; output < outputs; output++) {
			node_weights.push_back(Random::getRange(0, 1));
		}
		m_weights.push_back(node_weights);
	}
	
	for (int i = 0; i < outputs; i++) {
		m_bias.push_back(Random::getRange(0, 1));
	}
	

}

std::vector<double> Dense::forwardPass(std::vector<double> input) {
	m_input = input;
	std::vector<double> output;

	for (int i = 0;i < m_weights[0].size();i++) {
		double sum = 0;
		
		for (int j = 0;j < m_weights.size();j++) {
			sum += m_weights[j][i] * input[j];
		}
		output.push_back(sum);
	}
	
	for (int i = 0; i < m_bias.size(); i++) {
		output[i] += m_bias[i];
	}
	

	return output;
}
















std::vector<double> Dense::backwardPass(std::vector<double> outputGradient, float l_rate) {
	std::vector<std::vector<double>> weights_gradient;
	std::vector<double> input_gradient;

	for (double out : outputGradient) {
		std::vector<double> row;
		for (double inp : m_input) {
			row.push_back(out * inp);
		}
		weights_gradient.push_back(row);
	}

	for (std::vector<double> row : weights_gradient) {
		for (double v: row) {
		}
	}

	for (int i = 0; i < m_weights.size();i++) {
		double sum = 0;
		for (int j = 0;j < m_weights[0].size();j++) {
			sum += m_weights[i][j] * outputGradient[j];
		}
		input_gradient.push_back(sum);
	}

	for (int i = 0; i < m_weights.size();i++) {
		for (int j = 0;j < m_weights[0].size();j++) {
			m_weights[i][j] -= (l_rate * weights_gradient[j][i]);
		}
	}

	for (int i = 0;i < m_bias.size();i++) {
		m_bias[i] -= l_rate * outputGradient[i];
	}


	return input_gradient;
}






















void Dense::mutate(float l_rate, float mutation_chance) {
	for (std::vector<double>& node : m_weights) {
		for (double& weight : node) {
			if (Random::getRange(0, 1) < mutation_chance) {
				weight += Random::getRange(-l_rate, l_rate);
			}

		}
	}
}

std::vector<std::vector<double>> Dense::getWeights() {
	return m_weights;
}
std::vector<double> Dense::getBias() {
	return m_bias;
}
void Dense::setWeights(std::vector<std::vector<double>> weights) {
	this->m_weights = weights;
}
void Dense::setBias(std::vector<double> bias) {
	this->m_bias = bias;
}
int Dense::getInputs() {
	return m_inputs;
}
int Dense::getOutputs() {
	return m_outputs;
}

Dense::~Dense() {
	m_weights.clear();
	m_bias.clear();
}

Sigmoid::Sigmoid() {
	m_type = "Sigmoid";
}

std::vector<double> Sigmoid::forwardPass(std::vector<double> input) {
	m_input = input;
	std::vector<double> output;

	for (double value : input) {
		output.push_back(sigmoidFunction(value));
	}

	return output;
}

std::vector<double> Sigmoid::backwardPass(std::vector<double> outputGradient, float l_rate){

	std::vector<double> input_gradient;
	for (int i = 0; i < outputGradient.size();i++) {
		input_gradient.push_back(outputGradient[i] * sigmoidFunctionPrime(m_input[i]));
	}
	return input_gradient;
}


Tanh::Tanh() {
	m_type = "Tanh";
}

std::vector<double> Tanh::forwardPass(std::vector<double> input) {
	m_input = input;
	std::vector<double> output;

	for (double value : input) {
		output.push_back(tanhFunction(value));
	}

	return output;
}

std::vector<double> Tanh::backwardPass(std::vector<double> outputGradient, float l_rate) {

	std::vector<double> input_gradient;
	for (int i = 0; i < outputGradient.size(); i++) {
		input_gradient.push_back(outputGradient[i] * tanhFunctionPrime(m_input[i]));
	}
	return input_gradient;
}


Relu::Relu() {
	m_type = "Relu";
}

std::vector<double> Relu::forwardPass(std::vector<double> input) {
	m_input = input;
	std::vector<double> output;

	for (double value : input) {
		output.push_back(reluFunction(value));
	}

	return output;
}

std::vector<double> Relu::backwardPass(std::vector<double> outputGradient, float l_rate) {

	std::vector<double> input_gradient;
	for (int i = 0; i < outputGradient.size(); i++) {
		input_gradient.push_back(outputGradient[i] * reluFunctionPrime(m_input[i]));
	}
	return input_gradient;
}
