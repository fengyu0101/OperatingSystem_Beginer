#include <iostream>
#include <string>
using namespace std;

typedef struct pcb {
	string name;
	int ntime;
	int rtime;
	char state;
	struct pcb* prior;
	struct  pcb* next;
}PCB,*PCBlist;

PCBlist  input(int n) {
	PCBlist L=new PCB;
	L->prior = NULL;
	L->next = NULL;
	PCB* r = L;
	for (int i = 0; i < n; i++)
	{
		PCB* pr = new PCB;
		cout << "请输入第" << i+1 << "个进程的名字：";
		cin >>pr-> name;
		cout << "请输入第" << i +1<< "个进程的所需运行时间：";
		cin >> pr->ntime;
		pr->rtime = 0;
		pr->state = 'w';
		//尾插法建双向循环链表
		pr->next = r->next;
		pr->prior = r;
		r->next = pr;
		r = r->next;
		//建立循环
		if (i ==1)r->next = L->next;
		if (i == n - 1)r->next->prior = r;
	}
	return L;
}

void display(PCB *p){
	cout << "目前的进程的信息" << endl;
	cout << "进程的名称：" << p->name<<endl	;
	cout << "进程的所需的总时间：" << p->ntime << endl;
	cout << "进程的已运行时间：" << p->rtime << endl;
}
void Eliminate(PCB* p) {//消除已完成的进程
	p->prior->next = p->next;
	p->next->prior = p->prior;
	p->prior = NULL;
	p->next = NULL;
	free(p);
}
void running(PCB*p) {//运行进程一次
	if (p->rtime < p->ntime) {
		p->state = 'r';
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
	int n=0;//记录完成的进程的个数
	PCBlist head = new PCB();
	head = input(5);
	PCB* p = head->next;
	while (n<5) {
		running(p);
		p = p->next;
		if (p->prior->state == 'f') { Eliminate(p->prior);n++; }
	}
	return 0;
}