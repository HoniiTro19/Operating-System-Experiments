#ifndef DISK_H
#define DISK_H
#include <iostream>
#include <list>

using namespace std;

class DISK {
public:
    DISK(int arrival_time, int cpu_time, int cylinder);
	void disp();
	static int size;
	static list<DISK*> DISK_list;
	static void disp_list();
	static bool compare_FCFS(const DISK* f, const DISK* s);
	static bool compare_SSTF(const DISK* f, const DISK* s);
	static bool compare_SCAN(const DISK* f, const DISK* s);
	static void FCFSrun();
	static void SSTFrun();
	static void SCANrun();
	static void CSCANrun();
	static float FCFS_AST;
	static float SSTF_AST;
	static float SCAN_AST;
	static float CSCAN_AST;
    static int FCFS_WT;
	static int SSTF_WT;
	static int SCAN_WT;
	static int CSCAN_WT;
    static void update();

private:
	int name;
	int time_needed;
	int time_arrival;
	int time_waited;
	int cylinder_num;
	int dir_dis;
};

#endif // DISK_H
