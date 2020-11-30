#ifndef OSSCHEDULER_PLUS_BANKER_H
#define OSSCHEDULER_PLUS_BANKER_H
#include <vector>
#define ROW 5
#define COL 3
using namespace std;


void init();
void banker(vector<int> requst,int row);
bool isSecurity();
bool allocate(vector<int> request,int row);
void rollBack(vector<int>request, int row);
#endif //OSSCHEDULER_PLUS_BANKER_H

