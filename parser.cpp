#include"parser.h"

//�ݹ��½�����

Token Parser::getNextToken() {
    return scanner->GetToken();
}

Token Parser::getTopToken() {
    return scanner->getTopToken();
}

void Parser::matchToken(Token_Type token) {
    if (getTopToken().type != token) {
        throw errorFactory.createError(GetEnum(token));
    }
    else {
        getNextToken();
    }
}

Parser::Parser(const char* FILE) {
    scanner = new Scanner(FILE);
    expression = Expression();
    errorFactory = ErrorFactory(scanner);
    getNextToken();
}

double Parser::getExpressionValue() {
    ExprNode* node = expression.getTree(scanner);
    double value = node->getValue();
    delete node;
    return value;
}

void Parser::compile() {
    while (getTopToken().type != NONTOKEN) {
        Statement();
        matchToken(SEMICO);
    }
    printf("successfully compile\n");
}

void Parser::Statement() {
    printf("Enter in Statement\n");
    switch (getTopToken().type)
    {
    case ORIGIN:   
        OriginStatement(); break;
    case SCALE:   
        ScaleStatement(); break;
    case ROT:   
        RotStatement(); break;
    case FOR:   
        ForStatement(); break;
    case COLOR:  
        ColorStatement(); break;
    case size: 
        PixSizeStatement(); break;
    default: throw errorFactory.createError("Stat");//�����ϵľ���ʾ������Ϣ
    }
    printf("Exit from Statement\n");
}

void Parser::OriginStatement() {
    printf("Enter in Original Statement\n");
    matchToken(ORIGIN);
    matchToken(IS);
    matchToken(L_BRACKET);
    Origin_x = getExpressionValue();
    matchToken(COMMA);
    Origin_y = getExpressionValue();
    matchToken(R_BRACKET);
    printf("Exit from Origin Statement\n");
}

void Parser::RotStatement(){
    printf("Enter in Rot Statement\n");
    matchToken(ROT);
    matchToken(IS);
    Rot_angle = getExpressionValue();     //��ȡ��ת�Ƕ�
    //�ͷŲ�����ת�Ƕ��﷨����ռ�ռ�
    printf("Exit from Rot Statement\n");
}

void Parser::ScaleStatement(){
    printf("Enter in Scale Statement\n");
    matchToken(SCALE);
    matchToken(IS);
    matchToken(L_BRACKET);
    Scale_x = getExpressionValue();   //��ȡ������ı�������
    matchToken(COMMA);
    Scale_y =getExpressionValue();//��ȡ������ı�������
    matchToken(R_BRACKET);
    printf("Exit from Scale Statement\n");
}

void Parser::ColorStatement(){
    Token_Type token_tmp;
    struct ExprNode* value_ptr;
    matchToken(COLOR);
    matchToken(IS);
    switch (getTopToken().type)
    {//��ɫ��Ϊ����
     //��һ��ֱ��ָ����ɫ���죬�̣�����
    case red:Color_R = 255; Color_B = 0; Color_G = 0; getNextToken();  break;
    case green:Color_R = 0; Color_B = 0; Color_G = 255; getNextToken(); break;
    case blue:Color_R = 0; Color_B = 255; Color_G = 0; getNextToken();  break;
        //�ڶ���ͨ��RGB��r,g,b��������ɫ
    case L_BRACKET:
        matchToken(L_BRACKET);
        Color_R = getExpressionValue();
        matchToken(COMMA);
        Color_G =  getExpressionValue();
        matchToken(COMMA);
        Color_B =  getExpressionValue();
        matchToken(R_BRACKET);
        break;
    default: break;
    }
    printf(" RGB(%d,%d,%d)\n", Color_R, Color_G, Color_B);
    printf("Exit from Color Statement\n");
}

void Parser::PixSizeStatement(){
    matchToken(size);
    matchToken(IS);
    pixelsize = getExpressionValue();
    printf("Exit from PixelSize Statement\n");
}

void Parser::ForStatement(){
    double start, end, step;
    ExprNode* x_Func; ExprNode* y_Func;

    printf("Enter in For Statement\n");
    //�������ս���͵�����Ӧ�ӳ���
    matchToken(FOR);
    matchToken(T);
    matchToken(FROM);
    start = getExpressionValue();   //������������ʽ��ֵ
    matchToken(TO);
    end = getExpressionValue();  //��������յ���ʽ��ֵ 
    matchToken(STEP);
    step = getExpressionValue();   //��������������ʽ��ֵ  
    matchToken(DRAW);
    matchToken(L_BRACKET);
    x_Func = expression.getTree(scanner);
    TNode* x_t = expression.getTNode();
    matchToken(COMMA);
    y_Func = expression.getTree(scanner);
    TNode* y_t = expression.getTNode();
    matchToken(R_BRACKET);
    DrawLoop(start, end, step, x_Func, x_t, y_Func, y_t);
}

//���㱻���Ƶ������
void Parser::CalcCoord(double HorCord, double VerCord,   //��������
    double& Hor_x, double& Ver_y)  //���������ֵ���𷵻�ֵ������
{
    HorCord *= Scale_x;
    VerCord *= Scale_y;   //�Ƚ��б����任
    double Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);//��Ҫ�����м����Hor_tmp������HorCord��ֵ����ǰ�ı�
    VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
    HorCord = Hor_tmp;    //�ٽ�����ת�任
    HorCord += Origin_x;
    VerCord += Origin_y;   //������ƽ�Ʊ任
    Hor_x = HorCord;
    Ver_y = VerCord;    //���µ�x��yֵ����Hor_x,Ver_y
}

//ѭ�����Ƶ������
void Parser::DrawLoop(double start,double end,double step,   //��ʼ��ֹ����
    ExprNode* x_Func, TNode* x,  //��������ʽ�﷨���ĸ��ڵ�
    ExprNode* y_Func, TNode* y) //��������ʽ�﷨���ĸ��ڵ�
{
    double X,Y;
    for (double param = start; param <= end; param += step)//��T�ڷ�Χ�ڵ�ÿһ��ֵ�������
    {
        x->setValue(param);
        y->setValue(param);
        CalcCoord(x_Func->getValue(), y_Func->getValue(), X, Y);  //����Ҫ���Ƶ��ʵ������
        setfillcolor(RGB(Color_R, Color_G, Color_B));//����pixelsize�����Կ���ͨ����Բ����Ϊ�ı������
        solidcircle(X,Y, int(pixelsize / 2));//Բ�İ뾶Ӧ����Ϊ���ش�С��һ��
    }
}
