#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

const int MAXN = 128;

class EXP//����ʽ
{
public:
	char left = '\0';//��
	string right = "";//�Ҳ�
};

class grammar 
{
public:
	int N_cnt;//���ս������
	int T_cnt;//�ս������
	int G_cnt;//����ʽ����
	EXP exp[MAXN];//����ʽ
	set<char> FIRST[MAXN];//FIRST��
	set<char> FOLLOW[MAXN];//FOLLOW��
	char N_begin;            //��ʼ��
	char N_sym[MAXN] = { 0 };//���ս��
	char T_sym[MAXN] = { 0 };//�ս��
	bool whe_find_first[MAXN] = { false };  //��ʾ�÷����Ƿ��Ѿ��ҹ���FIRST��
	bool whe_find_follow[MAXN] = { false };  //��ʾ�÷����Ƿ��Ѿ��ҹ���FOLLOW��

	grammar();
	~grammar();

	void input();   //�����ķ�
	bool isN(char ch);//�ж�һ���ַ��Ƿ�Ϊ���ս��
	int getT(char ch);//��ȡ�ս���±�
	int getN(char ch);//��ȡ���ս���±�
	void getFirst(char ch);//��ȡFIRST��
	void getFollow(char ch);//��ȡFOLLOW��
	void printSet();//��ӡ�ķ�
};


