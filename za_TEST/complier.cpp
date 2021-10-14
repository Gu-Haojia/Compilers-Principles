#include <iostream>
#include <cstring>
#include <fstream>
#include <stdio.h>
using namespace std;

static char ReserveWord[32][15] = 
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

// 搜索保留字
int searchReserve(char ReserveWord[][20], char *s)
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(ReserveWord[i], s) == 0)
        {
            return i + 1;//返回种别码
        }
    }
    return -1;//否则返回-1，代表查找不成功，即为标识符
}

// 判断字符
bool IsLetter(char ch)
{
    if (ch >='a' && ch<='z'|| ch >='A' && ch<='Z' || ch=='_')
    {
        return true;
    }
    else 
        return false;

}

// 判断数字
bool IsDigit(char ch)
{
    if ( ch<='9' && ch>='0')
        return true;
    else
        return false;
}

//去掉注释
void annotation_dispose(char resourse[],int p_program)
{
    char annotation[10][1000];
    int n_anno = 0;
    int count = 0;
    char temp_resource[10000];

    for(int i =0;i<=p_program;i++)
    {
        if(resourse[i]=='/' && resourse[i+1]=='/')
        {
            i += 2;
            int k =0;
            while(resourse[i]!= '\n')
            {
                annotation[n_anno][k++] = resourse[i];
                i++; 
            }
            n_anno++;
        }

        if(resourse[i]=='/' && resourse[i+1]=='*')
        {
            i += 2;
            int k = 0;
            while( !(resourse[i]=='*' && resourse[i+1]=='/') )
            {              
                annotation[n_anno][k++] = resourse[i];
                if (resourse[i] ==EOF)
                {
                    cout<<"error annotation!";
                    exit(0);
                }
                i++;
            }
            resourse[i] = ' ';
            resourse[i+1] = ' ';
            n_anno++;
        }
        temp_resource[count++] = resourse[i];
    }
    strcpy(resourse,temp_resource);
    cout<<"annotation:"<<endl;
    for(int i = 0;i<n_anno;i++)
    {
        cout<<"annotation["<<i<<"]:"<<annotation[i]<<endl;
    }

}


int main()
{
    char ResourceProgram[10000];
    char token[20];
    int syn = -1;
    int p_program = 0; //源程序指针


    FILE *fp;
    if ((fp = fopen("D:\\CG\\Code_VS\\TEST\\test1.txt", "r")) == NULL)
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
    ResourceProgram[++p_program] = '\0';
    fclose(fp);


    annotation_dispose(ResourceProgram,p_program);
    cout<<"-------------------"<<endl;
    //cout << endl << "去掉注释后:" << endl;
    cout << ResourceProgram << endl;
    return 0;
    
}

