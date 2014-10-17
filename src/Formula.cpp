#include "Formula.h"

#include <easylogging++.h>
#include <boost/regex.hpp>

Formula::Formula(): valid(false) { }

Result Formula::doError(StringRef theError) {
	error = theError;
	return Result::FAILURE;
}

// the equation is parsed with the Shunting-yard algorithm
Result Formula::parse(StringRef str) {
	
	// clear everything about the formula
	rpnStack.clear();
	opStack.clear();
	valid = false;
	invalidOperation = false;

	// tokenize
	static boost::regex rgx("[\\d\\w\\._]+|[!<>=]=|&&|\\|\\||\"[^\"]+\"|[^\\s]");
	boost::regex_token_iterator<std::string::const_iterator> end;
	boost::regex_token_iterator<std::string::const_iterator> iter(str.begin(), str.end(), rgx);
	while (iter != end) {
		Result res = parseToken(*iter++);
		if (res == Result::FAILURE) return Result::FAILURE;
	}

	// toss the operator stack into the rpn stack
	while (!opStack.empty()) {
		Operator op = opStack.back();
		if (op == PAREN) return doError("Mismatched parenthesis.");
		opStack.pop_back();
		rpnStack.push_back(op);
	}

	// test run the formula
	valid = true;
	Variant test = evaluate();
	if (invalidOperation) {
		valid = false;
		return doError("Invalid operation.");
	}

	return Result::SUCCESS;
}

Result Formula::parseToken(StringRef str) {
	if (str.empty()) return Result::SUCCESS;

	if (std::isdigit(str[0]) || str[0] == '.') {
		// it's assumed to be a number if it starts a digit
		return parseNumber(str);
	} else if (std::isalpha(str[0]) || str[0] == '_') {
		// it's assumed to be a variable or function if it starts with a letter or underscore
		return Result::SUCCESS;
	} else if (str[0] == '"' && str[str.size() - 1] == '"' && str.size() >= 2) {
		// it's assumed to be a string if it starts and ends with quotes
		return parseString(str);
	} else {
		// otherwise it's an operator
		return parseOperator(str);
	}
}

Result Formula::parseNumber(StringRef str) {
	try {
		if (str.find('.') != std::string::npos) {
			// is a float
			size_t idx;
			float num = std::stof(str, &idx);
			if (idx < str.size()) return doError("Unknown token: '" + str + "'");
			rpnStack.emplace_back(num);
		} else {
			// is an int
			size_t idx;
			int num = std::stoi(str, &idx, 0);
			if (idx < str.size()) return doError("Unknown token: '" + str + "'");
			rpnStack.emplace_back(num);
		}
	} catch(std::invalid_argument ex) {
		return doError("Unknown token: '" + str + "'");
	} catch(std::out_of_range ex) {
		return doError("Too big (or small): '" + str + "'");
	}
	return Result::SUCCESS;
}

Result Formula::parseString(StringRef str) {
	rpnStack.emplace_back(str.substr(1, str.size() - 2));
	return Result::SUCCESS;
}

Result Formula::parseOperator(StringRef str) {
	if (str.size() == 1) {
		switch(str[0]) {
			case '+': pushOp(ADD);      break;
			case '-': pushOp(SUBTRACT); break;
			case '*': pushOp(MULTIPLY); break;
			case '/': pushOp(DIVIDE);   break;
			case '^': pushOp(POWER);    break;
			case '%': pushOp(MODULUS);  break;
			case '<': pushOp(LESS);     break;
			case '>': pushOp(GREATER);  break;
			case '(': opStack.push_back(PAREN); break;
			case ')': return popParen();
			default: return doError("Unknown token: '" + str + "'");
		}
		return Result::SUCCESS;
	} else if (str.size() == 2) {
		if      (str == "==") pushOp(EQUAL);
		else if (str == "!=") pushOp(NEQUAL);
		else if (str == "<=") pushOp(LEQ);
		else if (str == ">=") pushOp(GEQ);
		else if (str == "&&") pushOp(AND);
		else if (str == "||") pushOp(OR);
		else doError("Unknown token: '" + str + "'");
		return Result::SUCCESS;
	}
	return doError("Unknown token: '" + str + "'");
}

// pushes an operator onto the stack with the Shunting-yard rules
void Formula::pushOp(Operator op) {
	int precidence = getPrecidence(op) + isRightAssoc(op);
	while(true) {
		if (opStack.empty() || opStack.back() == PAREN) {
			break;
		} else {
			if (precidence <= getPrecidence(opStack.back())) {
				rpnStack.emplace_back(opStack.back());
				opStack.pop_back();
			} else break;
		}
	}
	opStack.push_back(op);
}

// a closing parenthesis was reached so everything until
// the opening parenthesis is pushed onto the rpn stack
Result Formula::popParen() {
	while(true) {
		if (opStack.empty()) return doError("Mismatched parenthesis.");
		else if (opStack.back() == PAREN) {
			opStack.pop_back();
			break;
		} else {
			rpnStack.emplace_back(opStack.back());
			opStack.pop_back();
		}
	}
	return Result::SUCCESS;
}

int Formula::getPrecidence(Operator op) {
	switch(op) {
		case PAREN: case FAILURE:
			return 0;
		case AND: case OR:
			return 2;
		case EQUAL: case NEQUAL:
			return 4;
		case LESS: case GREATER: case LEQ: case GEQ:
			return 6;
		case ADD: case SUBTRACT:
			return 8;
		case MULTIPLY: case DIVIDE: case MODULUS:
			return 10;
		case POWER:
			return 12;
	}
	return 0;
}
bool Formula::isRightAssoc(Operator op) {
	return op == POWER;
}

Formula::Token::Token(int num):     type(Token::CONSTANT), value(num) { }
Formula::Token::Token(float num):   type(Token::CONSTANT), value(num) { }
Formula::Token::Token(String str):  type(Token::CONSTANT), value(str) { }
Formula::Token::Token(Operator op): type(Token::OPERATOR), value(op)  { }

StringRef Formula::getError() {
	return error;
}
bool Formula::isValid() {
	return valid;
}

// runs the formula and returns the result
Variant Formula::evaluate() {
	if (!valid) return 0;
	
	std::vector<FormulaVariant> theStack;
	for (size_t i = 0; i < rpnStack.size(); i++) {
		Token& token = rpnStack[i];
		switch(token.type) {
			case Token::CONSTANT:
				theStack.push_back(token.value);
				break;
			case Token::OPERATOR:
				operate(theStack, boost::get<Operator>(token.value));
				if (invalidOperation) return 0;
				break;
			default: break;
		}
	}

	return theStack.back();
}


// Each visitor class is for a different operator.
// Using boost::static_visitor I am supposed define a function for every
// possible combination of types for these operators.
// I take a shortcut using SFINAE. If the operation is defined for the
// given types, it simply performs it. If it is not,
// an error occurs (or in the case of addition, concatination occurs.)
struct Formula::AddVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto add(A a, B b, bool) const -> decltype(a + b) {return a + b;}
	template<class A, class B>
	auto add(A a, B b, int)  const -> std::string {
		std::stringstream ss;
		ss << a << b;
		return ss.str();
	}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return add(a, b, true);
	}
};
struct Formula::SubVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto sub(A a, B b, bool) const -> decltype(a - b) {return a - b;}
	template<class A, class B> auto sub(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return sub(a, b, true);
	}
};
struct Formula::MultVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto mult(A a, B b, bool) const -> decltype(a * b) {return a * b;}
	template<class A, class B> auto mult(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return mult(a, b, true);
	}
};
struct Formula::DivVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto div(A a, B b, bool) const -> decltype(a / b) {return a / b;}
	template<class A, class B> auto div(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return div(a, b, true);
	}
};
struct Formula::PowVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto pow(A a, B b, bool) const -> decltype(a * b) {
		return std::pow((float)a, (float)b);
	}
	template<class A, class B> auto pow(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return pow(a, b, true);
	}
};
struct Formula::ModVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto mod(A a, B b, bool) const -> decltype(a % b) {return a % b;}
	template<class A, class B> auto mod(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return mod(a, b, true);
	}
};
struct Formula::AndVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto och(A a, B b, bool) const -> decltype(a && b) {return a && b;}
	template<class A, class B> auto och(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return och(a, b, true);
	}
};
struct Formula::OrVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto oar(A a, B b, bool) const -> decltype(a || b) {return a || b;}
	template<class A, class B> auto oar(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return oar(a, b, true);
	}
};
struct Formula::EqVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto eq(A a, B b, bool) const -> decltype(a == b) {return a == b;}
	template<class A, class B> auto eq(A a, B b, int)  const -> bool {return false;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return eq(a, b, true);
	}
};
struct Formula::NeqVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto neq(A a, B b, bool) const -> decltype(a != b) {return a != b;}
	template<class A, class B> auto neq(A a, B b, int)  const -> bool {return true;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return neq(a, b, true);
	}
};
struct Formula::LessVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto less(A a, B b, bool) const -> decltype(a < b) {return a < b;}
	template<class A, class B> auto less(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return less(a, b, true);
	}
};
struct Formula::LeqVisitor: public boost::static_visitor<FormulaVariant> {
	template<class A, class B> auto leq(A a, B b, bool) const -> decltype(a <= b) {return a <= b;}
	template<class A, class B> auto leq(A a, B b, int)  const -> Operator {return FAILURE;}
	template<class A, class B>
	FormulaVariant operator()(A a, B b) const {
		return leq(a, b, true);
	}
};
void Formula::operate(std::vector<FormulaVariant>& stack, Operator op) {
	if (stack.size() < 2) {
		invalidOperation = true;
		return;
	}

	FormulaVariant b = stack.back();
	stack.pop_back();
	FormulaVariant a = stack.back();
	stack.pop_back();
	
	if (b.which() == OP_IDX || a.which() == OP_IDX) {
		invalidOperation = true;
		return;
	}

	FormulaVariant result;
	switch(op) {
		case ADD:      result = boost::apply_visitor( AddVisitor(), a, b); break;
		case SUBTRACT: result = boost::apply_visitor( SubVisitor(), a, b); break;
		case MULTIPLY: result = boost::apply_visitor(MultVisitor(), a, b); break;
		case DIVIDE:   result = boost::apply_visitor( DivVisitor(), a, b); break;
		case POWER:    result = boost::apply_visitor( PowVisitor(), a, b); break;
		case MODULUS:  result = boost::apply_visitor( ModVisitor(), a, b); break;
		case AND:      result = boost::apply_visitor( AndVisitor(), a, b); break;
		case OR:       result = boost::apply_visitor(  OrVisitor(), a, b); break;
		case EQUAL:    result = boost::apply_visitor(  EqVisitor(), a, b); break;
		case NEQUAL:   result = boost::apply_visitor( NeqVisitor(), a, b); break;
		case LESS:     result = boost::apply_visitor(LessVisitor(), a, b); break;
		case GREATER:  result = boost::apply_visitor(LessVisitor(), b, a); break;
		case LEQ:      result = boost::apply_visitor( LeqVisitor(), a, b); break;
		case GEQ:      result = boost::apply_visitor( LeqVisitor(), b, a); break;
		case PAREN: case FAILURE: result = 0; break;
	}

	if (result.which() == OP_IDX) {
		invalidOperation = true;
		return;
	}

	stack.push_back(result);
}
