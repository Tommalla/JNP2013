#include <stack>
#include <string>
#include <functional>
#include <exception>
#include <cassert>
#include <unordered_map>
#include <set>
#include <cstdio>

// FIXME
// Const std functions?
// Typedefs
typedef std::function<int(void)> Lazy;

//Exceptions
class SyntaxError : public std::exception {
public:
    SyntaxError() {}
    const char* what() const noexcept {
	    return "Wrong syntax!";
    }
};

class OperatorAlreadyDefined : public std::exception {
public:
	OperatorAlreadyDefined() {}
	const char* what() const noexcept {
		return "Tried to redefine an operator!";
	}
};

class UnknownOperator : public std::exception {
public:
	UnknownOperator() {}
	const char* what() const noexcept {
		return "Operator not known!";
	}
};

class LazyCalculator {
private:
	std::unordered_map<char, std::function<int(Lazy, Lazy)>> operatorMap;
	std::set<char> allowed;

	int getIntValue(const char c) const {
		if (allowed.count(c) == 0)
			throw SyntaxError();
		return c - '0';
	}

	std::function<int(Lazy, Lazy)> getOperator(const char c) const {
		try {
			return operatorMap.at(c);
		} catch (std::out_of_range& e) {
			throw UnknownOperator();
		}
	}

public:
	LazyCalculator() {
		operatorMap['+'] = [] (Lazy a, Lazy b) -> int { return a() + b(); };
		operatorMap['*'] = [] (Lazy a, Lazy b) -> int { return a() * b(); };
		operatorMap['-'] = [] (Lazy a, Lazy b) -> int { return a() - b(); };
		operatorMap['/'] = [] (Lazy a, Lazy b) -> int { return a() / b(); };

		allowed.insert('0');
		allowed.insert('2');
		allowed.insert('4');
	}

	Lazy parse(const std::string& s) const {
		if (s.length() == 0)
			throw SyntaxError();

		bool op = false;
		int operand = getIntValue(s[0]);

		Lazy res = [=] () -> int {return operand;};

		for (size_t i = 1; i < s.length(); ++i, op = ~op) {
			if (op) {
				//we are expecting an operator
				auto fn = getOperator(s[i]);
				Lazy literal = [=] () -> int { return operand; };
// 				printf("Constructing: %d %c %d\n", res(), s[i], literal());
				res = [=] () -> int { return fn(res, literal); };
			} else {
				//we are expecting a literal
				operand = getIntValue(s[i]);
			}
// 			printf("%d\n", res());
		}

		return res;
	}

	int calculate(const std::string& s) const {
		return parse(s)();
	}

	void define(char c, std::function<int(Lazy, Lazy)> fn) {
		if (operatorMap.emplace(c, fn).second != true)
			throw OperatorAlreadyDefined();
	}
};

std::function<void(void)> operator*(int n, std::function<void(void)> fn) {
	return [=]() {
		for (int i = 0; i < n; i++)
			fn();
	};
}

int manytimes(Lazy n, Lazy fn) {
	(n() * fn)();  // Did you notice the type cast?
	return 0;
}

int main() {
	LazyCalculator calculator;

	// The only literals...
	assert(calculator.calculate("0") == 0);
	assert(calculator.calculate("2") == 2);
	assert(calculator.calculate("4") == 4);

	// Built-in operators.
	assert(calculator.calculate("42+") == 6);
	assert(calculator.calculate("24-") == -2);
	assert(calculator.calculate("42*") == 8);
	assert(calculator.calculate("42/") == 2);

	assert(calculator.calculate("42-2-") == 0);
	assert(calculator.calculate("242--") == 0);
	assert(calculator.calculate("22+2-2*2/0-") == 2);

	// The fun.
	calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
	assert(calculator.calculate("42!") == 42);

	std::string buffer;
	calculator.define(',', [](Lazy a, Lazy b) { a(); return b(); });
	calculator.define('P', [&buffer](Lazy a, Lazy b) { buffer += "pomidor"; return 0; });
	assert(calculator.calculate("42P42P42P42P42P42P42P42P42P42P42P42P42P42P42P42P,,,,42P42P42P42P42P,,,42P,42P,42P42P,,,,42P,,,42P,42P,42P,,42P,,,42P,42P42P42P42P42P42P42P42P,,,42P,42P,42P,,,,,,,,,,,,") == 0);
	assert(buffer.length() == 42 * std::string("pomidor").length());

	std::string buffer2 = std::move(buffer);
	buffer.clear();
	calculator.define('$', manytimes);
	assert(calculator.calculate("42!42P$") == 0);
	// Notice, how std::move worked.
	assert(buffer.length() == 42 * std::string("pomidor").length());

	calculator.define('?', [](Lazy a, Lazy b) { return a() ? b() : 0; });
	assert(calculator.calculate("042P?") == 0);
	assert(buffer == buffer2);

	assert(calculator.calculate("042!42P$?") == 0);
	assert(buffer == buffer2);

	for (auto bad: {"", "42", "4+", "424+"}) {
		try {
			calculator.calculate(bad);
			assert(false);
		} catch (SyntaxError) { }
	}

	try {
		calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
		assert(false);
	} catch (OperatorAlreadyDefined) { }

	try {
		calculator.calculate("02&");
		assert(false);
	} catch (UnknownOperator) { }
}
