/*
    完成进程调度算法中的先来先服务(FCFS)，最短寻道优先(SSTF)，电梯扫描算法(SCAN)，循环扫描算法(CSCAN)
    以平均寻道长度，平均等待时间为评判依据
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

//更新距离
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

//显示所有队列的信息
void DISK::disp_list() {
	cout << "---------------------------------------------------" << endl;
	cout << "|编号\t" << "|到达时间\t"<< "|CPU时间\t" << "|磁盘柱面|" <<endl;
	cout << "---------------------------------------------------" << endl;
	list <DISK*>::iterator plist;
	for (plist = DISK_list.begin(); plist != DISK_list.end(); plist++) {
		DISK* DISK;
		DISK = *plist;
		DISK->disp();
	}
	cout << endl;
}

//先来先服务调度算法
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
		cout << "当前进程" << disk->name << "\t当前时间" << time_lapses << "\t当前指针所在柱面" << position << endl;
		disp_list();
	}
}

//最短寻道优先调度算法
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
		cout << "当前进程" << disk->name << "\t当前时间" << time_lapses << "\t当前指针所在柱面" << position << endl;
		disp_list();
	}
}

//电梯扫描调度算法
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
            cout << "当前进程" << disk->name << "\t当前时间" << time_lapses << "\t当前指针所在柱面" << position << endl;
            disp_list();
            continue;
        }
        position += dir;
        step++;
        time_lapses += speed;
        //cout << "当前进程" << disk->name << "\t当前时间" << time_lapses << "\t当前指针所在柱面" << position << endl;
        //system("pause");
	}
}

//循环扫描算法
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
            cout << "当前进程" << disk->name << "\t当前时间" << time_lapses << "\t当前指针所在柱面" << position << endl;
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
        随机生成进程DISK块的信息
        进程到达的时间范围是1-MAX_arrival_time
        进程占用CPU时间的范围是1-MAX_CPU_time
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
    cout << "=====完成数据初始化=====" << endl;
}

int main(){
    cout << "=====当前进程数目=====" << endl;
    cout << disk_num << endl;
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    DISK::disp_list();
    cout << "=====利用FCFS策略进行调度=====" << endl;
    DISK::FCFSrun();
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    DISK::disp_list();
    cout << "=====利用SSTF策略进行调度=====" << endl;
    DISK::SSTFrun();
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    DISK::disp_list();
    cout << "=====利用SCAN策略进行调度=====" << endl;
    DISK::SCANrun();
    init();
    cout << "=====进程就绪队列的情况=====" << endl;
    DISK::disp_list();
    cout << "=====利用CSCAN策略进行调度=====" << endl;
    DISK::CSCANrun();

    cout << "FCSF策略的平均寻道长度为:" << (float)DISK::FCFS_AST / disk_num << endl;
    cout << "          磁盘平均等待时间为:" << (float)DISK::FCFS_WT / disk_num << endl;
    cout << "SSTF策略的平均寻道长度为:" << (float)DISK::SSTF_AST / disk_num<< endl;
    cout << "          磁盘平均等待时间为:" << (float)DISK::SSTF_WT / disk_num << endl;
    cout << "SCAN策略的平均寻道长度为:" << (float)DISK::SCAN_AST / disk_num<< endl;
    cout << "          平均等待时间为:" << (float)DISK::SCAN_WT / disk_num<< endl;
    cout << "CSCAN策略的平均寻道长度为:" << (float)DISK::CSCAN_AST / disk_num<< endl;
    cout << "          平均等待时间为:" << (float)DISK::CSCAN_WT / disk_num<< endl;
}
