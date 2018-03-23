#pragma once
#include <queue>
#include "RecResult2D.h"

class Queue2D
{
private:
	int m_size;
	std::queue<RecResult2D*> m_q;
public:
	Queue2D()
	{
		m_size = 10;
	}
	Queue2D(int size)
	{
		m_size = size;
	}
	~Queue2D()
	{
		Clear();
	}
private:
	void RemoveFront()
	{
		RecResult2D* r = m_q.front();
		m_q.pop();
		delete r;
	}
public:
	void Clear()
	{
		while (!m_q.empty())
		{
			RemoveFront();
		}
	}
	bool Empty()
	{
		return m_q.empty();
	}
	RecResult2D* One()
	{
		if (m_q.empty())
			return NULL;
		return m_q.back();
	}
	//push result into queue2D(size = 10), if result is other person, clear queue2D then push
	void Push(RecResult2D* r2d)
	{
		RecResult2D* r = m_q.front();
		//clear queue if newR is other person
		if (!r2d->IsSamePerson(r))
			Clear();
		m_q.push(r2d);
		if (m_q.size() > m_size)
			RemoveFront();
	}

};