
#include "lsystem.h"
#include <stack>
#include <memory>
#include <iostream>

std::string LindenmayerSystemDeterministic::expandSymbol(unsigned char const& sym) {
	/*============================================================
		TODO 1.1
		For a given symbol in the sequence, what should it be replaced with after expansion?
	*/
	if (sym == 'X')
		return "F[+X]F[-X]+X";
	else if (sym == 'F')
		return "FF";
	else return {char(sym)};

	
	//============================================================

	/*
	You may find useful:
		map.find: Iterator to an element with key equivalent to key. If no such element is found, past-the-end (see end()) iterator is returned.
		http://en.cppreference.com/w/cpp/container/unordered_map/find
	*/
}

std::string LindenmayerSystem::expandOnce(std::string const& symbol_sequence) {
	/*============================================================
		TODO 1.2
		Perform one iteration of grammar expansion on `symbol_sequence`.
		Use the expandSymbol method
	*/
	std::string tmpStr = symbol_sequence;
	std::string result = "";
	while (tmpStr.length() > 0) {
		result += expandSymbol(tmpStr[0]);
		tmpStr.erase(0, 1);
	}
	return result;

	//============================================================
}

std::string LindenmayerSystem::expand(std::string const& initial, uint32_t num_iters) {
	/*============================================================
		TODO 1.3
		Perform `num_iters` iterations of grammar expansion (use expandOnce)
	*/
	std::string result = initial;
	for (num_iters; num_iters > 0; num_iters--)
		result += expandOnce(result);
	return result;
	
	//============================================================
}

std::vector<Segment> LindenmayerSystem::draw(std::string const& symbols) {
	std::vector<Segment> lines; // this is already initialized as empty vector
	/*============================================================
		TODO 2
		Build line segments according to the sequence of symbols
		The initial position is (0, 0) and the initial direction is "up" (0, 1)
		Segment is std::pair<vec2, vec2>

		you can create a Segment as {p1, p2}, where p1 and p2 are vec2, e.g.:
		lines.push_back({p1, p2});

		You may also find std::stack useful:

		https://en.cppreference.com/w/cpp/container/stack

		There also is a mat2 class in utils/vec.* you may find useful for
		implementing rotations.
	*/
	double x1 = 0, y1 = 0, x2 = 0, y2 = 1;
	lines.push_back({ (x1, y1), (x2, y2) });
	std::stack<double> tmpStack;
	std::string tmpStr = symbols;
	double alpha = 0;
	while (tmpStr.length() > 0) {
		x1 = x2;
		y1 = y2;
		if (tmpStr[0] == '+') {
			alpha += rotation_angle_deg;
		}
		if (tmpStr[0] == '-') {
			alpha -= rotation_angle_deg;
		}
		if (tmpStr[0] == '[' && tmpStack.empty) {
			tmpStack.push(x1);
			tmpStack.push(y1);
			tmpStack.push(x2);
			tmpStack.push(y2);
		}
		if (tmpStr[0] == ']' && !tmpStack.empty) {
			x1 = tmpStack.pop;
			x1 = tmpStack.pop;
			x2 = tmpStack.pop;
			y2 = tmpStack.pop;
		}
		else {
			y2 += sin(alpha);
			x2 += cos(alpha);
			lines.push_back({ (x1, y1), (x2, y2) });
		}
		tmpStr.erase(0, 1);
	}

	return lines;
	//============================================================
}

std::string LindenmayerSystemStochastic::expandSymbol(unsigned char const& sym) {
	/*============================================================
		TODO 4
		For a given symbol in the sequence, what should it be replaced with after expansion?
		(stochastic case)

		Use dice.roll() to get a random number between 0 and 1
	*/
	if (sym == 'F') {
		double random = dice.roll();
		if (random > 0.5)
			return "F-F";
		else return "F+F";
	}
	else return { char(sym) };

	//============================================================
}

void LindenmayerSystemDeterministic::addRuleDeterministic(unsigned char sym, std::string const& expansion) {
	rules[sym] = expansion;
}

void LindenmayerSystemStochastic::addRuleStochastic(unsigned char sym, std::vector<StochasticRule> expansions_with_ps) {
	rules[sym] = expansions_with_ps;
}
