#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "scanner.h"

void tip(Token token);

int match(Token_Type token1, Token_Type token2);

typedef double(*Func)(double);

class ExprNode {
public:
	enum Token_Type opCode;
	ExprNode(Token_Type op) :opCode(op) {}
	virtual ~ExprNode() {}
	virtual double getValue() = 0;
	virtual void print() = 0;
};

class BinaryNode : public ExprNode {
public:
	BinaryNode(Token_Type op, ExprNode* l, ExprNode* r) :
		ExprNode(op), Left(l), Right(r) {}
	~BinaryNode() { delete Left; delete Right; }
	ExprNode* Left, * Right;
};

class UnaryNode :public ExprNode {
public:
	UnaryNode(Token_Type op, ExprNode* ch) : ExprNode(op), Child(ch) {}
	~UnaryNode() { delete Child; printf("UnaryNode\n"); }
	ExprNode* Child;
};

class ConstNode :public ExprNode {
public:
	ConstNode(double v) :ExprNode(CONST_ID), value(v) {}
	double getValue() { return value; }
	void print() { printf("CONST:%lf\n", value); }
	double value;
};

class TNode :public ExprNode {
public:
	TNode() :ExprNode(T) {}
	double getValue() { return value; }
	void setValue(double v) { value = v; }
	void print() { printf("T\n"); }
	double value;
};

class BaseOpNode : public BinaryNode {
public:
	BaseOpNode(Token_Type op, ExprNode* l, ExprNode* r) :BinaryNode(op, l, r) {}
	void print() { printf("%s\n", GetEnum(opCode)); Left->print(); Right->print(); }
	double getValue();
};

class UBaseOpNode :public UnaryNode {
public:
	UBaseOpNode(Token_Type op, ExprNode* ch) :UnaryNode(op, ch) {}
	void print() { printf("%s\n", GetEnum(opCode)); Child->print(); }
	double getValue();
};

class FuncNode : public UBaseOpNode {
public:
	FuncNode(Func f, ExprNode* ch) : UBaseOpNode(FUNC, ch), func(f) {}
	void print() { printf("Fun:%s\n", GetEnum(opCode)); Child->print(); }
	Func func;
	double getValue() { return func(Child->getValue()); }
};

// 调用语法分析器
class Expression {
	Scanner* scanner;
	TNode* tNode = NULL;
	ErrorFactory errorFactory;
public:
	Expression() {}
	Expression(Scanner* s) { scanner = s; errorFactory = ErrorFactory(s); }
public:
	Token getNextToken();
	Token getTopToken();

	int match(Token_Type token1, Token_Type token2);
	ExprNode* getExpression();
	ExprNode* getExpression2(ExprNode* node1);
	ExprNode* getTerm();
	ExprNode* getTerm2(ExprNode* node1);
	ExprNode* getFactor();
	ExprNode* getComponent();
	ExprNode* getAtom();
	ExprNode* getTree(Scanner* s);

	TNode* getTNode() { return tNode; }
};
#endif // !EXPRESSION_H