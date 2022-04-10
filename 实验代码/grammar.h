#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

const int MAXN = 128;

class EXP//产生式
{
public:
	char left = '\0';//左部
	string right = "";//右部
};

class grammar 
{
public:
	int N_cnt;//非终结符个数
	int T_cnt;//终结符个数
	int G_cnt;//产生式个数
	EXP exp[MAXN];//产生式
	set<char> FIRST[MAXN];//FIRST集
	set<char> FOLLOW[MAXN];//FOLLOW集
	char N_begin;            //起始符
	char N_sym[MAXN] = { 0 };//非终结符
	char T_sym[MAXN] = { 0 };//终结符
	bool whe_find_first[MAXN] = { false };  //表示该符号是否已经找过其FIRST集
	bool whe_find_follow[MAXN] = { false };  //表示该符号是否已经找过其FOLLOW集

	grammar();
	~grammar();

	void input();   //输入文法
	bool isN(char ch);//判断一个字符是否为非终结符
	int getT(char ch);//获取终结符下标
	int getN(char ch);//获取非终结符下标
	void getFirst(char ch);//获取FIRST集
	void getFollow(char ch);//获取FOLLOW集
	void printSet();//打印文法
};


