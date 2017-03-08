#include<iostream>
#include<windows.h>
#include <string>
#include "Queue.h"


; using namespace std;

#define SleepTime 1000

//为了测试时显示给io加的锁
CRITICAL_SECTION io_lock;

/*template <typename T>
class Queue
{
public:

	Queue()
	{
		Node<T> *node = new Node<T>();
		node->data = NULL;
		node->next = NULL;
		front = rear = node;
	}
	~Queue() {}

	template <typename T>
	struct Node
	{
		Node<T> *next;
		T data;
	};


	bool empty()
	{
		if (front == rear)
			return true;
		else
			return false;
	}


	bool pop(T & t)
	{
		Node<T> *p;

		do 
		{
			p = front->next;
			if (p == NULL)
				return false;
			if (p == rear)
				InterlockedCompareExchange((LPLONG)&rear, (LONG)front, (LONG)p);

		} while (InterlockedCompareExchange((LPLONG)&front->next, (LONG)p->next, (LONG)p) != (LONG)p);
		t= p->data;
		return true;

	}
	
	void push(const T &t)
	{
		Node<T> *node = new Node<T>;
		Node<T> *p;
		node->data = t;
		node->next = NULL;

		do {
			p = rear;
		} while (InterlockedCompareExchange((LPLONG)&p->next, (LONG)node, NULL) != NULL);

		InterlockedCompareExchange((LPLONG)&rear, (LONG)node, (LONG)p);
		//rear->next = node;
		//rear = node;
	}
private:
	Node<T> *front, *rear;
};*/

void Test1() 
{
	Queue<string*> q;
	string str1 = "teststr1";
	string str2 = "teststr2";
	q.push(&str1);
	q.push(&str2);
	string *i = new string();
	if (q.pop(i))
		cout << *i << endl;
	if (q.pop(i))
		cout << *i << endl;
	if (q.pop(i))
		cout << *i << endl;
	if (q.pop(i))
		cout << *i << endl;
	system("pause");
}

DWORD WINAPI Push(LPVOID lpParam)
{
	
	Queue<string*> *q = (Queue<string*> *)lpParam;
	for (;;)
	{
		string test = "test";
		q->push(&test);
		EnterCriticalSection(&io_lock);
		
		cout << GetCurrentThreadId() << "  push    success" << endl;
		
		LeaveCriticalSection(&io_lock);
		Sleep(SleepTime);
	}
	return 0;
}

DWORD WINAPI Pop(LPVOID lpParam)
{
	Queue<string*> *q = (Queue<string*> *)lpParam;
	for (;;)
	{
		string *test=new string();
		q->pop(test);
		EnterCriticalSection(&io_lock);
		if (*test == "") 
			cout << GetCurrentThreadId() << "  nothing pop"  << endl;
		else
			cout << GetCurrentThreadId() << "  pop     " << *test<< endl;
		LeaveCriticalSection(&io_lock);
	
		Sleep(SleepTime);
	}
	return 0;
}

void Test2() 
{
	InitializeCriticalSection(&io_lock);
	HANDLE handle[10];
	Queue<string*> q;

	for (int i = 0; i < 5; i++)
	{
		handle[i] = CreateThread(NULL, 0, Push, &q, 0, NULL);
	}

	for (int i = 5; i < 10; i++)
	{
		handle[i] = CreateThread(NULL, 0, Pop, &q, 0, NULL);
	}

	for (int i = 0; i < 10; i++)
	{
		WaitForSingleObject(handle[i], INFINITE);
	}
}

int main()
{
	Test1();
	Test2();

	return 0;
}