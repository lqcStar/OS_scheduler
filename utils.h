#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include "structs.h"
#include <list>
using namespace std;
void readData(char* res,vector<record> &records);
//void readData2(char *src,vector<banker> &banker);
void writeData(char* dest,vector<record>&records,vector<result>& results,vector<char>& execOrder);
void writeData(string dest,vector<RtResult>& result);
bool generateJob(int totalTime,int currentTime,list<RtRecord>& records);
bool compare(const RtRecord& record1,const RtRecord& record2);
//void writeData2(char* dest,int result);
#endif //UTILS_H

