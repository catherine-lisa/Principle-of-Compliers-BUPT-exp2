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
	cout << "��������ʼ��: ";
	cin >> N_begin;
	cout << "��������ս������: ";
	cin >> N_cnt;
	cout << "������" << N_cnt << "�����ս��: ";
	for (int i = 0; i < N_cnt; i++) {
		cin >> N_sym[i];
		//��ʼ���ŵ�FOLLOW����Ҫ�����һ��$
		if (N_sym[i] == N_begin)
			FOLLOW[getN(N_begin)].insert('$');
	}
	cout << "�������ս������: ";
	cin >> T_cnt;
	cout << "������" << T_cnt << "���ս��: ";
	for (int i = 0; i < T_cnt; i++) {
		cin >> T_sym[i];
	}
	T_sym[T_cnt] = '$';
	T_cnt++;
	cout << "���������ʽ����: ";
	cin >> G_cnt; 
	cout << "������" << G_cnt << "������ʽ: ";
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
	//���֮ǰ�Ѿ��ҹ��÷��ŵ�FIRST�������˳�
	if (whe_find_first[ch])
		return;
	bool nulls_flag = false;   //���ڴ洢First���Ƿ��пմ�$
	int total = 0;             //��¼���ս������$�ĸ���
	int loc_ch = getN(ch);
	for (int i = 0; i < G_cnt; i++) {
		//�����ǰ����ʽ������Ҫ��FIRST���ķ��ս��
		if (ch == exp[i].left) {
			for (int j = 0; j < (int)exp[i].right.length(); j++) {
				//�����ǰ�ַ����ս��
				if (!isN(exp[i].right[j])) {
					FIRST[loc_ch].insert(exp[i].right[j]);
					break;
				}
				//����Ƿ��ս������������FIRST��
				getFirst(exp[i].right[j]);
				//�Ѹ÷��ս����FIRST���ӵ���ǰ���ս����FIRST����
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
				//���û�пմ��򲻱�������һ��������
				if (!nulls_flag)
					break;
				else {
					nulls_flag = false;
					total++;
				}
			}
			//����Ҳ����з��ŵ�First�����пմ�,��ch��First��Ҳ�пմ� 
			if (total == exp[i].right.length())
				FIRST[loc_ch].insert('#');
		}
	}
	whe_find_first[ch] = true;
}

void grammar::getFollow(char ch) {
	//���֮ǰ�Ѿ��ҹ��÷��ŵ�FOLLOW�������˳�
	if (whe_find_follow[ch])
		return;
	int loc_ch = getN(ch);
	for (int i = 0; i < G_cnt; i++) {
		int index = -1;
		int rlen = exp[i].right.length();
		//�ҵ�ǰ�ַ��ڲ���ʽ�е�λ��
		for (int j = 0; j < rlen; j++) {
			if (ch == exp[i].right[j]) {
				index = j;
				break;
			}
		}
		//�����ǰ����ʽ�ҵ���ch����ch�������һ��λ��
		if (index != -1 && index < rlen - 1) {
			int total = 0;
			bool nulls_flag = false;
			char current_ch;
			for (int j = index + 1; j < exp[i].right.length(); j++) {
				current_ch = exp[i].right[j];
				//������ս��������뵽FOLLOW����
				if (!isN(current_ch)) {
					FOLLOW[loc_ch].insert(current_ch);
					break;
				}
				//����Ƿ��ս������ѷ��ս���ĳ��յ�FRIST������FOLLOW��
				else {
					set<char>::iterator it;
					for (it = FIRST[getN(current_ch)].begin(); it != FIRST[getN(current_ch)].end(); it++)
					{
						if (*it == '#')
							nulls_flag = true;
						else
							FOLLOW[loc_ch].insert(*it);
					}
					//����÷��Ų����ɿգ�����Ҫ�ٱ�������ķ���
					if (!nulls_flag)
						break;
					else {
						nulls_flag = false;
						total++;
					}
				}
			}
			//�������������ɿմ�����������ʽ��ߵķ��ŵ�FOLLOW��
			if (total == (exp[i].right.length() - index - 1)) {
				char temp = exp[i].left;
				getFollow(temp);
				set<char>::iterator it;
				for (it = FOLLOW[getN(temp)].begin(); it != FOLLOW[getN(temp)].end(); it++)
					FOLLOW[loc_ch].insert(*it);
			}
		}
		//�����ǰ����ʽch�����һ��λ�ã�����󲿵�FOLLOW��,ע��Ҫ����ѭ���ݹ�����
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
	cout << "First��:" << endl;
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
	cout << "Follow��:" << endl;
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
