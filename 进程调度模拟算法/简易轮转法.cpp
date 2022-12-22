#include <iostream>
#include <string>
#include<fstream>
#include<windows.h>
using namespace std;

typedef struct pcb {
	string name;
	char state;
	int super;
	int ntime;
	int rtime;
	struct  pcb* next;
}PCB, * PCBlist;

PCBlist  input(int n) {
	PCBlist L = new PCB;
	PCB* p = L;
	L->next = NULL;
	fstream file;
	file.open("PCBcontent.txt");
	for (int i = 0; i < n; i++)
	{
		PCB* pr = new PCB;
		file >> pr->name;
		file >> pr->super;
		file >> pr->ntime;
		pr->rtime = 0;
		pr->state = 'w';
		pr->next = p->next;
		p->next = pr;
		p = p->next;
	}
	file.close();
	return L;
}

void running(PCB* p) {
	if (p->rtime < p->ntime) {
		p->state = 'r';
		p->rtime++;
	}
	else{
		p->state = 'f';
	}
}

int main() {
	PCBlist head = new PCB();
	head = input(5);
	int time = 0;
	while (true){
		for (PCB* p = head->next; p != NULL;p=p->next) {
			if(p->state=='w')running(p);
			cout << "名称   " << "状态    " << "所需的总时间    " << "已运行时间    " << endl;
			for (PCB* p = head->next; p != NULL; p = p->next) {
				cout << p->name << "    " << p->state  << "            " << p->ntime << "              " << p->rtime << endl;
			}
			if (p->state == 'r') {
				p->state = 'w';
			}
			Sleep(1000);
			system("cls");
			time++;
		}
		if (time > 30)break;
	}
	return 0;
}
