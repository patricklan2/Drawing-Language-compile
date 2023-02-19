#include"scanner.h"//���Դʷ�����������ȷ��
#include"expression.h"
#include"parser.h"
#include"drawBoard.h"
using namespace std;

void test_scanner() {
    Token token;
    char file[] = "draw1.txt";
    Scanner scanner = Scanner(file);
    if (!scanner.isOpen())                                    //��ʼ���ʷ�������
    {
        printf("Open Sorce File Error !\n");
        return;
    }
    printf("�Ǻ����         �ַ���        ����ֵ           ����ָ��\n");
    printf("--------------------------------------------\n");
    while (true)
    {
        //token = scanner.GetToken();//���һ���Ǻ�
        scanner.GetToken();
        token = scanner.getTopToken();
        if (token.type != NONTOKEN)//�Ǻŵ�����Ǵ��󣬾ʹ�ӡ����������
            printf("%12s,%12s,%12f,%12x\n", GetEnum(token.type), token.lexeme, token.value, token.FuncPtr);
        else
            break;
    }
    printf("-------------------------------------------\n");
    scanner.close();//�رմʷ�������
    //system("pause");
}

int main()
{
    const char* filename = "draw.txt";
    DrawBoard drawBoard = DrawBoard();
    drawBoard.draw(filename);
    system("pause");
    return 0;
}