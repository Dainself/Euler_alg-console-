#pragma once

#include <iostream>

using namespace std;

struct stk_elem
{
	int data;
	stk_elem* ptr_next;
	stk_elem(int digit) : data(digit), ptr_next(NULL) { }
};

class Stack
{
public:
	stk_elem* stk;
	stk_elem* p_curr;
	Stack() : stk(NULL), p_curr(stk) { }
	~Stack()
	{
		stk_elem *cp = stk, *cp_next = cp->ptr_next;
		while (true)
		{
			delete cp;
			if (cp_next == NULL)
				break;
			cp = cp_next;
			cp_next = cp_next->ptr_next;
		}
	}
	void Add(int num)
	{
		stk_elem* p_new_elem = new stk_elem(num); //создание указателя на новый элемент
		if (stk == NULL)
		{
			stk = p_new_elem;
			p_curr = stk;
		}
		else
		{
			{
				p_curr->ptr_next = p_new_elem;
				p_curr = p_new_elem;
			}
		}
	}
	void Show()
	{
		cout << "\nEuler's track:\n";
		for (stk_elem* i = stk; i != NULL; i = i->ptr_next)
			cout << i->data << " ";
		cout << "\n";
	}
};
