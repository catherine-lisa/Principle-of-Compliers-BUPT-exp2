#pragma once
#include "grammar.h"

class analyze_table:public grammar {
public:
	string str = "";//���������ַ���
	int detect[MAXN][MAXN] = {};//������λ�ò���ʽ�ĸ����������ж��Ƿ���LL(1)�ķ�
	int analyze_t[MAXN][MAXN];//����Ԥ���
	vector<char> stack;       //����ջ
	vector<char>  remainder;  //���뻺����
	analyze_table();
	~analyze_table();
	void getAT();  //����Ԥ�������
	void printAT();//���Ԥ�������
	bool judgeL1();//�ж��Ƿ�ΪLL(1)�ķ�
	void inputS(); //�����ַ���
	void analyze();//����Ԥ�����
};

