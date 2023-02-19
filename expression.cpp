#include"expression.h"

void tip(Token token) {
	printf("%12s,%12s,%12f,%12x\n", GetEnum(token.type), token.lexeme, token.value, token.FuncPtr);
}

int Expression::match(Token_Type token1, Token_Type token2) {
	if (token1 != token2) {
		throw errorFactory.createError(GetEnum(token1));
	}
}

double BaseOpNode::getValue() {
	double l = Left->getValue(), r = Right->getValue();
	switch (opCode)
	{
	case POWER:
		return pow(l, r);
	case MUL:
		return l * r;
	case DIV:
		return l / r;
	case PLUS:
		return l + r;
	case MINUS:
		return l - r;
	default:
		throw Error("BaseOpNode");
	}
}

double UBaseOpNode::getValue() {
	switch (opCode)
	{
	case PLUS:
		return Child->getValue();
	case MINUS:
		return -Child->getValue();
	default:
		break;
	}
}

Token Expression::getNextToken() {
	return scanner->GetToken();
}

Token Expression::getTopToken() {
	return scanner->getTopToken();
}

ExprNode* Expression::getAtom() {
	ExprNode* result;
	switch (getTopToken().type) {
	case CONST_ID:
		result = new ConstNode(getTopToken().value); getNextToken(); break;
	case T:
		result = tNode;
		getNextToken(); break;
	case FUNC: {
		Func func = getTopToken().FuncPtr; getNextToken();
		match(L_BRACKET, getTopToken().type); getNextToken();
		ExprNode* node = getExpression();
		match(R_BRACKET, getTopToken().type); getNextToken();
		result = new FuncNode(func, node); break; }
	case L_BRACKET: {
		getNextToken();
		ExprNode* node = getExpression();
		match(R_BRACKET, getTopToken().type); getNextToken();
		result = node; break; }
	default:
		throw errorFactory.createError("Atom");
	}
	return result;
}

ExprNode* Expression::getComponent() {
	ExprNode* node1 = getAtom();
	if (getTopToken().type == POWER) {
		getNextToken();
		ExprNode* node2 = getComponent();
		node1 = new BaseOpNode(POWER, node1, node2);
	}
	return node1;
}

ExprNode* Expression::getFactor() {
	switch (getTopToken().type)
	{
	case CONST_ID: case T: case FUNC: case L_BRACKET:
		return getComponent();
	case PLUS: case MINUS: {
		Token_Type op = getTopToken().type;
		getNextToken();
		ExprNode* F = getFactor();
		return new UBaseOpNode(op, F);
	}
	default:
		throw errorFactory.createError("Factor");
	}
}

ExprNode* Expression::getTerm() {
	ExprNode* node1 = getFactor();
	return getTerm2(node1);
}

ExprNode* Expression::getTerm2(ExprNode* node1) {
	switch (getTopToken().type)
	{
	case MUL: case DIV:
		Token_Type op = getTopToken().type;
		getNextToken();
		ExprNode* node2 = getFactor();
		return getTerm2(new BaseOpNode(op, node1, node2));
	}
	return node1;
}

ExprNode* Expression::getExpression() {
	ExprNode* node1 = getTerm();
	return getExpression2(node1);
}

ExprNode* Expression::getExpression2(ExprNode* node1) {
	switch (getTopToken().type)
	{
	case PLUS: case MINUS:
		Token_Type op = getTopToken().type;
		getNextToken();
		ExprNode* node2 = getTerm();
		return getExpression2(new BaseOpNode(op, node1, node2));
	}
	return node1;
}

ExprNode* Expression::getTree(Scanner* s) {
	scanner = s;
	tNode = new TNode();
	return getExpression();
}