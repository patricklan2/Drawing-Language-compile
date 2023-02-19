#include"scanner.h"
#define TOKEN_LEN 100                            //�ַ�����������С��Ϊ100�����������Ǻŵ��ַ���


//�ж�Enum��ֵ�����ض�Ӧ����
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

	//��ʼ���ʷ�������
Scanner::Scanner(const char* FileName) //����Ҫ������Դ�����ļ���
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
		return 1;     //������ڣ����ļ�������ʼ��lineNO��ֵΪ1������true
	else
		return 0;    //�����ڷ���0
}

//�رմʷ�������
void Scanner::close(void)
{
	if (InFile != NULL)
		fclose(InFile);
}

//������Դ�����ж���һ���ַ�
void Scanner::GetChar(void)
{
	Char = toupper(getc(InFile));//���Դ�����һ���ַ�
}

//��Ԥ�����ַ��˻ص�����Դ�����У������Ĺ�������ҪԤ��1��2�������ַ���Ԥ�����ַ�������ˣ��Դ˱�֤�´ζ�ʱ���ᶪ���ַ�
void Scanner::BackChar(char Char)   //���룺����һ���ַ���û�����
{
	if (Char != EOF)
		ungetc(Char, InFile);
}

//���Ѿ�ʶ����ַ��ӵ�TokenBuffer
void Scanner::AddCharTokenString()//����Դ�����һ���ַ���û�����
{
	int TokenLength = strlen(TokenBuffer);//�趨�ó���
	if (TokenLength + 1 >= sizeof(TokenBuffer))
		return;//��ʱ�ַ����ĳ��ȳ������ֵ�����ش���
	TokenBuffer[TokenLength] = Char;//���һ���ַ�
	TokenBuffer[TokenLength + 1] = '\0';
}

//��ռǺŻ�����
void Scanner::EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

//����ʶ����ַ����ڷ��ű��в�����Ӧ�ļǺ�
Token Scanner::JudgeKeyToken(const char* IDString)//���룺ʶ������ַ�����������Ǻ�
{
	int loop;
	for (loop = 0; loop < sizeof(TokenTab) / sizeof(TokenTab[0]); loop++)
		if (strcmp(TokenTab[loop].lexeme, IDString) == 0)
			return TokenTab[loop];//���ҳɹ������ظüǺ�
	Token errortoken;
	memset(&errortoken, 0, sizeof(Token));  //��ʼ��errortoken
	errortoken.type = ERRTOKEN;
	return errortoken;//����ʧ�ܣ����ش���Ǻ�
}

//���ո񡣷��ؿո��һ���ַ�
void Scanner::trim(void)
{
	while(isspace(Char))//�����ո����ôʽ���
	{	
		if (Char == '\n') // \n��ת����һ��
			LineNo++;
		GetChar();//��Դ�����ж���һ���ַ�
	}
}

void Scanner::getString(void)
{
	while(isalnum(Char))//�ж������ֻ�����ĸ
	{
		AddCharTokenString();//�Ǿͼ��뵽������
		GetChar();//���ǵĻ��ʹ����Ǵ�����ַ�����ͣ
	}
}

void Scanner::getConst(void)
{
	while(isdigit(Char))
	{
		AddCharTokenString();//��λ����λ���ϵ���
		GetChar();
	}
	if (Char == '.')//С���Ĵ���
	{
		AddCharTokenString();
		GetChar();
		while(isdigit(Char))//С���������ִ���
		{
			AddCharTokenString();
			GetChar();
		}
	}
}

int Scanner::judgeMinus(void) {
	if (Char == '-') {
		GetChar();
		if (Char == '-') {//�ж��Ƿ���ע��
			while (Char != '\n' && Char != EOF)//��ע�ͣ���һֱ��������ע��λ��
				GetChar();
			return 0;
		}
		else {//���ǵĻ������ǵ����Ǽ���
			return 1;
		}
	}
}

int Scanner::judgeDIV(void) {
	if (Char == '/') {
		GetChar();
		if (Char == '/') {//��ոյ�-��һ��Ҫ�ж��Ƿ���ע��
			while (Char != '\n' && Char != EOF)//��ע�ͣ���һֱ��������ע��λ��
				GetChar();
			return 0;
		}
		else { //���ǵĻ������ǵ����ǳ���
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

//��ȡһ���Ǻ�
Token Scanner::GetToken(void)
//�˺�����DFAת���������˺������һ���Ǻš�ÿ���øú���һ�Σ��������һ���Ǻš�
//��ˣ�Ҫ���Դ��������мǺţ���Ҫ�ظ�����������������������ĺ��������˺������ù���
//���һ���Ǻţ�û������
{
	Token token;
	memset(&token, 0, sizeof(Token));
	EmptyTokenString();//��ջ�����
	token.lexeme = TokenBuffer;//�Ǻŵ��ַ�ָ��ָ���ַ�������
	trim();
	//�����ǿո�TAB���س����ļ��������ȣ����ȼ��뵽�Ǻŵ��ַ���������
	if (isalpha(Char))  //��char��A-Za-z����һ���Ǻ������ؼ��֡�PI��E��
	{
		getString();
		token = JudgeKeyToken(TokenBuffer);//����tokenbuffer�еĺϷ����ַ�token
		token.lexeme = TokenBuffer;//���������ݸ���lexme����
	}
	else if (isdigit(Char))  //����һ�����֣���һ���ǳ���
	{
		getConst();
		token.type = CONST_ID;//����token�͸���CONST_ID
		token.value = atof(TokenBuffer);//�ַ���ת������
	}
	else    //������ĸ�����֣���һ������������߷ָ���
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
			if (judgeMinus()) { //��ͨ�ļ���
				token.type = MINUS; break;
			}
			return GetToken();
		case '/':
			if (judgeDIV() == 1) { //��ͨ�ĳ���
				token.type = DIV; break;
			}
			return GetToken();
		case '*':
			if (judgeMUL()) token.type = MUL; //�ǳ˺�
			else token.type == POWER; //�ǳ˷�
				
			break;
		default:token.type = ERRTOKEN; GetChar(); break;//��������������㣬����ERRTOKEN
		}
	}
	topToken = token;
	return token;
}

Token Scanner::getTopToken(void) {
	Token token = topToken;
	return token;
}
