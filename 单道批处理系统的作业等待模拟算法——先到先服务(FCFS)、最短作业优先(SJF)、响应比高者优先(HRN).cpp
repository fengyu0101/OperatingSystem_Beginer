#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int Time = 0;//表示作业调度的运行时间
int select = 0;

typedef struct jcb {
	string name;
	int runTime=0;//所需运行时间
	int resource;//所占资源
	int subTime;//提交时刻
	int startTime;//开始运行时刻
	int finishTime;//结束运行时刻
	int turnoverTime;//周转时间
	int rightTime;//带权周转时间
	char state='w';
	float resRatio;//响应比
	struct jcb* prior;
	struct  jcb* next;
}JCB,Elem,*Elemtype;

class Queue {
public:
	Queue() { JCBHead = new Elem();	size = 0; }
	void Push(Elemtype j){
		if (j == NULL) {
			cout << "输入为空";
			return;
		}
		size++;
		Elemtype p = JCBHead;
		while (p->next != NULL) {
			p = p->next;
		}
		j->next = p->next;
		j->prior = p;
		p->next = j;
		
	}
	void Pop() {
		if (JCBHead->next == NULL) {
			cout << "队列空，无法删除";
			return;
		}
		size--;
		Elemtype p = JCBHead->next;
		JCBHead->next = p->next;
		if(p->next!=NULL)
			p->next->prior = JCBHead;
		delete(p);
	}
	Elemtype GetEnd() {
		return JCBHead->next;
	}
	//最短作业优先排序算法
	void SJF() {
		Elemtype p1 = JCBHead->next;
		Elemtype p2 = p1->next;
		for (int i = 0;i < size - 1;i++) {
			p1 = JCBHead->next;
			p2 = p1->next;
			while (p2 != NULL) {
				if (p1->runTime > p2->runTime) {
					p1->next = p2->next;
					if(p2->next!=NULL)
						p2->next->prior = p1;
					p2->prior = p1->prior;
					p1->prior->next = p2;
					p2->next = p1;
					p1->prior = p2;

					p2 = p1->next;
				}
				else {
					p1 = p1->next;
					p2 = p2->next;
				}
			}
		}
	}
	//计算每个作业的响应比
	void HRNindi() {
		Elemtype p = JCBHead->next;
		for (int i = 0;i < size;i++) {
			p->turnoverTime = i + p->runTime;
			p->resRatio = p->turnoverTime / p->runTime;
			p = p->next;
		}
	}
	//响应比高着优先排序算法
	void HRN() {
		HRNindi();
		Elemtype p1 = JCBHead->next;
		Elemtype p2 = p1->next;
		for (int i = 0;i < size - 1;i++) {
			p1 = JCBHead->next;
			p2 = p1->next;
			while (p2 != NULL) {
				if (p1->resRatio < p2->resRatio) {
					p1->next = p2->next;
					if (p2->next != NULL)
						p2->next->prior = p1;
					p2->prior = p1->prior;
					p1->prior->next = p2;
					p2->next = p1;
					p1->prior = p2;

					p2 = p1->next;
				}
				else {
					p1 = p1->next;
					p2 = p2->next;
				}
			}
		}
	}
	Elemtype JCBHead;
	int averTurnoverTime;//这组作业的平均周转时间
	int averRightTime;//这组作业的带权平均周转时间
	int size;
};

Queue CreatQueue(int n) {
	fstream JCBFile;
	JCBFile.open("JCB.txt");
	Queue JCBwait=Queue();
	for (int i = 0;i < n;i++) {
		Time++;
		JCB* j = new JCB();
		JCBFile >> j->name;
		JCBFile >> j->runTime;
		JCBFile >> j->resource;
		j->subTime = Time;
		JCBwait.Push(j);  
	}
	JCBFile.close();
	return JCBwait;
}

void Display(Elemtype j) {
	cout << j->name << "的开始运行时刻：" << j->startTime << endl;
	cout << j->name << "的结束运行时刻：" << j->finishTime << endl;
	cout << j->name << "的周转运行时刻：" << j->turnoverTime << endl;
	cout << j->name << "的带权周转运行时刻：" << j->rightTime << endl;
}

void Running(Queue &JCBwait) {
	while (JCBwait.JCBHead->next != NULL) {
		Elemtype j = JCBwait.GetEnd();
		j->state = 'r';
		j->startTime = Time;
		j->finishTime = j->startTime + j->runTime;
		j->turnoverTime = j->finishTime - j->subTime;
		j->rightTime = j->turnoverTime / j->runTime;
		Time += j->runTime;

		JCBwait.averTurnoverTime += j->turnoverTime;
		JCBwait.averRightTime += j->rightTime;
		Display(j);
		JCBwait.Pop();
		if(select==2&&JCBwait.size>1)
			JCBwait.HRN();
	}
}

int main() {
	cout << "选择作业调度算法(0为FCFS，1为SJF，2为HRN)：";
	cin >> select;
	Queue JCBwaitQueue = CreatQueue(5);
	if(select==1)
		JCBwaitQueue.SJF();
	else if(select==2)
		JCBwaitQueue.HRN();
	Running(JCBwaitQueue);
	cout << "这组作业的平均周转时间为：" << JCBwaitQueue.averTurnoverTime << endl;
	cout << "这组作业的带权平均周转时间为：" << JCBwaitQueue.averRightTime << endl;
	return 0;
}
