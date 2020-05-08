#include "banker.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include<fstream>

using namespace std;

banker::banker() {
}

void banker::send_request() {
    int flag = 1;
    cout << "=====���������������Դ����=====" << endl;
    cout << "=====�����ʽΪ(���̺�,��Դ1,��Դ2,...,��ԴN)=====" << endl;
    cin >> request[0];
    for (int i = 0; i < resource_num; i++){
        cin >> request[i+1];
        if (request[i+1] > need[request[0]][i]) flag = 0;
        if (request[i+1] > available[i]) flag = 0;
    }
	if (flag){
        int old_available[resource_num];
        int old_allocation[process_num][resource_num];
        int old_need[process_num][resource_num];
        for (int i = 0; i < resource_num; i++) {
            old_allocation[request[0]][i] = allocation[request[0]][i];
            allocation[request[0]][i] = allocation[request[0]][i] + request[i + 1];

            old_need[request[0]][i] = need[request[0]][i];
            need[request[0]][i] = need[request[0]][i] - request[i + 1];

            old_available[i] = available[i];
            available[i] -= request[i + 1];
        }

        if(!safety_check()){
            for (int i = 0; i < resource_num; i++) {
                allocation[request[0]][i] = old_allocation[request[0]][i];
                need[request[0]][i] = old_need[request[0]][i];
                available[i] = old_available[i];
            }
        }
        else {
            //for (int i = 0; i < resource_num; i++){
            //    if (available[i] <=0) is_continue = false;
            //}
            cout << "=====���뵽����Դ=====" << endl;
            cout << "����\t" << request[0] << endl;
            cout << "��Դ����";
            for (int i = 0; i < resource_num; i++){
                cout << request[i+1] << " ";
            }
            cout << "\n���ڰ�ȫ����";
            for (int i = 0; i < process_num; i++){
                cout << "P" << security[i] << " ";
            }
            cout << "\n=====��ǰ������Դ����=====" << endl;
            disp();
        }
    }
    else {
        cout << "=====������Դʧ��=====" << endl;
    }
}


bool banker::safety_check() {
	bool key = true;
	bool is_finial = false;
	int process = process_num-1, num = 0;
	int finished_process = 0;
	for (int i = 0; i < process_num; i++) finish[i] = false;
	for (int i = 0; i < resource_num; i++) work[i] = available[i];
	while (process >= 0) {
		if (!finish[process]) {
			for (int i = 0; i < resource_num; i++) {
				if (work[i] < need[process][i]) {
					key = false;
				}
			}

			if (key) {
				for (int i = 0; i < resource_num; i++) {
					work[i] += allocation[process][i];
				}
				finish[process] = true;
				security[num++] = process;
				process = process_num;
			}
			key = true;
		}
		process--;
	}

	for (int i = 0; i < process_num; i++) {
		if (finish[i]) {
			finished_process++;
		}
	}

	if (finished_process == process_num) {
		is_finial = true;
	}

	return is_finial;
}

void banker::disp() {
	cout << "---------------------------------------------------" << endl;
	cout << "| ���� " << " Max���� " << "| Allocation���� " << "| Need���� " << "| Available���� |" << endl;
	cout << "---------------------------------------------------" << endl;
	for (int i = 0; i < process_num; i++) {
		cout << "| " << "P" << i << " |";
		for (int j = 0; j < resource_num; j++){
            cout << " " << max[i][j] << "  ";
		}
        cout << "|";
		for (int j = 0; j < resource_num; j++){
            cout << " " << allocation[i][j] << "  ";
		}
        cout << "|";
		for (int j = 0; j < resource_num; j++){
            cout << " " << need[i][j] << "  ";
		}
        cout << "|";
		if (i == 0) {
            for (int j = 0; j < resource_num; j++){
                cout << "  " << available[j];
            }
            cout << "\t|" << endl;
		}
		else {
			cout <<"           |"<< endl;
		}
	}
}

void banker::init(){
    /*
        ���ݸ�ʽ��
        ������������Դ����
        ��Դ����
        ����������
        �������
        �������
    */
    ifstream in("data.txt");

    for (int i = 0; i < resource_num; i++){
        in >> available[i];
    }

    for (int i = 0; i < process_num; i++){
        for (int j = 0; j < resource_num; j++){
            in >> max[i][j];
        }
    }

    for (int i = 0; i < process_num; i++){
        for (int j = 0; j < resource_num; j++){
            in >> allocation[i][j];
        }
    }

    for (int i = 0; i < process_num; i++){
        for (int j = 0; j < resource_num; j++){
            in >> need[i][j];
        }
    }
	cout << "=====���ݳ�ʼ�����=====" << endl;
	bool init = safety_check();
    if (!init) cout << "=====��ʼ�������޷���������=====" << endl;
    else{
        cout << "=====��ʼ���ݴ��ڰ�ȫ����=====" << endl;
        for (int i = 0; i < process_num; i++){
            cout << "P" << security[i] << " ";
        }
        cout << "\n" << endl;
    }
}
int main() {
    //bool is_continue = true;
	banker model = banker();
	cout << "=====���ڼ���ģ��=====" << endl;
	model.init();
	model.disp();
	while (true) {
		model.send_request();
	}
	return 0;
}
