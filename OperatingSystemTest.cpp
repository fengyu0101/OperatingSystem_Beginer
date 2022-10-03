#include <iostream>
#include <string>
using namespace std;

typedef struct pcb {
	string name;
	char state;
	int super;
	int ntime;
	int rtime;
	struct  pcb* next;
}PCB,*PCBlist;

void sort(PCBlist& L, PCB* p) {
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
	PCBlist L=new PCB;
	L->next = NULL;
	for (int i = 0; i < n; i++)
	{
		PCB* pr = new PCB;
		cout << "请输入第" << i+1 << "个进程的名字：";
		cin >>pr-> name;
		cout << "请输入第" << i+1 << "个进程的优先级：";
		cin >> pr->super;
		cout << "请输入第" << i +1<< "个进程的所需运行时间：";
		cin >> pr->ntime;
		pr->rtime = 0;
		pr->state = 'w';
		sort(L, pr);
	}
	return L;
}

void display(PCB *p){
	cout << "目前的进程的信息" << endl;
	cout << "进程的名称：" << p->name<<endl	;
	cout << "进程的状态：" << p->state << endl;
	cout << "进程的优先级：" << p->super<<endl;
	cout << "进程的所需的总时间：" << p->ntime << endl;
	cout << "进程的已运行时间：" << p->rtime << endl;
}
void running(PCB*p) {
	if (p->rtime < p->ntime) {
		p->state = 'r';
		p->super--;
		p->rtime++;
	}
	else
	{
		p->state = 'f';
		cout << p->name << "进程已完成，现退出。以下是退出进程信息：" << endl;
		display(p);
	}
}

int main() {
	PCBlist head = new PCB();
	head = input(3);
	while (head->next != NULL) {
		PCB* p = head->next;
		running(p);
		if (p->state == 'f') {
			head->next = p->next;
			p->next = NULL;
			free(p);
		}
		else if (p->state == 'r') {
			p->state = 'w';
			head->next = p->next;
			p->next = NULL;
			sort(head, p);
		}
	}
	return 0;
}