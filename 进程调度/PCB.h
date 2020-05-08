#ifndef PCB_H_
#define PCB_H_
#include <iostream>
#include <list>

using namespace std;

class PCB {
public:
	PCB(int arrival_time, int cpu_time);
	void disp();
	void disp_response();
	static int size;
	static list<PCB*> pcb_list;
	static void disp_list();
	static bool compare_FCFS(const PCB* f, const PCB* s);
	static bool compare_SCBF(const PCB* f, const PCB* s);
	static bool compare_HRN(const PCB* f, const PCB* s);
	static void FCFSrun();
	static void SCBFrun();
	static void HRNrun();
	static float FCFS_ATT;
	static float SCBF_ATT;
	static float HRN_ATT;
    static int FCFS_T;
	static int SCBF_T;
	static int HRN_T;
	static void update();

private:
	int name;
	int time_needed;
	int time_arrival;
	int time_waited;
	float response;
};

#endif // PCB_H
