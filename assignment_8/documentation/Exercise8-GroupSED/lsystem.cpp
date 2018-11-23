
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
        result = expandOnce(result);
	
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

	// Create position and direction and push them on the stack
	vec2 position = vec2(0, 0);
	vec2 direction = vec2(0, 1);  // upwards

	double winkel = rotation_angle_deg * 3.14159265 / 180;

	std::stack<Segment> tmpStack;
	std::string tmpStr = symbols;

	while (tmpStr.length() > 0) {

		if (tmpStr[0] == '+') {
			direction = operator*(mat2(cos(winkel), -sin(winkel), sin(winkel), cos(winkel)), direction);
		} else if (tmpStr[0] == '-') {
			direction = operator*(mat2(cos(-winkel), -sin(-winkel), sin(-winkel), cos(-winkel)), direction);
		} else if (tmpStr[0] == '[') {
			tmpStack.push({position, direction});
		} else if (tmpStr[0] == ']' && !tmpStack.empty()) {
			position = tmpStack.top().first;
			direction = tmpStack.top().second;
			tmpStack.pop();
		} else {
			lines.push_back({position, position+direction});
			position += direction;
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
