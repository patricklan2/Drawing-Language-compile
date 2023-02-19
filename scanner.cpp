#include"scanner.h"
#define TOKEN_LEN 100                            //字符缓冲区，大小设为100，用来保留记号的字符串


//判断Enum的值，返回对应名称
char* GetEnum(int i)
{
	char* name = (char*)malloc(sizeof(char) * 10);
	switch (i)
	{
	case 0:strcpy(name, "ORIGIN"); break;
	case 1:strcpy(name, "SCALE"); break;
	case 2:strcpy(name, "ROT"); break;
	case 3:strcpy(name, "IS"); break;
	case 4:strcpy(name, "TO"); break;
	case 5:strcpy(name, "STEP"); break;
	case 6:strcpy(name, "DRAW"); break;
	case 7:strcpy(name, "FOR"); break;
	case 8:strcpy(name, "FROM"); break;
	case 9:strcpy(name, "T"); break;
	case 10:strcpy(name, "SEMICO"); break;
	case 11:strcpy(name, "L_BRACKET"); break;
	case 12:strcpy(name, "R_BRACKET"); break;
	case 13:strcpy(name, "COMMA"); break;
	case 14:strcpy(name, "PLUS"); break;
	case 15:strcpy(name, "MINUS"); break;
	case 16:strcpy(name, "MUL"); break;
	case 17:strcpy(name, "DIV"); break;
	case 18:strcpy(name, "POWER"); break;
	case 19:strcpy(name, "FUNC"); break;
	case 20:strcpy(name, "CONST_ID"); break;
	case 21:strcpy(name, "NONTOKEN"); break;
	case 22:strcpy(name, "ERRTOKEN"); break;
	case 23:strcpy(name, "COLOR"); break;
	case 24:strcpy(name, "BLUE"); break;
	case 25:strcpy(name, "GREEN"); break;
	case 26:strcpy(name, "RED"); break;
	case 27:strcpy(name, "PIXELSIZE"); break;
	}
	return name;
}

	//初始化词法分析器
Scanner::Scanner(const char* FileName) //输入要分析的源程序文件名
{
	LineNo = 1;
	InFile = fopen(FileName, "r");
	if (InFile == NULL) {
		throw Error("the file is not exist!");
	}
	GetChar();
}

int Scanner::isOpen(void)
{
	if (InFile != NULL)
		return 1;     //如果存在，打开文件，并初始化lineNO的值为1，返回true
	else
		return 0;    //不存在返回0
}

//关闭词法分析器
void Scanner::close(void)
{
	if (InFile != NULL)
		fclose(InFile);
}

//从输入源程序中读入一个字符
void Scanner::GetChar(void)
{
	Char = toupper(getc(InFile));//输出源程序的一个字符
}

//把预读的字符退回到输入源程序中，分析的过程中需要预读1、2……个字符，预读的字符必须回退，以此保证下次读时不会丢掉字符
void Scanner::BackChar(char Char)   //输入：回退一个字符，没有输出
{
	if (Char != EOF)
		ungetc(Char, InFile);
}

//把已经识别的字符加到TokenBuffer
void Scanner::AddCharTokenString()//输入源程序的一个字符，没有输出
{
	int TokenLength = strlen(TokenBuffer);//设定好长度
	if (TokenLength + 1 >= sizeof(TokenBuffer))
		return;//此时字符串的长度超过最大值，返回错误
	TokenBuffer[TokenLength] = Char;//添加一个字符
	TokenBuffer[TokenLength + 1] = '\0';
}

//清空记号缓冲区
void Scanner::EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

//根据识别的字符串在符号表中查找相应的记号
Token Scanner::JudgeKeyToken(const char* IDString)//输入：识别出的字符串；输出：记号
{
	int loop;
	for (loop = 0; loop < sizeof(TokenTab) / sizeof(TokenTab[0]); loop++)
		if (strcmp(TokenTab[loop].lexeme, IDString) == 0)
			return TokenTab[loop];//查找成功，返回该记号
	Token errortoken;
	memset(&errortoken, 0, sizeof(Token));  //初始化errortoken
	errortoken.type = ERRTOKEN;
	return errortoken;//查找失败，返回错误记号
}

//读空格。返回空格后一个字符
void Scanner::trim(void)
{
	while(isspace(Char))//遇到空格代表该词结束
	{	
		if (Char == '\n') // \n则转至下一行
			LineNo++;
		GetChar();//从源程序中读出一个字符
	}
}

void Scanner::getString(void)
{
	while(isalnum(Char))//判断是数字或者字母
	{
		AddCharTokenString();//是就加入到缓冲区
		GetChar();//不是的话就代表是错误的字符，暂停
	}
}

void Scanner::getConst(void)
{
	while(isdigit(Char))
	{
		AddCharTokenString();//两位或两位以上的数
		GetChar();
	}
	if (Char == '.')//小数的处理
	{
		AddCharTokenString();
		GetChar();
		while(isdigit(Char))//小数点后的数字处理
		{
			AddCharTokenString();
			GetChar();
		}
	}
}

int Scanner::judgeMinus(void) {
	if (Char == '-') {
		GetChar();
		if (Char == '-') {//判断是否是注释
			while (Char != '\n' && Char != EOF)//是注释，则一直读到不是注释位置
				GetChar();
			return 0;
		}
		else {//不是的话，就是单纯是减号
			return 1;
		}
	}
}

int Scanner::judgeDIV(void) {
	if (Char == '/') {
		GetChar();
		if (Char == '/') {//与刚刚的-号一样要判断是否是注释
			while (Char != '\n' && Char != EOF)//是注释，则一直读到不是注释位置
				GetChar();
			return 0;
		}
		else { //不是的话，就是单纯是除号
			return 1;
		}
	}
}

int Scanner::judgeMUL(void) {
	if (Char == '*') {
		GetChar();
		if (Char == '*') {
			GetChar();
			return 0;
		}
		else {
			return 1;
		}
	}
}

//获取一个记号
Token Scanner::GetToken(void)
//此函数由DFA转化而来。此函数输出一个记号。每调用该函数一次，仅仅获得一个记号。
//因此，要获得源程序的所有记号，就要重复调用这个函数。上面声明的函数都被此函数调用过！
//输出一个记号，没有输入
{
	Token token;
	memset(&token, 0, sizeof(Token));
	EmptyTokenString();//清空缓冲区
	token.lexeme = TokenBuffer;//记号的字符指针指向字符缓冲区
	trim();
	//若不是空格、TAB、回车、文件结束符等，则先加入到记号的字符缓冲区中
	if (isalpha(Char))  //若char是A-Za-z，则一定是函数，关键字、PI、E等
	{
		getString();
		token = JudgeKeyToken(TokenBuffer);//对于tokenbuffer中的合法的字符token
		token.lexeme = TokenBuffer;//将它的内容赋给lexme属性
	}
	else if (isdigit(Char))  //若是一个数字，则一定是常量
	{
		getConst();
		token.type = CONST_ID;//数字token就赋给CONST_ID
		token.value = atof(TokenBuffer);//字符串转浮点数
	}
	else    //不是字母和数字，则一定是运算符或者分隔符
	{
		switch (Char)
		{
		case EOF:token.type = NONTOKEN; break;
		case ';':token.type = SEMICO; GetChar(); break;
		case '(':token.type = L_BRACKET; GetChar(); break;
		case ')':token.type = R_BRACKET; GetChar(); break;
		case ',':token.type = COMMA; GetChar(); break;
		case '+':token.type = PLUS; GetChar(); break;
		case '-':
			if (judgeMinus()) { //普通的减号
				token.type = MINUS; break;
			}
			return GetToken();
		case '/':
			if (judgeDIV() == 1) { //普通的除号
				token.type = DIV; break;
			}
			return GetToken();
		case '*':
			if (judgeMUL()) token.type = MUL; //是乘号
			else token.type == POWER; //是乘方
				
			break;
		default:token.type = ERRTOKEN; GetChar(); break;//以上情况均不满足，则是ERRTOKEN
		}
	}
	topToken = token;
	return token;
}

Token Scanner::getTopToken(void) {
	Token token = topToken;
	return token;
}
