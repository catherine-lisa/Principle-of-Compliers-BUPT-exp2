#include "analyze_table.h"

analyze_table::analyze_table() {
	for (int i = 0; i < MAXN; i++) {
		for (int j = 0; j < MAXN; j++) {
			analyze_t[i][j] = -1;
		}
	}
	getAT();
}
analyze_table::~analyze_table() {
}

void analyze_table::getAT() {
	for (int i = 0; i < G_cnt; i++) {
		int nulls_flag = false;
		int total = 0;
		char temp;
		for (int j = 0; j < exp[i].right.length(); j++) {
			temp = exp[i].right[j];
			//������ս��
			if (!isN(temp)) { 
				//�������ʽ�Ҳ��ǿգ����ǣ������ᴦ�����FOLLOW��
				if (temp == '#') {
					nulls_flag = true;
					total++;
				}
				//���ս�����ǿմ�������Ԥ�������
				else {
					analyze_t[getN(exp[i].left)][getT(temp)] = i;
					detect[getN(exp[i].left)][getT(temp)] ++;
					break;
				}
			}
			//����Ƿ��ս��,�������FIRST��
			else {  
				set<char>::iterator it;
				for (it = FIRST[getN(temp)].begin(); it != FIRST[getN(temp)].end(); it++)
				{
					//��FIRST�������еķǿ�Ԫ��a����M[A,a]��
					if (*it == '#') {
						nulls_flag = true;
						total++;
					}
					else {
						analyze_t[getN(exp[i].left)][getT(*it)] = i;
						detect[getN(exp[i].left)][getT(*it)] ++;
					}	
				}
			}
			if (!nulls_flag)
				break;
			else
				nulls_flag = true;
		}
		//���ұ߿���ȫ��Ϊ�գ�������󲿵�follow��
		if (total == exp[i].right.length()) {
			set<char>::iterator it;
			for (it = FOLLOW[getN(exp[i].left)].begin();it != FOLLOW[getN(exp[i].left)].end(); it++)
			{
				detect[getN(exp[i].left)][getT(*it)]++;
				analyze_t[getN(exp[i].left)][getT(*it)] = i;
			}
		}
	}
}

void analyze_table::printAT() {
	cout << "Ԥ�����������:" << endl << "\t";
	for (int i = 0; i < T_cnt; i++) {
		cout << T_sym[i] << "\t";
	}
	cout << endl;
	for (int i = 0; i < N_cnt; i++) {
		cout << N_sym[i] << "\t";
		for (int j = 0; j < T_cnt; j++) {
			if (analyze_t[i][j] >= 0) {
				cout << exp[analyze_t[i][j]].left << "->" << exp[analyze_t[i][j]].right << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}

bool analyze_table::judgeL1() {
	for (int i = 0; i < MAXN; i++)
		for (int j = 0; j < MAXN; j++)
		{
			if (detect[i][j] > 1)
			{
				cout << "���ķ�����LL(1)�ķ�!" << endl;
				return false;
			}
		}
	cout << "���ķ���LL(1)�ķ�!" << endl;
	return true;
}

void analyze_table::inputS() {
	cout << "������Ҫ�������ַ���: ";
	cin >> str;
}

void analyze_table::analyze() {
	cout << "ջ\t\t\t" << "����\t\t\t" << "���" << endl;
	remainder.push_back('$');
	for (int i = str.length() - 1; i >= 0; i--) {
		remainder.push_back(str[i]);
	}
	stack.push_back('$');
	stack.push_back(N_begin);
	while (!remainder.empty()) {
		string outS = "";
		for (int i = 0; i < stack.size(); i++)
		{
			outS += stack[i];
		}
		if (outS.length() >= 8)
			cout << outS << "\t\t";
		else
			cout << outS << "\t\t\t";//�������ջ����,$ջ�׵�ջ��
		outS = "";
		for (int i = remainder.size() - 1; i >= 0; i--)
		{
			outS += remainder[i];
		}
		if (outS.length() >= 8)
			cout << outS << "\t\t";
		else
			cout << outS << "\t\t\t"; 
		
		char stack_top = stack[stack.size() - 1];//��ʾ��ǰջ��Ԫ��
		char remainder_top = remainder[remainder.size() - 1];//��ʾʣ������ĵ�һ���ַ�

		if (stack_top == remainder_top && stack_top == '$') {
			cout << "acc" << endl;
			return;
		}
		if (stack_top == remainder_top) {//remainder_top��Ϊ�ս��
			stack.pop_back();
			remainder.pop_back();
			cout << "ƥ��" << endl;
		}
		else if (analyze_t[getN(stack_top)][getT(remainder_top)] != -1) {
			int loc = analyze_t[getN(stack_top)][getT(remainder_top)];
			stack.pop_back();
			//�������ʽ�Ҳ಻Ϊ�գ������򽫲���ʽ��ʽѹ��ջ��
			if (exp[loc].right != "#") {
				for (int i = exp[loc].right.length() - 1; i >= 0; i--) {
					stack.push_back(exp[loc].right[i]);
				}
			}
			cout << exp[loc].left << "->" << exp[loc].right << endl;
		}
		else {
			cout << "error!" << endl;
			return;
		}
	}

}