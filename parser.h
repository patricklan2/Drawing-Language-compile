#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <conio.h>
#include "scanner.h"
#include "expression.h"

class Parser {
private:
	double Parameter = 0;   //Parameter是参数T的存储空间：记录T每次加一点的变化
	double Origin_x = 0, Origin_y = 0;
	double Scale_x = 1, Scale_y = 1;
	double Rot_angle = 0;
	int Color_R = 250, Color_G = 250, Color_B = 250; //红色，绿色，蓝色参数值，还有像素点大小
	int pixelsize = 1;

	Scanner* scanner;
	Expression expression;
	ErrorFactory errorFactory;

public:
	Parser(const char* FILE);

	Token getNextToken();
	Token getTopToken();
	void matchToken(Token_Type token);
	double getExpressionValue();

	void compile();
	void Statement();
	void OriginStatement();
	void RotStatement();
	void ScaleStatement();
	void ColorStatement();
	void PixSizeStatement();
	void ForStatement();

public:
	void CalcCoord(double HorCord, double VerCord, double& Hor_x, double& Ver_y);
	void DrawLoop(double Start, double end, double step,   //起始终止步长
		ExprNode* HorPtr, TNode* x,  //横坐标表达式语法树的根节点
		ExprNode* VerPtr, TNode* y); //纵坐标表达式语法树的根节点
};

#endif // !PARSER_H