#ifndef FORMULA_H
#define FORMULA_H

#include "Type.h"
#include "util/StringUtil.h"

/**
 * A formula class for the property system.
 */
class Formula {
public:
	Formula();

	/**
	 * Parses the given string as a formula.
	 * If it returns Result::SUCCESS then this is now a valid Formula.
	 * If it returns Result::FAILURE, you can call getError() to
	 * sort of see what went wrong.
	 */
	Result parse(StringRef str);
	StringRef getError();

	/** @return Whether or not this Formula can be evaluated. */
	bool isValid();

	/**
	 * Evaluates the formula and returns the result.
	 * If formula is not valid, this will return 0.
	 */
	Variant evaluate();

private:
	enum Operator {
		ADD, SUBTRACT, MULTIPLY, DIVIDE, POWER, MODULUS,
		AND, OR, EQUAL, NEQUAL, LESS, GREATER, LEQ, GEQ,
		PAREN, FAILURE};
	int getPrecidence(Operator op);
	bool isRightAssoc(Operator op);
	class AddVisitor; class SubVisitor; class MultVisitor; class DivVisitor;
	class PowVisitor; class ModVisitor; class  AndVisitor; class  OrVisitor;
	class  EqVisitor; class NeqVisitor; class LessVisitor; class LeqVisitor;

	Result parseToken(StringRef token);
	Result parseNumber(StringRef token);
	Result parseString(StringRef token);
	Result parseOperator(StringRef token);
	void pushOp(Operator op);
	Result popParen();

	Result doError(StringRef error);

	static const int OP_IDX = 3;
	typedef boost::variant<int, float, std::string, Operator> FormulaVariant;
	void operate(std::vector<FormulaVariant>& stack, Operator op);
	
	struct Token {
		enum Type {CONSTANT, VARIABLE, OPERATOR};
		Token(int num);
		Token(float num);
		Token(String str);
		Token(Operator op);
		Type type;
		FormulaVariant value;
	};

	bool valid;
	bool invalidOperation;
	String error;
	std::vector<Operator> opStack;
	std::vector<Token>  rpnStack;
};

#endif // FORMULA_H