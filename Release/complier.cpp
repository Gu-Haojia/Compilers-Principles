#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <fstream>
#include <stdio.h>
#define PATH "./test1.txt"
#define SIZE 100000
using namespace std;

string keyword[] = {"auto", "break", "case", "char", "const", "continue",
                    "default", "do", "double", "else", "enum", "extern",
                    "float", "for", "goto", "if", "int", "long",
                    "register", "return", "short", "signed", "sizeof", "static",
                    "struct", "switch", "typedef", "union", "unsigned", "void",
                    "volatile", "while"}; //keyword表

int lookup(string s) //查找s是否在保留字表keyword中
{

    for (int i = 0; i < (sizeof(keyword) / sizeof(keyword[0])); i++)
    {
        if (s.compare(keyword[i]) == 0)
            return 1; //是保留字，返回1
    }
    return 0; //不是保留字，返回0
}

// 搜索保留字
int searchReserve(char ReserveWord[][20], char *s)
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(ReserveWord[i], s) == 0)
        {
            return i + 1; //返回种别码
        }
    }
    return -1; //否则返回-1，代表查找不成功，即为标识符
}

// 判断字符
bool IsLetter(char ch)
{
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_')
    {
        return true;
    }
    else
        return false;
}

// 判断数字
bool IsDigit(char ch)
{
    if (ch <= '9' && ch >= '0')
        return true;
    else
        return false;
}

//去掉注释
void annotation_dispose(char resourse[], int p_program)
{
    char annotation[10][100];
    int n_anno = 0;
    int count = 0;
    char temp_resource[SIZE];

    for (int i = 0; i <= p_program; i++)
    {
        if (resourse[i] == '/' && resourse[i + 1] == '/')
        {
            i += 2;
            int k = 0;
            while (resourse[i] != '\n')
            {
                annotation[n_anno][k++] = resourse[i];
                i++;
            }
            annotation[n_anno][k] = 0;
            n_anno++;
        }

        if (resourse[i] == '/' && resourse[i + 1] == '*')
        {
            i += 2;
            int k = 0;
            while (!(resourse[i] == '*' && resourse[i + 1] == '/'))
            {
                annotation[n_anno][k++] = resourse[i];
                if (resourse[i] == EOF)
                {
                    cout << "error annotation!";
                    exit(0);
                }
                i++;
            }
            resourse[i] = ' ';
            resourse[i + 1] = ' ';
            n_anno++;
        }
        temp_resource[count++] = resourse[i];
    }
    temp_resource[count] = '\0';
    strcpy(resourse, temp_resource);
    cout << "annotation:" << endl;
    for (int i = 0; i < n_anno; i++)
    {
        cout << "annotation[" << i << "]:" << annotation[i] << endl;
    }
}

int main()
{
    char ResourceProgram[SIZE];
    char filepath[] = PATH;
    int p_program = 0; //源程序指针

    FILE *fp;
    if ((fp = fopen(filepath, "r")) == NULL)
    {
        cout << "Can't open this file";
        exit(0);
    }

    ResourceProgram[p_program] = fgetc(fp);
    while (ResourceProgram[p_program] != EOF) //读取文件到结尾时返回EOF
    {
        p_program++;
        ResourceProgram[p_program] = fgetc(fp); //将源程序读入ResourceProgram[]数组
    }
    ResourceProgram[p_program] = '\0';
    fclose(fp);

    annotation_dispose(ResourceProgram, p_program);
    cout << "-------------------" << endl;
    cout << "Analysis：" << endl;

    fstream newFile("temp.txt", ios::out);
    newFile.close();
    fstream out("temp.txt", ios::in | ios::out);
    out << ResourceProgram;
    out.seekg(0L, ios::beg);

    string s;
    char ch;

    ch = out.get();

    while (!out.eof())
    {
        if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}')
        {
            //识别分界符 4
            cout << "<" << ch << ",delimiter>\n";
            ch = out.get();
        }
        else if (isdigit(ch))
        {
            //识别整数 5 / 实数 6
            s = "";
            while (isdigit(ch))
            {
                s += ch;
                ch = out.get();
                if (ch == '.')
                {
                    //出现小数点，有可能是常实数
                    s += ch;
                    ch = out.get();
                    if (isdigit(ch))
                    {
                        while (isdigit(ch))
                        {
                            s += ch;
                            ch = out.get();
                        }

                        cout << "<" << s << ",Float number>\n"; //常实数
                        break;
                    }
                    else
                        cout << s + " is error!\n"; //如果小数点后没有数字，则报错
                }
                if (!isdigit(ch))
                {
                    cout << "<" << s << ",Constant integer>\n"; //常整数
                    break;
                }
            }
        }
        else if (isalpha(ch) || ch == '_')
        {
            //识别标识符 1 / 保留字 2
            s = "";
            while (isalpha(ch) || isdigit(ch) || ch == '_')
            {
                s += ch;
                ch = out.get();
            }
            if (lookup(s))
            {
                // 在keyword表中查找s是否是保留字
                cout << "<" << s << ",Keyword>\n"; //保留字
            }
            else
                cout << "<" << s << ",Identifier>\n"; //标识符
        }
        //后面都是运算符 3
        else if (ch == '>')
        {
            s = "";
            s += ch;
            ch = out.get();
            if (ch == '=')
            {
                //">="
                s += ch;
                ch = out.get();
                cout << "<" << s << ",Double bounder>\n";
            }
            else
                cout << "<" << s << ",Bounder>\n";
        }
        else if (ch == '<')
        {
            s = "";
            s += ch;
            ch = out.get();
            if (ch == '=')
            {
                //"<="
                s += ch;
                ch = out.get();
                cout << "<" << s << ",Double bounder>\n";
            }
            else
                cout << "<" << s << ",Bounder>\n";
        }
        else if (ch == '=')
        {
            s = "";
            s += ch;
            ch = out.get();
            if (ch == '=')
            {
                //"=="
                s += ch;
                ch = out.get();
                cout << "<" << s << ",Double bounder>\n";
            }
            else
                cout << "<" << s << ",Bounder>\n";
        }

        else if (ch == '!')
        {
            s = "";
            s += ch;
            ch = out.get();
            if (ch == '=')
            {
                //"!="
                s += ch;
                ch = out.get();
                cout << "<" << s << ",Double bounder>\n";
            }
            else
                cout << "<" << s << ",Bounder>\n";
        }
        else
        { //跳过当前单词
            ch = out.get();
        }
    }
    return 0;
}
