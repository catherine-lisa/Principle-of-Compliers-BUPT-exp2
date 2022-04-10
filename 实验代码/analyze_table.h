#pragma once
#include "grammar.h"

class analyze_table:public grammar {
public:
	string str = "";//待分析的字符串
	int detect[MAXN][MAXN] = {};//计数该位置产生式的个数，用于判断是否是LL(1)文法
	int analyze_t[MAXN][MAXN];//分析预测表
	vector<char> stack;       //分析栈
	vector<char>  remainder;  //输入缓冲区
	analyze_table();
	~analyze_table();
	void getAT();  //生成预测分析表
	void printAT();//输出预测分析表
	bool judgeL1();//判断是否为LL(1)文法
	void inputS(); //输入字符串
	void analyze();//分析预测过程
};

