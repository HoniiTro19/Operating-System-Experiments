/*
    ��ɽ��̵����㷨�е������ȷ���(FCFS)�����Ѱ������(SSTF)������ɨ���㷨(SCAN)��ѭ��ɨ���㷨(CSCAN)
    ��ƽ��Ѱ�����ȣ�ƽ���ȴ�ʱ��Ϊ��������
*/

#include "disk.h"
#include <iostream>
#include <cstdlib>
#include <list>

const int MAX_CPU_time = 3;
const int MAX_arrival_time = 20;
const int MAX_cylinder = 100;
const int disk_num = 10;
const float speed = 0.1;
const int CSCAN_dir = 1;
const int MMAX = 1000000;
using namespace std;

float time_lapses = 0;
int dir = 1;
int position = MAX_cylinder / 2;

int DISK::size;
float DISK::FCFS_AST;
float DISK::SSTF_AST;
float DISK::SCAN_AST;
float DISK::CSCAN_AST;
int DISK::FCFS_WT;
int DISK::SSTF_WT;
int DISK::SCAN_WT;
int DISK::CSCAN_WT;

list<DISK*> DISK::DISK_list;

DISK::DISK(int arrival_time, int cpu_time, int cylinder) {
	name = ++size;
	time_needed = cpu_time;
	time_arrival = arrival_time;
    time_waited = 0;
    cylinder_num = cylinder;
	DISK_list.push_back(this);
}

void DISK::disp() {
	cout << name << "\t\t" << time_arrival << "\t\t" << time_needed << "\t\t" << cylinder_num << endl;
}

bool DISK::compare_FCFS(const DISK* f, const DISK* s) {
    return f->time_arrival < s->time_arrival;
}

bool DISK::compare_SSTF(const DISK* f, const DISK* s) {
	if (f->time_arrival < time_lapses && s->time_arrival < time_lapses)
        return abs(f->cylinder_num - position) < abs(s->cylinder_num - position);
	else return f->time_arrival < s->time_arrival;
}

//���¾���
void DISK::update(){
    DISK* disk;
    list <DISK*>::iterator plist;
    for (plist = DISK_list.begin(); plist != DISK_list.end(); plist++){
        disk = *plist;
        if (dir == 1){
            if (disk->cylinder_num - position > 0) disk->dir_dis = disk->cylinder_num - position;
            else disk->dir_dis = MMAX;
        }

        else{
            if (disk->cylinder_num - position < 0) disk->dir_dis = position - disk->cylinder_num;
            else disk->dir_dis = MMAX;
        }
    }
}

bool DISK::compare_SCAN(const DISK* f, const DISK* s) {
	if (f->time_arrival < time_lapses && s->time_arrival < time_lapses) return f->dir_dis < s->dir_dis;
    else return f->time_arrival < s->time_arrival;
}

//��ʾ���ж��е���Ϣ
void DISK::disp_list() {
	cout << "---------------------------------------------------" << endl;
	cout << "|���\t" << "|����ʱ��\t"<< "|CPUʱ��\t" << "|��������|" <<endl;
	cout << "---------------------------------------------------" << endl;
	list <DISK*>::iterator plist;
	for (plist = DISK_list.begin(); plist != DISK_list.end(); plist++) {
		DISK* DISK;
		DISK = *plist;
		DISK->disp();
	}
	cout << endl;
}

//�����ȷ�������㷨
void DISK::FCFSrun() {
	float i = 0;
	int num = 0;
	DISK_list.sort(compare_FCFS);
	DISK* disk;
	while(num < disk_num){
		disk = *DISK_list.begin();
		time_lapses = max((float)disk->time_arrival, i);
		DISK_list.pop_front();
		num ++;
		time_lapses += disk->time_needed + speed * abs(position-disk->cylinder_num);
		DISK::FCFS_AST += abs(position-disk->cylinder_num);
		DISK::FCFS_WT += time_lapses - disk->time_arrival;
		position = disk->cylinder_num;
		cout << "��ǰ����" << disk->name << "\t��ǰʱ��" << time_lapses << "\t��ǰָ����������" << position << endl;
		disp_list();
	}
}

//���Ѱ�����ȵ����㷨
void DISK::SSTFrun() {
	float i = 0;
	int num = 0;
	DISK* disk;
	while(num < disk_num){
        DISK_list.sort(compare_SSTF);
		disk = *DISK_list.begin();
		time_lapses = max((float)disk->time_arrival, i);
		DISK_list.pop_front();
		num ++;
		time_lapses += disk->time_needed + speed * abs(position-disk->cylinder_num);
		DISK::SSTF_AST += abs(position-disk->cylinder_num);
		DISK::SSTF_WT += time_lapses - disk->time_arrival;
		position = disk->cylinder_num;
		cout << "��ǰ����" << disk->name << "\t��ǰʱ��" << time_lapses << "\t��ǰָ����������" << position << endl;
		disp_list();
	}
}

//����ɨ������㷨
void DISK::SCANrun() {
	int num = 0, step = 0;
    DISK* disk;
    while (num < disk_num) {
        DISK::update();
        DISK_list.sort(compare_SCAN);
        disk = *DISK_list.begin();
        if (position == 1 || position == MAX_cylinder) dir = -dir;
        if (disk->time_arrival <= time_lapses && disk->cylinder_num == position + dir){
            num++;
            DISK_list.pop_front();
            time_lapses += speed;
            position += dir;
            DISK::SCAN_AST += step;
            DISK::SCAN_WT += time_lapses - disk->time_arrival;
            time_lapses += disk->time_needed;
            step = 0;
            cout << "��ǰ����" << disk->name << "\t��ǰʱ��" << time_lapses << "\t��ǰָ����������" << position << endl;
            disp_list();
            continue;
        }
        position += dir;
        step++;
        time_lapses += speed;
        //cout << "��ǰ����" << disk->name << "\t��ǰʱ��" << time_lapses << "\t��ǰָ����������" << position << endl;
        //system("pause");
	}
}

//ѭ��ɨ���㷨
void DISK::CSCANrun(){
    int num = 0, step = 0;
    DISK* disk;
    while (num < disk_num) {
        if (position == 1 && CSCAN_dir == 0) position = MAX_cylinder;
        if (position == MAX_cylinder && CSCAN_dir == 1) position = 1;
        DISK::update();
        //cout << time_lapses << '\t' << position << endl;
        DISK_list.sort(compare_SCAN);
        disk = *DISK_list.begin();

        if (disk->time_arrival <= time_lapses && disk->cylinder_num == position + dir){
            num++;
            DISK_list.pop_front();
            time_lapses += speed;
            position += dir;
            DISK::CSCAN_AST += step;
            DISK::CSCAN_WT += time_lapses - disk->time_arrival;
            time_lapses += disk->time_needed;
            step = 0;
            cout << "��ǰ����" << disk->name << "\t��ǰʱ��" << time_lapses << "\t��ǰָ����������" << position << endl;
            disp_list();
            continue;
        }
        position += dir;
        step++;
        time_lapses += speed;
        //system("pause");
    }
}

void init(){
    /*
        ������ɽ���DISK�����Ϣ
        ���̵����ʱ�䷶Χ��1-MAX_arrival_time
        ����ռ��CPUʱ��ķ�Χ��1-MAX_CPU_time
    */
    srand(111);
    DISK::size = 0;
    time_lapses = 0;
    position = MAX_cylinder / 2;
    for(int i = 0; i < disk_num; i++){
        int AT = rand() % MAX_arrival_time + 1;
        int CT = rand() % MAX_CPU_time + 1;
        int CN = rand() % MAX_cylinder + 1;
        new DISK(AT, CT, CN);
    }
    cout << "=====������ݳ�ʼ��=====" << endl;
}

int main(){
    cout << "=====��ǰ������Ŀ=====" << endl;
    cout << disk_num << endl;
    init();
    cout << "=====���̾������е����=====" << endl;
    DISK::disp_list();
    cout << "=====����FCFS���Խ��е���=====" << endl;
    DISK::FCFSrun();
    init();
    cout << "=====���̾������е����=====" << endl;
    DISK::disp_list();
    cout << "=====����SSTF���Խ��е���=====" << endl;
    DISK::SSTFrun();
    init();
    cout << "=====���̾������е����=====" << endl;
    DISK::disp_list();
    cout << "=====����SCAN���Խ��е���=====" << endl;
    DISK::SCANrun();
    init();
    cout << "=====���̾������е����=====" << endl;
    DISK::disp_list();
    cout << "=====����CSCAN���Խ��е���=====" << endl;
    DISK::CSCANrun();

    cout << "FCSF���Ե�ƽ��Ѱ������Ϊ:" << (float)DISK::FCFS_AST / disk_num << endl;
    cout << "          ����ƽ���ȴ�ʱ��Ϊ:" << (float)DISK::FCFS_WT / disk_num << endl;
    cout << "SSTF���Ե�ƽ��Ѱ������Ϊ:" << (float)DISK::SSTF_AST / disk_num<< endl;
    cout << "          ����ƽ���ȴ�ʱ��Ϊ:" << (float)DISK::SSTF_WT / disk_num << endl;
    cout << "SCAN���Ե�ƽ��Ѱ������Ϊ:" << (float)DISK::SCAN_AST / disk_num<< endl;
    cout << "          ƽ���ȴ�ʱ��Ϊ:" << (float)DISK::SCAN_WT / disk_num<< endl;
    cout << "CSCAN���Ե�ƽ��Ѱ������Ϊ:" << (float)DISK::CSCAN_AST / disk_num<< endl;
    cout << "          ƽ���ȴ�ʱ��Ϊ:" << (float)DISK::CSCAN_WT / disk_num<< endl;
}
