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

void sort(PCBlist& L, PCB* p) {//每个新建的节点都在合适的地方插入，按优先数从大到小
	if (L->next == NULL) {
		p->next = L->next;
		L->next = p;
	}
	else {
		PCB* temp = new PCB;
		temp = L;
		if (p->super >= temp->next->super) {
			p->next = temp->next;
			temp->next = p;
			return;
		}
		while (temp->next != NULL) {
			temp = temp->next;
			if (temp->next == NULL) {
				p->next = temp->next;
				temp->next = p;
				break;
			}
			if (p->super < temp->super && p->super >= temp->next->super) {
				p->next = temp->next;
				temp->next = p;
				break;
			}
		}
	}
}

PCBlist  input(int n) {
	PCBlist L = new PCB;
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
		sort(L, pr);
	}
	file.close();
	return L;
}

void running(PCB* p) {
	if (p->rtime < p->ntime) {//模拟进程运行，每次运行，运行时间加1，优先数减1
		p->state = 'r';
		p->super--;
		p->rtime++;
	}
	else
	{
		p->state = 'f';
	}
	
}

int main() {
	PCBlist head = new PCB();
	head = input(5);
	int time = 0;
	while (true){
		for (int i = 0; i < 5;i++) {
			PCB* p = head->next;
			if(p->state=='w')running(p);
			if (p->state == 'f') {
				p->super = -10;//将完成的进程放到最后面
			}
			else if (p->state == 'r') {
				p->state = 'w';
			}
			head->next = p->next;
			p->next = NULL;
			sort(head, p);
			cout << "名称   " << "状态    " << "优先级    " << "所需的总时间    " << "已运行时间    " << endl;
			for (PCB* p = head->next; p != NULL; p = p->next) {
				cout << p->name << "    " << p->state << "        " << p->super << "            " << p->ntime << "               " << p->rtime << endl;
			}
			Sleep(1000);
			system("cls");
			time++;
		}
		if (time > 30)break;
	}
	
	if (head->next != NULL)system("cls");
	return 0;
}
