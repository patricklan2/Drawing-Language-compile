#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <conio.h>
#include "scanner.h"
#include "expression.h"

class Parser {
private:
	double Parameter = 0;   //Parameter�ǲ���T�Ĵ洢�ռ䣺��¼Tÿ�μ�һ��ı仯
	double Origin_x = 0, Origin_y = 0;
	double Scale_x = 1, Scale_y = 1;
	double Rot_angle = 0;
	int Color_R = 250, Color_G = 250, Color_B = 250; //��ɫ����ɫ����ɫ����ֵ���������ص��С
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
	void DrawLoop(double Start, double end, double step,   //��ʼ��ֹ����
		ExprNode* HorPtr, TNode* x,  //��������ʽ�﷨���ĸ��ڵ�
		ExprNode* VerPtr, TNode* y); //��������ʽ�﷨���ĸ��ڵ�
};

#endif // !PARSER_H