#pragma once
template <typename T>
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
		t = p->data;
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
}