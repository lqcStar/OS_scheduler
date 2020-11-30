#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "utils.h"
#include "structs.h"

void FCFS(char* src,char* dest);
void SJF(char* src,char* dest);
void HRRN(char* src,char* dest);
void SRT(char* src,char* dest);
void RR(char* src,char* dest);
void no_PRIORITY(char* src,char* dest);
void PRIORITY(char* src,char* dest);
void EDF(string dest);
void LLF(string dest);
//int Banker_Al(char *src,int Request[][REC_NUM],int i);

#endif //SCHEDULER_H

