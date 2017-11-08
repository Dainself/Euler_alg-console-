#pragma once

#include <iostream>
#include <cstdlib>
#include "Stack.h"

using namespace std;

struct elem
{
	int node_ord;
	elem* p_next;
	bool passed;
	elem(int ord, elem* elem_next = NULL) : node_ord(ord), p_next(elem_next), passed(false) { }
};

bool CheckArr(elem* list, int* mass, int limit)
{
	for (int j = 0; j<limit; j++) // пока не повторяется в текущем массиве
		if (mass[j] == mass[limit])
			return true;
	for (elem* pt = list; pt != NULL; pt = pt->p_next) // пока не повторяется в текущем списке
		if (pt->node_ord == mass[limit])
			return true;
	return false;
}

class Matrix
{
private:
	elem** overlap_list; // сам список смежности
	int node_amount;

	void GenerateNodes(int i)
	{
		//придумываем кол-во смежных вершин
		int imagine_count = rand() % node_amount + 1;
		if (node_amount > 2)
			imagine_count = rand() % (node_amount - 2) + 1;
		int curr_count = CountNodes(i);
		if (curr_count >= imagine_count) // если сейчас уже здесь много вершин, я перехожу к след. списку
			return;
		int work_count = imagine_count - curr_count;
		if (work_count == 0)
			return;
		int* p_arr = new int[work_count];
		for (int j = 0; j < work_count; j++) // добавляем в массив пока что
		{
			do
				p_arr[j] = rand() % node_amount + 1;
			while (CheckArr(overlap_list[i], p_arr, j) || p_arr[j] == i + 1);//пока повторяется
		}
		// начинаем заносить из массива числа в текущий список. 
		for (int k = 0; k < work_count; k++)
		{
			if (overlap_list[i] == NULL)
				overlap_list[i] = new elem(p_arr[k]);
			else
				overlap_list[i] = new elem(p_arr[k], overlap_list[i]);
			if (overlap_list[p_arr[k] - 1] == NULL)
				overlap_list[p_arr[k] - 1] = new elem(i + 1);
			else
				overlap_list[p_arr[k] - 1] = new elem(i + 1, overlap_list[p_arr[k] - 1]);
		}
	}

	int CountNodes(int slot)
	{
		int counter = 0; // для текущего числа элементов в списке
		for (elem* pt = overlap_list[slot]; pt != NULL; pt = pt->p_next)
			counter++;
		return counter;
	}

	void Show()
	{
		cout << "\n{X} ";
		for (int i = 1; i <= node_amount; cout << "{" << i++ << "} ");
		cout << "\n";
		for (int pt = 0; pt < node_amount; pt++)
		{
			int counter = 1;
			elem* cp = overlap_list[pt];
			cout << "{" << pt+1 << "} ";
			while (counter <= node_amount)
			{
				if (cp->node_ord == counter)
				{
					cout << "[w] ";
					if (cp->p_next != NULL)
						cp = cp->p_next;
				}
				else
					cout << "--- ";
				counter++;
			}
			cout << "\n";
		}
		cout << "\n";
	}

	void Sort()
	{
		for (int k = 0; k < node_amount; k++)
		{
			elem* x = NULL;
			elem* nxt = NULL;
			for (elem* current = overlap_list[k]; current != NULL; current = nxt)
			{
				nxt = current->p_next;
				if (x == NULL)
				{
					x = current;
					current->p_next = NULL;
				}
				else if (current->node_ord < x->node_ord)
				{
					current->p_next = x;
					x = current;
				}
				else if (x->p_next == NULL)
				{
					x->p_next = current;
					current->p_next = NULL;
				}
				else for (elem* i = x; i != NULL; i = i->p_next)
				{
					if (i->p_next == NULL)
					{
						i->p_next = current;
						current->p_next = NULL;
						break;
					}
					else if (i->p_next->node_ord > current->node_ord)
					{
						current->p_next = i->p_next;
						i->p_next = current;
						break;
					}
				}
			}
			overlap_list[k] = x;
		}
	}

	int FindStartTrack()
	{
		for (int k = 0; k < node_amount; k++)
		{
			if (CountNodes(k) % 2 == 1)
				return k;
		}
		return 0;
	}
	/** \brief эйлеров обход по графу
		\param [in] slot - слот списка смежности
		\param [in] stack - стек, в который будут заноситься вершины
		\retval void */
	void Tracking(int slot, Stack*& stack)
	{
		elem* ptr; // создание указателя, будет указывать на необходимый список
		bool there_is = false; // показывает, нашли ли мы непройденные вершины
		for (ptr = overlap_list[slot]; ptr != NULL; ptr = ptr->p_next) // двигаясь по списку
		{
			if (!ptr->passed) // если вершина не пройдена
			{
				there_is = true; // даем об этом знать
				ptr->passed = true; // обозначаем ее пройденной
				//  если стек пуст, добавляем туда текущую вершину.
				if (stack->stk == NULL)
					stack->Add(slot + 1);
				/*  переходим в список, сответствующий нашей вершине, и ищем в нем вершину, смежную с текущим слотом; 
					когда находим, обозначаем ее пройденной и заносим в стек */
				for (elem* cp = overlap_list[(ptr->node_ord) - 1]; cp != NULL; cp = cp->p_next)
					if (cp->node_ord == slot + 1)
					{
						cp->passed = true;
						stack->Add(ptr->node_ord);
						break; // на этом оканчиваем цикл
					}
				break; // и здесь
			}
		}
		// если находили непройденную вершину, повторяем рекурсивно в том списке, с которым она связана
		if (there_is)
			Tracking((ptr->node_ord) - 1, stack);
		else return; // иначе, весь граф пройден
	}
public:
	Matrix(int n_amount) : node_amount(n_amount)
	{
		overlap_list = new elem*[node_amount];
		for (int i = 0; i < node_amount; overlap_list[i++] = NULL);
	}

	~Matrix()
	{
		for (int i = 0; i < node_amount; i++)
		{
			elem *cp = overlap_list[i], *cp_next = cp->p_next;
			// удаляем элементы списка
			while (true)
			{
				delete cp;
				if (cp_next == NULL)
					break;
				cp = cp_next;
				cp_next = cp_next->p_next;
			}
		}
		delete[] overlap_list;
	}

	void CreateEulerGraff()
	{
		srand(int(new int));
		for (int i = 0; i < node_amount; i++)
			/*Записываем в каждый список какие-нить вершины, минимум 1, максимум node_amount-2 штук.
			Проверяем сначала, не многовато ли вершин накопилось.
			Причем при записи мы заносим текущий i+1 в соотвествующий список (значение_вершины - 1).*/
			GenerateNodes(i); // работает!
		Sort();
		Show();
	}

	void CorrectGraff()
	{
		int n_amount;
		for (int i = 0; i < node_amount; i++)
		{
			if (IsCorrectGraff())
				break;
			n_amount = CountNodes(i);
			if (n_amount % 2 == 0)
				continue;
			else if (n_amount == 1)
				continue;
			else
			{
				for (elem* ptr = overlap_list[i]; ptr != NULL; ptr = ptr->p_next)
				{
					elem* ptr2 = overlap_list[(ptr->node_ord) - 1];
					int nodes_in_other = CountNodes((ptr->node_ord) - 1);
					if (nodes_in_other % 2 == 1 && nodes_in_other != 1)
					{
						//cout << "Deleting in " << (ptr->node_ord) - 1 << " and " << i << "\n";
						if (ptr2->node_ord == i + 1) // для второго
						{
							overlap_list[(ptr->node_ord) - 1] = ptr2->p_next;
							delete ptr2;
						}
						else for (elem* cp = overlap_list[(ptr->node_ord) - 1]->p_next; cp != NULL; cp = cp->p_next, ptr2 = ptr2->p_next) // найти здесь значение i и удалить RJCZR nen
						{
							if (cp->node_ord == i + 1)
							{
								ptr2->p_next = cp->p_next;
								delete cp;
								break;
							}
						}
						if (overlap_list[i] == ptr) // для первого
						{
							overlap_list[i] = ptr->p_next;
							delete ptr;
						}
						else
						{
							elem* ptr_prev;
							for (ptr_prev = overlap_list[i]; ptr_prev->p_next != ptr; ptr_prev = ptr_prev->p_next);
							ptr_prev->p_next = ptr->p_next;
							delete ptr;
						}
						break;
					}
				}
			}
		}
		Show();
	}

	bool IsCorrectGraff()
	{
		int counter = 0;
		for (int i = 0; i < node_amount; i++)
		{
			if (CountNodes(i) % 2 == 1)
				counter++;
		}
		if (counter > 2)
			return false;
		else
			return true;
	}

	void MakeTrack()
	{
		Stack* stack = new Stack();
		int i = FindStartTrack();
		Tracking(i, stack);
		stack->Show();
	}
};
