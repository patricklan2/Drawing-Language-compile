#define _CRT_SECURE_NO_WARNINGS    //忽略安全警告
#pragma once
#define TOKEN_LEN 100  

#ifndef SCANNER_H
#define SCANNER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <graphics.h>

enum Token_Type  //枚举记号的类别
{
	ORIGIN, SCALE, ROT, IS, TO, STEP, DRAW, FOR, FROM,   //保留字
	T,                                                   //参数
	SEMICO, L_BRACKET, R_BRACKET, COMMA,                 //分隔符
	PLUS, MINUS, MUL, DIV, POWER,                        //运算符
	FUNC,                                                //函数
	CONST_ID,                                            //常数
	NONTOKEN,                                            //空记号
	ERRTOKEN,                                            //出错记号
	COLOR, blue, green, red,                             //颜色
	size                                                 //大小
};

// MathFuncPtr代表一个 指向返回double值并带有一个double形参的函数的指针的类型
typedef double(*MathFuncPtr) (double);

struct Token  //记号的数据结构,记号由类别和属性组成
{
	Token_Type type;         //记号的类别
	char* lexeme;            //属性，原始输入的字符串，是要指向TokenBuffer的指针
	double value;            //属性，为常数的值
	MathFuncPtr FuncPtr;     //属性，函数的指针
};

//符号表（字典）：数组元素的类型于记号的类型相同
//当识别出一个ID时，通过此表来确认具体是哪个记号
static Token TokenTab[] =
{
{ CONST_ID,    (char*)"PI",         3.1415926,  NULL },
{ CONST_ID,    (char*)"E",          2.71828,    NULL },
{ T,           (char*)"T",          0.0,        NULL },
{ FUNC,        (char*)"SIN",        0.0,        sin  },
{ FUNC,        (char*)"COS",        0.0,        cos  },
{ FUNC,        (char*)"TAN",        0.0,        tan  },
{ FUNC,        (char*)"LN",        0.0,         log  },
{ FUNC,        (char*)"EXP",        0.0,        exp  },
{ FUNC,        (char*)"SQRT",      0.0,        sqrt },
{ ORIGIN,      (char*)"ORIGIN",    0.0,        NULL },
{ SCALE,       (char*)"SCALE",     0.0,        NULL },
{ ROT,         (char*)"ROT",       0.0,        NULL },
{ IS,          (char*)"IS",         0.0,        NULL },
{ FOR,         (char*)"FOR",        0.0,        NULL },
{ FROM,        (char*)"FROM",      0.0,        NULL },
{ TO,          (char*)"TO",        0.0,        NULL },
{ STEP,        (char*)"STEP",       0.0,        NULL },
{ DRAW,        (char*)"DRAW",      0.0,        NULL },
{ COLOR ,      (char*)"COLOR",     0.0,         NULL},
{ blue,        (char*)"BLUE",       0.0,       NULL },
{ green,       (char*)"GREEN",     0.0,         NULL },
{ red,         (char*)"RED",        0.0,        NULL },
{ size,        (char*)"PIXELSIZE",  0.0,       NULL }
};


class Scanner {
	unsigned int LineNo;                             //记录字符所在行的行号
	FILE* InFile;                             //指向存放绘图语言程序的指针
	char TokenBuffer[TOKEN_LEN];              //设置一个字符缓冲区，用来保留记号的字符串，当需要记号的字符串时，char* lexeme指针会指向TokenBuffer
	int Char;                                 //当下读到的字符
	Token topToken;
public:
	int getLineNo() { return LineNo; }
public:
	Scanner() {};
	Scanner(const char* FileName);
	int isOpen(void);
	void close(void);
	void GetChar(void);
	void BackChar(char Char);
	void AddCharTokenString();
	void EmptyTokenString();
	void trim(void);
	void getString(void);
	void getConst(void);
	int judgeMinus(void);
	int judgeDIV(void);
	int judgeMUL(void);
	Token JudgeKeyToken(const char* IDString);
	Token GetToken(void);

	Token getTopToken(void);
};

char* GetEnum(int i);

class Error {
public:
	Error() {};
	Error(const char* imformation) { errorImformation = imformation;}
public:
	void printOut();
private:
	const char* errorImformation;
};

class ErrorFactory {
	Scanner* scanner;
public:
	ErrorFactory() {};
	ErrorFactory(Scanner* s) { scanner = s; }

	Error createError(const char* errorImformation,int type = 1);
};

#endif