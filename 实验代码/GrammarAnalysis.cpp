#include "analyze_table.h"
using namespace std;

int main(){
    analyze_table t;
    t.printSet();
    t.printAT();
    if (t.judgeL1()) {
        t.inputS();
        t.analyze();
    }
    return 0;
}
