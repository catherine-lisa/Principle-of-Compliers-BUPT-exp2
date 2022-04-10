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
			//如果是终结符
			if (!isN(temp)) { 
				//如果产生式右侧是空，则标记，后续会处理加入FOLLOW集
				if (temp == '#') {
					nulls_flag = true;
					total++;
				}
				//该终结符不是空串，更新预测分析表
				else {
					analyze_t[getN(exp[i].left)][getT(temp)] = i;
					detect[getN(exp[i].left)][getT(temp)] ++;
					break;
				}
			}
			//如果是非终结符,则遍历其FIRST集
			else {  
				set<char>::iterator it;
				for (it = FIRST[getN(temp)].begin(); it != FIRST[getN(temp)].end(); it++)
				{
					//把FIRST集中所有的非空元素a放入M[A,a]中
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
		//若右边可以全部为空，则遍历左部的follow集
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
	cout << "预测分析表如下:" << endl << "\t";
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
				cout << "该文法不是LL(1)文法!" << endl;
				return false;
			}
		}
	cout << "该文法是LL(1)文法!" << endl;
	return true;
}

void analyze_table::inputS() {
	cout << "请输入要分析的字符串: ";
	cin >> str;
}

void analyze_table::analyze() {
	cout << "栈\t\t\t" << "输入\t\t\t" << "输出" << endl;
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
			cout << outS << "\t\t\t";//输出分析栈内容,$栈底到栈顶
		outS = "";
		for (int i = remainder.size() - 1; i >= 0; i--)
		{
			outS += remainder[i];
		}
		if (outS.length() >= 8)
			cout << outS << "\t\t";
		else
			cout << outS << "\t\t\t"; 
		
		char stack_top = stack[stack.size() - 1];//表示当前栈顶元素
		char remainder_top = remainder[remainder.size() - 1];//表示剩余输入的第一个字符

		if (stack_top == remainder_top && stack_top == '$') {
			cout << "acc" << endl;
			return;
		}
		if (stack_top == remainder_top) {//remainder_top恒为终结符
			stack.pop_back();
			remainder.pop_back();
			cout << "匹配" << endl;
		}
		else if (analyze_t[getN(stack_top)][getT(remainder_top)] != -1) {
			int loc = analyze_t[getN(stack_top)][getT(remainder_top)];
			stack.pop_back();
			//如果产生式右侧不为空，则逆序将产生式右式压入栈中
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