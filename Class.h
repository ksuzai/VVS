#include <iostream>
#include <initializer_list>
#include <vector>
#include <cmath>
using namespace std;
template<typename LT>
class tlist
{
protected:
	struct titem // ������� �������� 
	{
		LT m_value; // �������� ������ 
		titem* m_pNext; // ������ �� ��������� ������� 
		titem()
		{
			m_value = LT(0); // 0 ��� ������� �����, ������ ��� �������� 
			m_pNext = nullptr;
		};
	};
public:
	class iterator
	{
		friend class tlist; // �������� ����� ����� ������ ������ � ��������� 
	protected:
		titem* m_pItem; // ������ �� ������� ������ 
	public:
		iterator() // ����������� �� ��������� � ������� ������ 
		{
			m_pItem = nullptr; // �� ��������� �� �� ��� �� ��������� 
		};
		iterator(const iterator& it) // ����������� ����������� - ������� ������ 
		{
			m_pItem = it.m_pItem;
		}
	protected:
		iterator(titem* p) // ����������� ������������� � ������ ������ �� ������ 
		{
			m_pItem = p;
		};
	public:
		LT& operator*() // ��������* - ���������� �������� ������ 
		{
			return m_pItem->m_value;
		}
		iterator& operator++() // ���������� ++ 
		{
			m_pItem = m_pItem->m_pNext;
			return *this;
		}
		iterator operator++(int) // ����������� ++ 
		{
			iterator it(*this);
			m_pItem = m_pItem->m_pNext;
			return it;
		}
		iterator& operator=(const iterator& it) // ������������ ������������ 
		{
			m_pItem = it.m_pItem;
			return *this;
		}
		bool operator==(const iterator& it) // ��������� �� ��������� 
		{
			return (m_pItem == it.m_pItem ? true : false);
		}
		bool operator!=(const iterator& it) // ��������� �� ����������� 
		{
			return (m_pItem != it.m_pItem ? true : false);
		}
	};
public:
	iterator begin() { return iterator(m_pFirst); };
	iterator end() { return iterator(nullptr); };
	tlist(); // ����������� �� ��������� 
	tlist(const tlist& tl); // ����������� ����������� 
	tlist(tlist&& tl); // ����������� ����������� 
	~tlist();
	tlist(const LT* arr, int size); // ������������� ��������
	tlist(initializer_list<LT> init_list); // ������������� �������

	tlist& operator=(const tlist& tl); // �������� ������������ ������������
	tlist& operator=(tlist&& tl); // �������� ������������ ������������

	void print();

protected:
	titem* m_pFirst; // ������ ������ ������ 
protected:
	void init(); // ������� ������������� 
	void destroy(titem* pi); // ������� ������� ��������� 
public:
	int push_back(LT val); // ���������� � ����� ������ 
	int size(void); // ���������� ������� 
	int copy(const tlist& tl); // ����������� 
	int move(tlist& tl); // ����������� ������������ 
	
	tlist<LT>& operator+=(tlist<LT>& lt) {
		this->add(lt);
		return *this;
	}
	tlist<LT>& operator-=(tlist<LT>& lt) {
		this->sub(lt);
		return *this;
	}
	void add(tlist<LT>& tl) {
		this->copy(sum(*this, tl));
	}
	void sub(tlist<LT>& tl) {
		this->copy(diff(*this, tl));
	}

};
template<typename LT>
tlist<LT> operator+(tlist<LT>& a, tlist<LT>& b) {
	return sum(a, b);
}
template<typename LT>
tlist<LT> operator-(tlist<LT>& a, tlist<LT>& b) {
	return diff(a, b);
}
template<typename LT>
tlist<LT> sum(tlist<LT>& a, tlist<LT>& b) {
	tlist<LT> result;
	auto iter_a = a.begin();
	auto iter_b = b.begin();
	while (iter_a != a.end() && iter_b != b.end()) {
		result.push_back(*iter_a + *iter_b);
		++iter_a;
		++iter_b;
	}
	while (iter_a != a.end()) {
		result.push_back(*iter_a);
		++iter_a;
	}
	while (iter_b != b.end()) {
		result.push_back(*iter_b);
		++iter_b;
	}
	return result;
}
template<typename LT>
tlist<LT> diff(tlist<LT>& a, tlist<LT>& b) {
	tlist<LT> result;
	auto iter_a = a.begin();
	auto iter_b = b.begin();
	while (iter_a != a.end() && iter_b != b.end()) {
		result.push_back(*iter_a - *iter_b);
		++iter_a;
		++iter_b;
	}
	while (iter_a != a.end()) {
		result.push_back(*iter_a);
		++iter_a;
	}
	while (iter_b != b.end()) {
		result.push_back(-(*iter_b));
		++iter_b;
	}
	return result;
}

template<typename LT>
double dot_product(tlist<LT>& a, tlist<LT> b) {
	double result = 0;
	auto iter_a = a.begin();
	auto iter_b = b.begin();
	while (iter_a != a.end() && iter_b != b.end()) {
		result += (*iter_a) * (*iter_b);
		++iter_a;
		++iter_b;
	}
	return result;
}
template<typename LT>
double norm(tlist<LT>& vec) {
	return (sqrt(dot_product(vec, vec)));
}
template<typename LT>
// move
int tlist<LT>::move(tlist& tl) {
	if (this == &tl)
		return size();
	destroy(m_pFirst);
	m_pFirst = tl.m_pFirst;
	tl.init();
	return size();
}
// ������������� �������
template<typename LT>
tlist<LT>::tlist(initializer_list<LT> init_list) {
	init();
	for (auto& item : init_list) {
		push_back(item);
	}
}
// ������������� ��������
template<typename LT>
tlist<LT>::tlist(const LT* arr, int size) {
	init();
	for (int i = 0; i < size; i++) {
		push_back(arr[i]);
	}
}
// ������������
template<typename LT>
tlist<LT>& tlist<LT>::operator=(const tlist& tl) {
	if (this != &tl) {
		copy(tl);
	}
	return *this;
}
// ������������ ������������
template<typename LT>
tlist<LT>& tlist<LT>::operator=(tlist<LT>&& tl) {
	if (this != &tl) {
		move(tl);
	}
	return *this;
}
// �����
template<typename LT>
void tlist<LT>::print() {
	cout << "Num: " << size() << " Data: ";
	titem* p = m_pFirst;
	while (p) {
		cout << p->m_value << " ";
		p = p->m_pNext;
	}
	cout << endl;
}
template<typename LT>
tlist<LT>::tlist()
{
	init();
}
template<typename LT>
tlist<LT>::tlist(const tlist<LT>& tl)
{
	init();
	copy(tl);
}
template<typename LT>
tlist<LT>::tlist(tlist<LT>&& tl)
{
	init();
	move(tl);
}
template<typename LT>
tlist<LT>::~tlist()
{
	destroy(m_pFirst);
}
template<typename LT>
void tlist<LT>::init()
{
	m_pFirst = nullptr;
}
template<typename LT>
void tlist<LT>::destroy(titem* pi)
{
	titem* p;
	while (pi) // ���� �� ��������� (���� ������� ����������) 
	{
		p = pi->m_pNext; // ���������� ��������� ������� 
		delete pi; // ������� ������� ������� 
		pi = p; // ��������� ���� ������� 
	}
}
template<typename LT>
int tlist<LT>::push_back(LT val) // ���������� � ����� ������ 
{
	titem* pLast, * pNew;
	pNew = new titem; // ������� ������� 
	if (pNew) // ���� ������� ��������, �� ��������� � ����� 
	{
		if (m_pFirst) // ���������, ���� �� �������� �> ���� ��������� 
		{
			pLast = m_pFirst; // �������� � ������� 
			while (pLast->m_pNext) // ���� ���� ��������� 
			{
				pLast = pLast->m_pNext; // ��������� -> ��������� 
			}
			pLast->m_pNext = pNew; // ����� ��������� -> ����� ��������� 
		}
		else // ��������� �� ���� -> ����� ������ { 
			m_pFirst = pNew;
	}
	pNew->m_value = val; // ��������� ������ 
	return size() - 1; // ���������� ����� ������������ �������� } 
	return -1; // ������ 
}
template<typename LT>
int tlist<LT>::size(void) // ���������� ������� ������ 
{
	titem* p;
	int s = 0;
	p = m_pFirst; // �������� � ������� 
	while (p) // ���� �������� ���� 
	{
		p = p->m_pNext; // ��������� ������� 
		s++; // ��������� ������� 
	}
	return s;
}
template<typename LT>
int tlist<LT>::copy(const tlist<LT>& lt) // ����������� 
{
	if (this == &lt)return size(); // ��������� �� a = a 
	titem* p;
	tlist nl; // ������ ��� ����������� ����������� 
	p = lt.m_pFirst; // �������� � ������� �������� 
	while (p) // ���� �������� ���� 
	{
		if (nl.push_back(p->m_value) < 0)return 0;// ��������� �������� 
		// ���� ������, �� ������� 
		p = p->m_pNext; // ��������� ������� 
	}
	return move(nl); // ������� ������ �������� � ������� ������ 
};
