#include "grammar.h"
#pragma once


grammar::grammar(){
	input();
	for (int i = 0; i < N_cnt; i++)
	{
		getFirst(N_sym[i]);
	}
	for (int i = 0; i < N_cnt; i++)
	{
		getFollow(N_sym[i]);
	}
}

grammar::~grammar() {
}

void grammar::input() {
	cout << "请输入起始符: ";
	cin >> N_begin;
	cout << "请输入非终结符个数: ";
	cin >> N_cnt;
	cout << "请输入" << N_cnt << "个非终结符: ";
	for (int i = 0; i < N_cnt; i++) {
		cin >> N_sym[i];
		//起始符号的FOLLOW集需要多添加一个$
		if (N_sym[i] == N_begin)
			FOLLOW[getN(N_begin)].insert('$');
	}
	cout << "请输入终结符个数: ";
	cin >> T_cnt;
	cout << "请输入" << T_cnt << "个终结符: ";
	for (int i = 0; i < T_cnt; i++) {
		cin >> T_sym[i];
	}
	T_sym[T_cnt] = '$';
	T_cnt++;
	cout << "请输入产生式个数: ";
	cin >> G_cnt; 
	cout << "请输入" << G_cnt << "个产生式: ";
	for (int i = 0; i < G_cnt; i++) {
		char tmp;
		cin >> exp[i].left;
		cin >> tmp >> tmp;
		cin >> exp[i].right;
	}
}

bool grammar::isN(char ch) {
	for (int i = 0; i < N_cnt; i++) {
		if (ch == N_sym[i])
			return true;
	}
	return false;
}

int grammar::getT(char ch) {
	for (int i = 0; i < T_cnt; i++) {
		if (ch == T_sym[i])
			return i;
	}
	return -1;
}

int grammar::getN(char ch) {
	for (int i = 0; i < N_cnt; i++) {
		if (ch == N_sym[i])
			return i;
	}
	return -1;
}

void grammar::getFirst(char ch) {
	//如果之前已经找过该符号的FIRST集，则退出
	if (whe_find_first[ch])
		return;
	bool nulls_flag = false;   //用于存储First集是否有空串$
	int total = 0;             //记录非终结符产生$的个数
	int loc_ch = getN(ch);
	for (int i = 0; i < G_cnt; i++) {
		//如果当前产生式左部是需要找FIRST集的非终结符
		if (ch == exp[i].left) {
			for (int j = 0; j < (int)exp[i].right.length(); j++) {
				//如果当前字符是终结符
				if (!isN(exp[i].right[j])) {
					FIRST[loc_ch].insert(exp[i].right[j]);
					break;
				}
				//如果是非终结符，则求它的FIRST集
				getFirst(exp[i].right[j]);
				//把该非终结符的FIRST集加到当前非终结符的FIRST集中
				set<char>::iterator it;
				int locj = getN(exp[i].right[j]);
				for (it = FIRST[locj].begin(); it != FIRST[locj].end(); it++) {
					if (*it == '#') {
						nulls_flag = true;
					}
					else {
						FIRST[loc_ch].insert(*it);
					}
				}
				//如果没有空串则不必再找下一个符号了
				if (!nulls_flag)
					break;
				else {
					nulls_flag = false;
					total++;
				}
			}
			//如果右部所有符号的First集都有空串,则ch的First集也有空串 
			if (total == exp[i].right.length())
				FIRST[loc_ch].insert('#');
		}
	}
	whe_find_first[ch] = true;
}

void grammar::getFollow(char ch) {
	//如果之前已经找过该符号的FOLLOW集，则退出
	if (whe_find_follow[ch])
		return;
	int loc_ch = getN(ch);
	for (int i = 0; i < G_cnt; i++) {
		int index = -1;
		int rlen = exp[i].right.length();
		//找当前字符在产生式中的位置
		for (int j = 0; j < rlen; j++) {
			if (ch == exp[i].right[j]) {
				index = j;
				break;
			}
		}
		//如果当前产生式找到了ch，且ch不在最后一个位置
		if (index != -1 && index < rlen - 1) {
			int total = 0;
			bool nulls_flag = false;
			char current_ch;
			for (int j = index + 1; j < exp[i].right.length(); j++) {
				current_ch = exp[i].right[j];
				//如果是终结符，则加入到FOLLOW集中
				if (!isN(current_ch)) {
					FOLLOW[loc_ch].insert(current_ch);
					break;
				}
				//如果是非终结符，则把非终结符的除空的FRIST集加入FOLLOW中
				else {
					set<char>::iterator it;
					for (it = FIRST[getN(current_ch)].begin(); it != FIRST[getN(current_ch)].end(); it++)
					{
						if (*it == '#')
							nulls_flag = true;
						else
							FOLLOW[loc_ch].insert(*it);
					}
					//如果该符号不会变成空，则不需要再遍历后面的符号
					if (!nulls_flag)
						break;
					else {
						nulls_flag = false;
						total++;
					}
				}
			}
			//如果后面可以生成空串，则加入产生式左边的符号的FOLLOW集
			if (total == (exp[i].right.length() - index - 1)) {
				char temp = exp[i].left;
				getFollow(temp);
				set<char>::iterator it;
				for (it = FOLLOW[getN(temp)].begin(); it != FOLLOW[getN(temp)].end(); it++)
					FOLLOW[loc_ch].insert(*it);
			}
		}
		//如果当前产生式ch在最后一个位置，则加左部的FOLLOW集,注意要避免循环递归的情况
		else if (index == rlen - 1 && ch != exp[i].left) {
			char temp = exp[i].left;
			getFollow(temp);
			set<char>::iterator it;
			for (it = FOLLOW[getN(temp)].begin(); it != FOLLOW[getN(temp)].end(); it++)
				FOLLOW[loc_ch].insert(*it);
		}
	}
	whe_find_follow[ch] = true;
}
	
void grammar::printSet() {
	set<char>::iterator it;
	cout << "First集:" << endl;
	for (int i = 0; i < N_cnt; i++)
	{
		cout << N_sym[i] << ": { ";
		for (it = FIRST[i].begin(); it != FIRST[i].end(); it++) {
			cout << *it;
			it++;
			if (it != FIRST[i].end())
				cout << ",";
			it--;
		}
		cout << " }" << endl;
	}
	cout << "Follow集:" << endl;
	for (int i = 0; i < N_cnt; i++)
	{
		cout << N_sym[i] << ": { ";
		for (it = FOLLOW[i].begin(); it != FOLLOW[i].end(); it++) {
			cout << *it;
			it++;
			if (it != FOLLOW[i].end())
				cout << ",";
			it--;
		}
		cout << " }" << endl;
	}
}
