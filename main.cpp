#include <iostream>
#include "scheduler.h"
#include "banker.h"
using namespace std;
int main() {

    char* src = "data/text.txt";
    char* src2 = "data/text2.txt";

    FCFS(src,"data/fcfs.txt");
    SJF(src,"data/sjf.txt");
    SRT(src,"data/srt.txt");
    HRRN(src,"data/hrrn.txt");
    RR(src,"data/rr.txt");
    no_PRIORITY(src2,"data/no_priority.txt");
    PRIORITY(src2,"data/priorty.txt");
    EDF("data/edf.txt");
    LLF("data/llf.txt");

    init();
    int arr0[COL] ={0,0,0};
    int arr1[COL] = {1,0,2};
    int arr2[COL] ={3,3,0};
    int arr3[COL] = {0,2,0};
    vector<int> request0(arr0,arr0+COL);
    vector<int> request1(arr1,arr1+COL);
    vector<int> request2(arr2,arr2+COL);
    vector<int> request3(arr3,arr3+COL);
    banker(request0,0);
    banker(request1,1);
    banker(request2,4);
    banker(request3,0);
    return 0;
}
