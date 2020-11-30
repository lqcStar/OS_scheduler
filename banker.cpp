#include "banker.h"
#include <iostream>

using namespace std;

vector<int> available(COL);
vector<vector<int> > imax(ROW,vector<int>(COL));
vector<vector<int> > allocation(ROW,vector<int>(COL));
vector<vector<int> > need(ROW,vector<int>(COL));
void init(){
    int maxArr[ROW][COL] = {
            {7,5,3},
            {3,2,2},
            {9,0,2},
            {2,2,2},
            {4,3,3}
    };

    int allocationArr[ROW][COL]{
            {0,1,0},
            {2,0,0},
            {3,0,2},
            {2,1,1},
            {0,0,2}
    };
    int availableArr[COL] = {3,3,2};


    for(int i=0; i<COL;i++){
        available[i] = availableArr[i];
    }

    for(int i=0; i<ROW;i++){
        for(int j=0; j<COL; j++){
            allocation[i][j] = allocationArr[i][j];
            imax[i][j] = maxArr[i][j];
            need[i][j] = imax[i][j] - allocation[i][j] ;
        }
    }
}

void banker(vector<int> request,int row){
    for(int i=0; i<COL; i++){
        if(request[i] > need[row][i]){
            cout << "请求资源大于需求，分配失败" <<endl;
            return;
        }

        if(request[i] > available[i]){
            cout << "资源不足，分配失败"<<endl;
            return;
        }
    }

    allocate(request,row);
    if(isSecurity()){
        cout <<"分配成功，系统处于安全状态"<<endl;
    } else{
            rollBack(request,row);
        cout <<"系统处于不安全状态，分配失败"<<endl;
    }
}

bool allocate(vector<int> request,int row){
    for(int i=0; i<COL; i++){
        allocation[row][i] += request[i];
        available[i] -= request[i];
        need[row][i] -= request[i];
    }
    return true;
}

void rollBack(vector<int> request,int row){
    for(int i=0; i<COL; i++){
        allocation[row][i] -= request[i];
        available[i] += request[i];
        need[row][i] += request[i];
    }
}

bool isSecurity(){
    vector<int> finish(ROW,0);
    vector<int> work(available);
    bool security = true;
    int count = 0;
    int flag = 1;
    while(count < ROW){
        int isChange = 0;
        for(int i=0; i<ROW; i++){
            flag = 1;
            for(int j=0; j<COL; j++){
                if((need[i][j] > work[j]) || (finish[i])){
                    flag = 0;
                    continue;
                }
            }

            if(flag){
                isChange =1;
                finish[i] = true;
                count++;
                for(int k=0; k<COL;k++){
                    work[k] +=allocation[i][k];
                }
            }
        }

        if(!isChange && count < ROW){

            security = false;
            break;
        }
    }
    return  security;
}
