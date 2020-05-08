/*
    完成进程调度算法中的先来先服务(FCFS)，短CPU运行时间优先(SCBF)以及高响应比优先(HRN)调度算法
    其中前两者为非抢占式，最后一个为抢占式
    进程的数目通过用户输入得到，进程的PCB控制块信息随机生成
    以带权平均周转时间,总周转时间作为评判依据
*/

#include "PCB.h"
#include <iostream>
#include <cstdlib>
#include <list>

const int MAX_CPU_time = 8;
const int MAX_arrival_time = 10;
using namespace std;

const int PCB_num = 10;
int FCFS_TT[PCB_num];
int SCBF_TT[PCB_num];
int HRN_TT[PCB_num];
int rel[PCB_num];
int time_lapses = 0;

int PCB::size;
float PCB::FCFS_ATT;
float PCB::SCBF_ATT;
float PCB::HRN_ATT;
int PCB::FCFS_T;
int PCB::SCBF_T;
int PCB::HRN_T;
list<PCB*> PCB::pcb_list;

PCB::PCB(int arrival_time, int cpu_time) {
	name = ++size;
	time_needed = cpu_time;
	time_arrival = arrival_time;
    time_waited = 0;
	response = 1 + (float)time_waited / time_needed;
	pcb_list.push_back(this);
}

void PCB::disp() {
	cout << name << "\t\t" << time_arrival << "\t\t" << time_needed << "\t\t" << response << endl;
}

bool PCB::compare_FCFS(const PCB* f, const PCB* s) {
    return f->time_arrival < s->time_arrival;
}

bool PCB::compare_SCBF(const PCB* f, const PCB* s) {
	if (f->time_arrival < time_lapses && s->time_arrival < time_lapses)
        return f->time_needed < s->time_needed;
	else return f->time_arrival < s->time_arrival;
}

bool PCB::compare_HRN(const PCB* f, const PCB* s) {
	if (f->time_arrival < time_lapses && s->time_arrival < time_lapses)
        return f->response > s->response;
    else return f->time_arrival < s->time_arrival;
}

//显示所有队列的信息
void PCB::disp_list() {
	cout << "---------------------------------------------------" << endl;
	cout << "|编号\t" << "|到达时间\t"<< "|CPU时间\t" << "|响应比|" <<endl;
	cout << "---------------------------------------------------" << endl;
	list <PCB*>::iterator plist;
	for (plist = pcb_list.begin(); plist != pcb_list.end(); plist++) {
		PCB* pcb;
		pcb = *plist;
		pcb->disp();
	}
	cout << endl;
}

//更新响应比算法中的响应比
void PCB::update(){
    PCB* pcb;
    list <PCB*>::iterator plist;
    plist = pcb_list.begin();
    pcb = *plist;
    pcb->time_needed --;
    for (plist++; plist != pcb_list.end(); plist++){
        pcb = *plist;
        pcb->time_waited = max(0, time_lapses - pcb->time_arrival);
        pcb->response = 1 + (float)pcb->time_waited / pcb->time_needed;
    }
}

//先来先服务调度算法
void PCB::FCFSrun() {
	int i = 0, num = 0;
	pcb_list.sort(compare_FCFS);
	PCB* pcb;
	while(num < PCB_num){
		pcb = *pcb_list.begin();
		i = max(pcb->time_arrival, i);
		pcb_list.pop_front();
		num ++;
		i += pcb->time_needed;
		FCFS_TT[pcb->name-1] = i - pcb->time_arrival;
		cout << "当前进程" << pcb->name << "\t当前时间" << i << endl;
		disp_list();
	}
}

//短CPU优先调度算法
void PCB::SCBFrun() {
	int i = 0,num = 0;
	PCB* pcb;
	while(num < PCB_num){
        pcb_list.sort(compare_SCBF);
		pcb = *pcb_list.begin();
		i = max(pcb->time_arrival, i);
		pcb_list.pop_front();
		num ++;
		i += pcb->time_needed;
		SCBF_TT[pcb->name-1] = i - pcb->time_arrival;
        time_lapses = i;
		cout << "当前进程" << pcb->name << "\t当前时间" << i << endl;
		disp_list();
	}
}

//高响应比优先调度算法
void PCB::HRNrun() {
	int i = 0,num = 0;
    PCB* pcb;
    pcb_list.sort(compare_HRN);
    while (num < PCB_num) {
		pcb = *pcb_list.begin();
		if(pcb->time_needed == 0){
            num++;
            pcb_list.pop_front();
            HRN_TT[pcb->name] = i - pcb->time_arrival;
            continue;
		}
        pcb_list.sort(compare_HRN);
        i = max(pcb->time_arrival, i);
        time_lapses = i + 1;
        PCB::update();
		cout << "当前进程" << pcb->name << "\t当前时间" << ++i << endl;
		disp_list();
	}
}

void init(){
    /*
        随机生成进程PCB块的信息
        进程到达的时间范围是1-MAX_arrival_time
        进程占用CPU时间的范围是1-MAX_CPU_time
    */
    srand(111);
    PCB::size = 0;
    time_lapses = 0;
    for(int i = 0; i < PCB_num; i++){
        int AT = rand() % MAX_arrival_time + 1;
        int CT = rand() % MAX_CPU_time + 1;
        new PCB(AT, CT);
        rel[i] = CT;
    }
    cout << "=====完成数据初始化=====" << endl;
}

float ATT1(int flag){
    float ATT;
    for (int i=0;i<PCB_num;i++){
        if (flag == 0) ATT += (float)FCFS_TT[i]/rel[i];
        else ATT += FCFS_TT[i];
    }
    return ATT;
}

float ATT2(int flag){
    float ATT;
    for (int i=0;i<PCB_num;i++){
        if (flag == 0) ATT += (float)SCBF_TT[i]/rel[i];
        else ATT += SCBF_TT[i];
    }
    return ATT;
}

float ATT3(int flag){
    float ATT;
    for (int i=0;i<PCB_num;i++){
        if (flag == 0) ATT += (float)HRN_TT[i]/rel[i];
        else ATT += HRN_TT[i];
    }
    return ATT;
}

int main(){
    int (*FCFS_TT),(*SCBF_TT),(*HRN_TT);
    int (*FCFS_rel),(*SCBF_rel),(*HRN_rel);
    cout << "=====当前进程数目=====" << endl;
    cout << PCB_num << endl;
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    PCB::disp_list();
    cout << "=====利用FCFS策略进行调度=====" << endl;
    PCB::FCFSrun();
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    PCB::disp_list();
    cout << "=====利用SCBF策略进行调度=====" << endl;
    PCB::SCBFrun();
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    PCB::disp_list();
    cout << "=====利用HRN策略进行调度=====" << endl;
    PCB::HRNrun();

    PCB::FCFS_ATT = ATT1(0);
    PCB::SCBF_ATT = ATT2(0);
    PCB::HRN_ATT = ATT3(0);
    PCB::FCFS_T = ATT1(1);
    PCB::SCBF_T = ATT2(1);
    PCB::HRN_T = ATT3(1);
    cout << "FCSF策略的平均带权周转时间为:" << (float)PCB::FCFS_ATT / PCB_num << endl;
    cout << "          总周转时间为:" << PCB::FCFS_T << endl;
    cout << "SCBF策略的平均带权周转时间为:" << (float)PCB::SCBF_ATT / PCB_num<< endl;
    cout << "          总周转时间为:" << PCB::SCBF_T << endl;
    cout << "HRN策略的平均带权周转时间为:" << (float)PCB::HRN_ATT / PCB_num<< endl;
    cout << "          总周转时间为:" << PCB::HRN_T << endl;
}
