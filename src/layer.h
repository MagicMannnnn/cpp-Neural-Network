#pragma once

#include <vector>
#include <string>

class Layer {
public:
	Layer(int inputs, int outputs);
	Layer();
	virtual std::vector<double> forwardPass(std::vector<double> input);
	virtual std::vector<double> backwardPass(std::vector<double> outputGradient, float l_rate);
	virtual void mutate(float l_rate, float mutation_chance);
	virtual std::vector<std::vector<double>> getWeights();
	virtual std::vector<double> getBias();
	virtual void setWeights(std::vector<std::vector<double>>);
	virtual void setBias(std::vector<double>);
	virtual int getInputs();
	virtual int getOutputs();
	~Layer();
	std::string m_type;
protected:
	int m_inputs = 0;
	int m_outputs = 0;
	std::vector<double> m_input;
};

class Dense : public Layer {
public:
	Dense(int inputs, int outputs);
	std::vector<double> forwardPass(std::vector<double> input);
	std::vector<double> backwardPass(std::vector<double> outputGradient, float l_rate);
	void mutate(float l_rate, float mutation_chance);
	std::vector<std::vector<double>> getWeights();
	std::vector<double> getBias();
	void setWeights(std::vector<std::vector<double>>);
	void setBias(std::vector<double>);
	int getInputs();
	int getOutputs();
	~Dense();
private:
	std::vector<std::vector<double>> m_weights;
	std::vector<double> m_bias;
};

class Sigmoid : public Layer {
public:
	Sigmoid();
	std::vector<double> forwardPass(std::vector<double> input);
	std::vector<double> backwardPass(std::vector<double> outputGradient, float l_rate);
};

class Tanh : public Layer {
public:
	Tanh();
	std::vector<double> forwardPass(std::vector<double> input);
	std::vector<double> backwardPass(std::vector<double> outputGradient, float l_rate);
};

class Relu : public Layer {
public:
	Relu();
	std::vector<double> forwardPass(std::vector<double> input);
	std::vector<double> backwardPass(std::vector<double> outputGradient, float l_rate);
};