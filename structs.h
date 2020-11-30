#ifndef STRUCTS_H
#define STRUCTS_H
#include <string>
using namespace std;
//#define PROGRESS 5     //进程数量
//#define REC_NUM  3     //资源种类数量
typedef struct record Record;
struct record{
    char job;
    int arrivalTime;
    int serviceTime;
    int priority;
};

typedef struct result Result;
struct result{
    int startTime;
    int finishTime;
    int turnTime;
    double weightTurnTime;
};

typedef struct realTimeRecord RtRecord;
struct realTimeRecord{
    string job;
    int arrivalTime;
    int deadline;
    int remainingTime;
    int periodTime;
    int status;

};


typedef struct realTimeResult RtResult;
struct realTimeResult{
    string job;
    int finishTime;
};
//typedef struct banker Banker;
//struct banker{
//    int Available[PROGRESS];
//    int sign[PROGRESS];
//    int work[PROGRESS][REC_NUM];
//    int workAll[PROGRESS][REC_NUM];
//    int Max[PROGRESS][REC_NUM];
//    int Allocation[PROGRESS][REC_NUM];
//    int Need[PROGRESS][REC_NUM];
//};


#endif //STRUCTS_H

