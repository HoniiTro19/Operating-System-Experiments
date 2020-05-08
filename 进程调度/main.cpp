/*
    ��ɽ��̵����㷨�е������ȷ���(FCFS)����CPU����ʱ������(SCBF)�Լ�����Ӧ������(HRN)�����㷨
    ����ǰ����Ϊ����ռʽ�����һ��Ϊ��ռʽ
    ���̵���Ŀͨ���û�����õ������̵�PCB���ƿ���Ϣ�������
    �Դ�Ȩƽ����תʱ��,����תʱ����Ϊ��������
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

//��ʾ���ж��е���Ϣ
void PCB::disp_list() {
	cout << "---------------------------------------------------" << endl;
	cout << "|���\t" << "|����ʱ��\t"<< "|CPUʱ��\t" << "|��Ӧ��|" <<endl;
	cout << "---------------------------------------------------" << endl;
	list <PCB*>::iterator plist;
	for (plist = pcb_list.begin(); plist != pcb_list.end(); plist++) {
		PCB* pcb;
		pcb = *plist;
		pcb->disp();
	}
	cout << endl;
}

//������Ӧ���㷨�е���Ӧ��
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

//�����ȷ�������㷨
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
		cout << "��ǰ����" << pcb->name << "\t��ǰʱ��" << i << endl;
		disp_list();
	}
}

//��CPU���ȵ����㷨
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
		cout << "��ǰ����" << pcb->name << "\t��ǰʱ��" << i << endl;
		disp_list();
	}
}

//����Ӧ�����ȵ����㷨
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
		cout << "��ǰ����" << pcb->name << "\t��ǰʱ��" << ++i << endl;
		disp_list();
	}
}

void init(){
    /*
        ������ɽ���PCB�����Ϣ
        ���̵����ʱ�䷶Χ��1-MAX_arrival_time
        ����ռ��CPUʱ��ķ�Χ��1-MAX_CPU_time
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
    cout << "=====������ݳ�ʼ��=====" << endl;
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
    cout << "=====��ǰ������Ŀ=====" << endl;
    cout << PCB_num << endl;
    init();
    cout << "=====���̾������е����=====" << endl;
    PCB::disp_list();
    cout << "=====����FCFS���Խ��е���=====" << endl;
    PCB::FCFSrun();
    init();
    cout << "=====���̾������е����=====" << endl;
    PCB::disp_list();
    cout << "=====����SCBF���Խ��е���=====" << endl;
    PCB::SCBFrun();
    init();
    cout << "=====���̾������е����=====" << endl;
    PCB::disp_list();
    cout << "=====����HRN���Խ��е���=====" << endl;
    PCB::HRNrun();

    PCB::FCFS_ATT = ATT1(0);
    PCB::SCBF_ATT = ATT2(0);
    PCB::HRN_ATT = ATT3(0);
    PCB::FCFS_T = ATT1(1);
    PCB::SCBF_T = ATT2(1);
    PCB::HRN_T = ATT3(1);
    cout << "FCSF���Ե�ƽ����Ȩ��תʱ��Ϊ:" << (float)PCB::FCFS_ATT / PCB_num << endl;
    cout << "          ����תʱ��Ϊ:" << PCB::FCFS_T << endl;
    cout << "SCBF���Ե�ƽ����Ȩ��תʱ��Ϊ:" << (float)PCB::SCBF_ATT / PCB_num<< endl;
    cout << "          ����תʱ��Ϊ:" << PCB::SCBF_T << endl;
    cout << "HRN���Ե�ƽ����Ȩ��תʱ��Ϊ:" << (float)PCB::HRN_ATT / PCB_num<< endl;
    cout << "          ����תʱ��Ϊ:" << PCB::HRN_T << endl;
}
