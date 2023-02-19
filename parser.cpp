#include"parser.h"

//递归下降分析

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
    default: throw errorFactory.createError("Stat");//不符合的就提示错误信息
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
    Rot_angle = getExpressionValue();     //获取旋转角度
    //释放参数旋转角度语法树所占空间
    printf("Exit from Rot Statement\n");
}

void Parser::ScaleStatement(){
    printf("Enter in Scale Statement\n");
    matchToken(SCALE);
    matchToken(IS);
    matchToken(L_BRACKET);
    Scale_x = getExpressionValue();   //获取横坐标的比例因子
    matchToken(COMMA);
    Scale_y =getExpressionValue();//获取纵坐标的比例因子
    matchToken(R_BRACKET);
    printf("Exit from Scale Statement\n");
}

void Parser::ColorStatement(){
    Token_Type token_tmp;
    struct ExprNode* value_ptr;
    matchToken(COLOR);
    matchToken(IS);
    switch (getTopToken().type)
    {//颜色分为两类
     //第一类直接指定颜色（红，绿，蓝）
    case red:Color_R = 255; Color_B = 0; Color_G = 0; getNextToken();  break;
    case green:Color_R = 0; Color_B = 0; Color_G = 255; getNextToken(); break;
    case blue:Color_R = 0; Color_B = 255; Color_G = 0; getNextToken();  break;
        //第二类通过RGB（r,g,b）定义颜色
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
    //遇到非终结符就调用相应子程序
    matchToken(FOR);
    matchToken(T);
    matchToken(FROM);
    start = getExpressionValue();   //计算参数起点表达式的值
    matchToken(TO);
    end = getExpressionValue();  //计算参数终点表达式的值 
    matchToken(STEP);
    step = getExpressionValue();   //计算参数步长表达式的值  
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

//计算被绘制点的坐标
void Parser::CalcCoord(double HorCord, double VerCord,   //横纵坐标
    double& Hor_x, double& Ver_y)  //点横纵坐标值，起返回值的作用
{
    HorCord *= Scale_x;
    VerCord *= Scale_y;   //先进行比例变换
    double Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);//需要引入中间变量Hor_tmp，否则HorCord的值会提前改变
    VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
    HorCord = Hor_tmp;    //再进行旋转变换
    HorCord += Origin_x;
    VerCord += Origin_y;   //最后进行平移变换
    Hor_x = HorCord;
    Ver_y = VerCord;    //将新的x，y值赋给Hor_x,Ver_y
}

//循环绘制点的坐标
void Parser::DrawLoop(double start,double end,double step,   //起始终止步长
    ExprNode* x_Func, TNode* x,  //横坐标表达式语法树的根节点
    ExprNode* y_Func, TNode* y) //纵坐标表达式语法树的根节点
{
    double X,Y;
    for (double param = start; param <= end; param += step)//把T在范围内的每一个值带入计算
    {
        x->setValue(param);
        y->setValue(param);
        CalcCoord(x_Func->getValue(), y_Func->getValue(), X, Y);  //计算要绘制点的实际坐标
        setfillcolor(RGB(Color_R, Color_G, Color_B));//处理pixelsize，所以可以通过画圆来作为改变的像素
        solidcircle(X,Y, int(pixelsize / 2));//圆的半径应该设为像素大小的一半
    }
}
