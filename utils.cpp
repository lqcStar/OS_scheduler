#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

#define BUFSIZE 1024
#define A_PERIOD_TIME 20
#define B_PERIOD_TIME 50
#define A_SERVICE_TIME 10
#define B_SERVICE_TIME 25
using namespace std;

void readData(char* src,vector<record>& records){
    Record  record;
    ifstream in;
    in.open(src);

    char buffer[BUFSIZE];

    in.getline(buffer,BUFSIZE);

    while(in.getline(buffer,BUFSIZE)){
        for(int i=0; i<strlen(buffer);i++){
            if(buffer[i] == '\t'){
                buffer[i] ='\0';
            }
        }

        int index = 0;
        record.job = buffer[0];

        index += strlen(buffer)+1;
        record.arrivalTime = atoi(buffer+index);

        index += strlen(buffer)+1;
        record.serviceTime = atoi(buffer+index);
        index += strlen(buffer)+1;
        record.priority = atoi(buffer+index);
        records.push_back(record);
    }
}
//void readData2(char *src,vector<banker> &banker){
//    ifstream in;
//    in.open(src);
//    int data;
//    for(int j=0; j<PROGRESS; j++){
//        for(int i=0;i<REC_NUM;i++){
//            in>>data;
//            banker.Max[j][i];
//        }
//        for(int i=0;i<REC_NUM;i++){
//            in>>data;
//            banker.Allocation[j][i];
//        }
//        for(int i=0;i<REC_NUM;i++){
//            in>>data;
//            banker.Need[j][i];
//        }
//        if(j ==0){
//            for(int i=0;i<REC_NUM;i++){
//            in>>data;
//            banker.Available[j][i];
//            }
//        }
//    }
//    inFile2.close();
//}
void writeData(char* dest,vector<record>& records,vector<result>& results,vector<char>& execOrder){
    ofstream out;
    double avgTurnTime =0;
    double avgWeightedTurnTime =0;
    out.open(dest);

    out <<setw(12) << left <<"job"
        <<'\t'<<setw(12)<<"arrivalTime"
        <<'\t'<<setw(12)<<"serviceTime"
        <<'\t'<<setw(12)<<"startTime"
        <<'\t'<<setw(12)<<"finishTime"
        <<'\t'<<setw(12)<<"turnTime"
        <<'\t'<<setw(12)<<"weightedTurnTime"
        <<endl;

    for(int i=0;i<records.size();i++){
        out <<setw(15) << left <<  records[i].job
            << '\t' <<setw(12)<< records[i].arrivalTime
            << '\t' <<setw(12) << records[i].serviceTime
            << '\t' <<setw(12)<< results[i].startTime
            << '\t' <<setw(12)<< results[i].finishTime
            << '\t' <<setw(12)<< results[i].turnTime
            << '\t' <<setw(12)<< results[i].weightTurnTime
            <<endl;
        avgTurnTime += results[i].turnTime;
        avgWeightedTurnTime += results[i].weightTurnTime;
    }

    int size = records.size();

    //保留两位小数
    out.setf(ios::fixed);
    out.precision(2);
    out << "average turnTime: "
        <<'\t'<<avgTurnTime/size
        <<endl;
    out << "average weigtedTurnTime: "
        << avgWeightedTurnTime/size
        <<endl;

    out <<"作业执行顺序：";
    for(int i=0 ;i<execOrder.size();i++){
        out << execOrder[i];
    }
    out <<endl;
}
void writeData(string dest,vector<RtResult>& results){
    ofstream out;
    out.open(dest);


    for(int i=0; i< results.size();i++){
        out << "(" << results[i].job <<"," <<results[i].finishTime <<")\n";
    }
}
bool generateJob(int totalTime ,int currentTime,list<RtRecord>& records){
    RtRecord record;
    int count;
    bool flag = false;
    string jobA("A");
    string jobB("B");

    if(totalTime >= currentTime){
        if((currentTime % A_PERIOD_TIME) == 0){
            count = currentTime/A_PERIOD_TIME;
            record.arrivalTime = currentTime;
            record.job = jobA.append(to_string(count));
            record.deadline = A_PERIOD_TIME - A_SERVICE_TIME + currentTime;
            record.remainingTime = A_SERVICE_TIME;
            record.periodTime = A_PERIOD_TIME;
            record.status = 1;
            records.push_back(record);
            flag = true;
        }

        if((currentTime % B_PERIOD_TIME) == 0){
            count = currentTime/B_PERIOD_TIME;
            record.arrivalTime = currentTime;
            record.job = jobB.append(to_string(count));
            record.deadline = B_PERIOD_TIME - B_SERVICE_TIME + currentTime;
            record.remainingTime = B_SERVICE_TIME;
            record.periodTime = B_PERIOD_TIME;
            record.status = 1;
            records.push_back(record);
            flag = true;
        }
    }
    return  flag;

}

bool compare(const RtRecord& record1, const RtRecord& record2){
    return (record1.deadline < record2.deadline);
}
//void writeData2(char* dest,int result){
//    ofstream out;
//    out.open(dest);
//    f(result == PROGRESS) {
//                out<<" 进程\\资源情况"<<" Work(A B C)"<<" Need(A B C)"
//                    <<" Allocation(A B C)"<<" Work+Available(A B C)"<<" Finish";
//                out<<endl;
//                for(int i = 0;i < PROGRESS;i++) {
//                        out<<"    "<<"P["<<banker.sign[i]<<"]  "<<'\t';
//                        for(int j = 0;j < REC_NUM;j++)
//                               out<<banker.work[sign[i]][j]<<" ";
//                        out<<'\t'<<'\t';
//                        for(int j = 0;j < REC_NUM;j++)
//                               out<<banker.Need[sign[i]][j]<<" ";
//                        out<<'\t'<<'\t';
//                        for(int j = 0;j < REC_NUM;j++)
//                               out<<banker.Allocation[sign[i]][j]<<" ";
//                        out<<'\t'<<'\t';
//                        for(int j = 0;j < REC_NUM;j++)
//                               out<<banker.workAll[sign[i]][j]<<" ";
//                        out<<'\t'<<'\t';
//                        out<<"true"<<endl;
//                }
//                out<<endl<<"找到安全序列｛P["<<sign[0]<<"]";
//                for(int m = 1;m < PROGRESS;m++){
//                        out<<", P["<<banker.sign[m]<<"]";
//                }
//                out<<"}"<<endl;
//
//        } else {
//                out<<"   进程\\资源情况 "<<"  Allocation(A B C)"<<"   Need(A B C)"<<"   Available(A B C)";
//                out<<endl;
//                for(int k = 0;k < 5;k++){
//                        out<<'\t'<<"P["<<k<<"]"<<'\t'<<'\t';
//                        for(int j = 0;j < 3;j++)
//                                out<<banker.Allocation[k][j]<<" ";
//                        out<<'\t'<<'\t';
//                        for(int j = 0;j < 3;j++)
//                                out<<banker.Need[k][j]<<" ";
//                        out<<'\t'<<'\t';
//                        if(k == 0) {
//                                for(int j = 0;j < 3;j++)
//                                out<<banker.Available[j]<<" ";
//                        }
//                        out<<endl;
//                }
//        }
//}

