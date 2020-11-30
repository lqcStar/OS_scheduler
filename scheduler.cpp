#include <iostream>
#include <queue>

#include "scheduler.h"


using namespace std;
void FCFS(char* src,char* dest){
    vector<Record> records;
    readData(src,records);
    int size = records.size();
    vector<Result> results(size);
    vector<int> remaining(size);
    vector<char> execOrder;
    queue<int> queue;
    vector<int> isStart(size,0);
    int totalTime = 0;

    for(int i=0; i<size; i++){
        remaining[i] = records[i].serviceTime;
    }


    int i = 0; //records
    int current = 0;
    while(totalTime >= 0){

        if((i < size) && (records[i].arrivalTime <= totalTime)){
            queue.push(i);
            i++;
        }

        current = queue.front();
        execOrder.push_back(records[current].job);
        if(!isStart[current]){
            results[current].startTime = totalTime;
            isStart[current] = 1;
        }

        totalTime++;
        remaining[current] -= 1;
        if(remaining[current] == 0){
            results[current].finishTime = totalTime;
            results[current].turnTime  = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);
            queue.pop();

            //当队列为空时退出循环
            if(queue.empty()){
                break;
            }
        }
    }

    writeData(dest,records,results,execOrder);
}

void SJF(char* src,char* dest){
    vector<Record> records;
    readData(src,records);

    int size = records.size();
    vector<Result> results(size);
    vector<int> status(size,0);
    vector<int> remaning(size,0);
    vector<char> execOrder;

    for(int i=0; i<size;i++){
        remaning[i] = records[i].serviceTime;
    }


    int totalTime =0;
    int i=0;
    int finish = 0;
    int current = 0;
    int flag = 1;
    while(totalTime >= 0){
        if((i<size) && (status[i]==0) && records[i].arrivalTime <= totalTime){
            status[i] = 1;
            i++;
        }

        if(flag == 1){
            int minValue = 9999;
            for(int k=0; k<size; k++){
                if((status[k]==1) && (minValue > records[k].serviceTime)){
                    minValue = records[k].serviceTime;
                    current = k;
                }
            }
            flag = 0;
        }

        if(remaning[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        remaning[current] -=1;
        totalTime++;
        if(remaning[current] == 0){
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);
            status[current] = -1;
            finish++;
            flag = 1;
        }

        if(finish == size){
            break;
        }
    }

    writeData(dest,records,results,execOrder);
}
void SRT(char* src, char* dest){

    vector<Record> records;
    readData(src,records);
    int size = records.size();
    vector<Result> results(size);
    vector<int> status(size,0); //记录作业是否已经开始，0表示未开始，1表示就绪,-1表示已结束
    vector<int> remaining(size,0); //记录每个作业剩余的服务时间
    vector<char> execOrder;


    //初始化remaining
    for(int i=0; i<size; i++){
        remaining[i] = records[i].serviceTime;
    }

    int totalTime = 0;
    int i = 0;
    int current = 0;
    int finish = 0; //记录已完成任务个数
    while(totalTime >=0) {

        //检测是否有新作业加入，如果有就将它对应的status设置为1（就绪状态）
        if((i<size) &&(status[i] == 0) && (records[i].arrivalTime <= totalTime)){
            status[i] = 1;
            i++;
        }

        //选择当前服务时间最短的作业执行
        int minValue = 9999;
        for(int k=0; k<size; k++){
            if((status[k]==1) && (minValue > remaining[k])){
                minValue = remaining[k];
                current = k;
            }
        }

        //如果剩余时间等于要求服务时间，则说明这个作业是第一次执行，设置开始时间
        if(remaining[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        totalTime++;
        remaining[current] -=1;
        if(remaining[current] == 0){
            //剩余时间为0,作业已结束，将状态设置为-1
            status[current] = -1;
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);

            finish++;
            if(finish == size) //若所以作业都完成，退出循环
                break;
        }

    }
    writeData(dest,records,results,execOrder);
}

void HRRN(char* src,char* dest){
    vector<Record> records;
    readData(src,records);

    int size = records.size();
    vector<Result> results(size);
    vector<int> status(size,0); //记录工作的状态，0表示未开始，1表示就绪，-1表示介绍
    vector<int> remaning(size,0);
    vector<double> priority(size,-1.0); //记录响应比
    vector<char> execOrder;
    int totalTime = 0;

    for(int i=0; i<size; i++){
        remaning[i] = records[i].serviceTime;
    }

    int current = 0; //表示当前运行的作业下标
    int i = 0; //表示前i个任务进入就绪状态
    int flag = 1;//类似一把”锁“，flag=1时表示当前任务结束，可重新计算响应比并重新选择作
    int finish=0;//记录以经完成任务的个数
    while(totalTime >=0){
        //检测是否有新作业加入，如果有就将它对应的status设置为1（就绪状态）
        if((i<size) &&(status[i] == 0) && (records[i].arrivalTime <= totalTime)){
            status[i] = 1;
            i++;
        }



        //当flag为1时，重新计算响应比和重新计算作业
       if(flag == 1){

           for(int k=0; k<size; k++){
               if(status[k]==1){
                   priority[k] = (totalTime-records[k].arrivalTime+records[k].serviceTime)/(1.0*records[k].serviceTime);
               }
           }

           double maxValue = -9999;

           for(int k=0;k<size;k++){
               if((status[k]==1)&&(maxValue < priority[k])){
                   maxValue = priority[k];
                   current = k;
               }
           }
           flag = 0;
       }


        if(remaning[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        totalTime++;
        remaning[current] -=1;

        if(remaning[current] == 0){
            status[current] = -1;
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);
            flag = 1; //flag置1
            finish++;
        }

        if(finish == size)
            break;
    }

    writeData(dest,records,results,execOrder);
}

void RR(char* src,char* dest){
    vector<Record> records;
    readData(src,records);

    int size = records.size();
    vector<Result> results(size);
    vector<int> remaning(size,0);
    vector<int> status(size,0);
    vector<char> execOrder;
    queue<int> queue;

    for(int i=0;i<size;i++){
        remaning[i] = records[i].serviceTime;
    }

    int slice = 1;
    int totalTime = 0;
    int current = 0;
    int i =0;
    int finish = 0;

    while(totalTime >=0){
        if((i<size) && (status[i]==0) && records[i].arrivalTime <= totalTime){
            status[i] = 1;
            queue.push(i);
            i++;
        }

        current = queue.front();
        queue.pop();
        if(remaning[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        remaning[current] -=slice;

        totalTime +=slice;

        //当总时间增加时，判断是否有新作业加入。
        if((i<size) && (status[i]==0) && records[i].arrivalTime <= totalTime){
            status[i] = 1;
            queue.push(i);
            i++;
        }

        if(remaning[current] == 0){
            status[current] =-1 ;
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);
            finish++;
        }else{
            queue.push(current);
        }

        if(finish == size)
            break;
    }

    writeData(dest,records,results,execOrder);
}

void no_PRIORITY(char* src,char* dest){
    vector<Record> records;
    readData(src,records);

    int size = records.size();
    vector<Result> results(size);
    vector<int> status(size,0);
    vector<int> remaning(size,0);
    vector<int> priority(size,0);
    vector<char> execOrder;

    for(int i=0; i<size;i++){
        remaning[i] = records[i].serviceTime;
    }


    int totalTime =0;
    int i=0;
    int finish = 0;
    int current = 0;
    int flag = 1;
    while(totalTime >= 0){
        if((i<size) && (status[i]==0) && records[i].arrivalTime <= totalTime){
            status[i] = 1;
            i++;
        }

        if(flag == 1){
            int minValue = 9999;
            for(int k=0; k<size; k++){
                if((status[k]==1) && (minValue > records[k].priority)){
                    minValue = records[k].priority;
                    current = k;
                }
            }
            flag = 0;
        }

        if(remaning[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        remaning[current] -=1;
        totalTime++;
        if(remaning[current] == 0){
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);
            status[current] = -1;
            finish++;
            flag = 1;
        }

        if(finish == size){
            break;
        }
    }

    writeData(dest,records,results,execOrder);
}

void PRIORITY(char* src,char* dest){

    vector<Record> records;
    readData(src,records);
    int size = records.size();
    vector<Result> results(size);
    vector<int> status(size,0); //记录作业是否已经开始，0表示未开始，1表示就绪,-1表示已结束
    vector<int> remaining(size,0); //记录每个作业剩余的服务时间
    vector<char> execOrder;



    //初始化remaining
    for(int i=0; i<size; i++){
        remaining[i] = records[i].serviceTime;
    }

    int totalTime = 0;
    int i = 0;
    int current = 0;
    int finish = 0; //记录已完成任务个数
    while(totalTime >=0) {


        if((i<size) &&(status[i] == 0) && (records[i].arrivalTime <= totalTime)){
            status[i] = 1;
            i++;
        }

        int minValue = 9999;
        for(int k=0; k<size; k++){
            if((status[k]==1) && (minValue > records[k].priority)){
                minValue = records[k].priority;
                current = k;
            }
        }

        //如果剩余时间等于要求服务时间，则说明这个作业是第一次执行，设置开始时间
        if(remaining[current] == records[current].serviceTime){
            results[current].startTime = totalTime;
        }

        execOrder.push_back(records[current].job);
        totalTime++;
        remaining[current] -=1;
        if(remaining[current] == 0){
            //剩余时间为0,作业已结束，将状态设置为-1
            status[current] = -1;
            results[current].finishTime = totalTime;
            results[current].turnTime = results[current].finishTime - records[current].arrivalTime;
            results[current].weightTurnTime = results[current].turnTime/(1.0*records[current].serviceTime);

            finish++;
            if(finish == size) //若所以作业都完成，退出循环
                break;
        }

    }
    writeData(dest,records,results,execOrder);

}


void EDF(string dest){
    list<RtRecord> records;
    vector<RtResult> results;
    RtRecord* record;
    RtResult result;
    int totalTime = 0;
    int time = 100;


    while(totalTime >= 0){
        if(generateJob(time,totalTime,records)){
            record = &records.front();
            record->deadline = record->periodTime - record->remainingTime + record->arrivalTime;
            records.sort(compare);
        }

        if(records.empty()) break;

        record = &records.front();

        totalTime++;
        record->remainingTime--;

        if((record->remainingTime) == 0){
            records.pop_front();
            record->status = -1;
            result.job = record->job;
            result.finishTime = totalTime;
            results.push_back(result);
        }
    }

    writeData(dest,results);
}

void LLF(string dest){
    list<RtRecord> records;
    vector<RtResult> results;
    RtRecord* record;
    RtResult result;
    int totalTime = 0;
    int time = 100;
    int flag = 1;

    while(totalTime >= 0){
        if(generateJob(time,totalTime,records)){
            RtRecord* tmp = &records.front();
            tmp->deadline = tmp->periodTime - tmp->remainingTime + tmp->arrivalTime;
            records.sort(compare);
        }

        if(records.front().deadline == totalTime){
            flag = 1;
        }


        if(records.empty()) break;


        if(flag){
            record = &records.front();
            flag = 0;
        }

        totalTime++;
        record->remainingTime--;

        if((record->remainingTime) == 0){
            flag = 1;
            records.pop_front();
            record->status = -1;
            result.job = record->job;
            result.finishTime = totalTime;
            results.push_back(result);
        }
    }

    writeData(dest,results);
}

//int Banker_Al(char* src,int Request[][REC_NUM],int i){
//    vector<Banker> banker;
//    readData2(src,banker);
//    for(int m=0;m<REC_NUM;m++){
//        if(Request[i][m] > banker.Need[i][m]){
//            cout<<"所需资源过多!"<<endl;
//            return 0;
//        }else if(Request[i][m] > banker.Available[m]){
//            cout<<"资源不足，p["<<i<<"]需等待！"<<end;
//            return 0;
//        }
//    }
//    for(int j = 0;j < REC_NUM;j++){
//        banker.Available[j] = banker.Available[j]-Request[i][j];
//        banker.Allocation[i][j] = banker.Allocation[i][j]+Request[i][j];
//        banker.Need[i][j] = banker.Need[i][j] - Request[i][j];
//    }
//    //安全性算法
//     int n = Safe_Algorithm(Available,Need,Allocation);
//        cout<<endl;
//
//        if(n == PROGRESS) {//有5个'true'返回1，表示此时刻安全
//                cout<<"此时刻是安全状态，可以分配资源给"<<"P["<<i<<"]!"<<endl;
//        }else {
//                cout<<"此时刻不是安全状态，不能将资源分配给"<<"P["<<i<<"]!"<<endl;
//        }
//        return n;
//}

