#ifndef BANKER_H
#define BANKER_H

int const process_num = 5;
int const resource_num = 3;
class banker {
public:
	banker();
	void send_request();
	bool safety_check();
	void disp();
	void init();

private:
	int available[resource_num];
	int max[process_num][resource_num];
	int allocation[process_num][resource_num];
	int need[process_num][resource_num];
	int work[resource_num];
	bool finish[process_num];
	int request[resource_num+1];
	int security[process_num];
};
#endif // BANKER_H

