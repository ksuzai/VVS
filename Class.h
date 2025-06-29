#include <iostream>
#include <initializer_list>
#include <vector>
#include <cmath>
using namespace std;
template<typename LT>
class tlist
{
protected:
	struct titem // элемент хранения 
	{
		LT m_value; // значение данных 
		titem* m_pNext; // ссылка на следующий элемент 
		titem()
		{
			m_value = LT(0); // 0 для простых типов, пустой для объектов 
			m_pNext = nullptr;
		};
	};
public:
	class iterator
	{
		friend class tlist; // основной класс имеет полный доступ к итератору 
	protected:
		titem* m_pItem; // ссылка на элемент списка 
	public:
		iterator() // конструктор по умолчанию – внешний доступ 
		{
			m_pItem = nullptr; // по умолчанию ни на что не ссылается 
		};
		iterator(const iterator& it) // конструктор копирования - внешний доступ 
		{
			m_pItem = it.m_pItem;
		}
	protected:
		iterator(titem* p) // конструктор инициализации – доступ только из списка 
		{
			m_pItem = p;
		};
	public:
		LT& operator*() // оператор* - возвращает значение данных 
		{
			return m_pItem->m_value;
		}
		iterator& operator++() // префиксный ++ 
		{
			m_pItem = m_pItem->m_pNext;
			return *this;
		}
		iterator operator++(int) // постфиксный ++ 
		{
			iterator it(*this);
			m_pItem = m_pItem->m_pNext;
			return it;
		}
		iterator& operator=(const iterator& it) // присваивание копированием 
		{
			m_pItem = it.m_pItem;
			return *this;
		}
		bool operator==(const iterator& it) // сравнение на равенство 
		{
			return (m_pItem == it.m_pItem ? true : false);
		}
		bool operator!=(const iterator& it) // сравнение на неравенство 
		{
			return (m_pItem != it.m_pItem ? true : false);
		}
	};
public:
	iterator begin() { return iterator(m_pFirst); };
	iterator end() { return iterator(nullptr); };
	tlist(); // конструктор по умолчанию 
	tlist(const tlist& tl); // конструктор копирования 
	tlist(tlist&& tl); // конструктор перемещения 
	~tlist();
	tlist(const LT* arr, int size); // инициализация массивом
	tlist(initializer_list<LT> init_list); // инициализация списком

	tlist& operator=(const tlist& tl); // оператор присваивания копированием
	tlist& operator=(tlist&& tl); // оператор присваивания перемещением

	void print();

protected:
	titem* m_pFirst; // храним начало списка 
protected:
	void init(); // функция инициализации 
	void destroy(titem* pi); // функция очистки элементов 
public:
	int push_back(LT val); // добавление в конец списка 
	int size(void); // вычисление размера 
	int copy(const tlist& tl); // копирование 
	int move(tlist& tl); // копирование перемещением 
	
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
// инициализация списком
template<typename LT>
tlist<LT>::tlist(initializer_list<LT> init_list) {
	init();
	for (auto& item : init_list) {
		push_back(item);
	}
}
// инициализация массивом
template<typename LT>
tlist<LT>::tlist(const LT* arr, int size) {
	init();
	for (int i = 0; i < size; i++) {
		push_back(arr[i]);
	}
}
// присваивание
template<typename LT>
tlist<LT>& tlist<LT>::operator=(const tlist& tl) {
	if (this != &tl) {
		copy(tl);
	}
	return *this;
}
// присваивание перемещением
template<typename LT>
tlist<LT>& tlist<LT>::operator=(tlist<LT>&& tl) {
	if (this != &tl) {
		move(tl);
	}
	return *this;
}
// вывод
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
	while (pi) // цикл по элементам (пока элемент существует) 
	{
		p = pi->m_pNext; // запоминаем следующий элемент 
		delete pi; // удаляем текущий элемент 
		pi = p; // следующий стал текущим 
	}
}
template<typename LT>
int tlist<LT>::push_back(LT val) // добавление в конец списка 
{
	titem* pLast, * pNew;
	pNew = new titem; // создаем элемент 
	if (pNew) // если элемент создался, то добавляем в конец 
	{
		if (m_pFirst) // проверяем, были ли элементы –> ищем последний 
		{
			pLast = m_pFirst; // начинаем с первого 
			while (pLast->m_pNext) // пока есть следующий 
			{
				pLast = pLast->m_pNext; // следующий -> последний 
			}
			pLast->m_pNext = pNew; // нашли последний -> новый следующий 
		}
		else // элементов не было -> новый первый { 
			m_pFirst = pNew;
	}
	pNew->m_value = val; // сохраняем данные 
	return size() - 1; // возвращаем номер добавленного элемента } 
	return -1; // ошибка 
}
template<typename LT>
int tlist<LT>::size(void) // вычисление размера списка 
{
	titem* p;
	int s = 0;
	p = m_pFirst; // начинаем с первого 
	while (p) // пока элементы есть 
	{
		p = p->m_pNext; // следующий элемент 
		s++; // добавляем счетчик 
	}
	return s;
}
template<typename LT>
int tlist<LT>::copy(const tlist<LT>& lt) // копирование 
{
	if (this == &lt)return size(); // проверяем на a = a 
	titem* p;
	tlist nl; // объект для безопасного копирования 
	p = lt.m_pFirst; // начинаем с первого элемента 
	while (p) // пока элементы есть 
	{
		if (nl.push_back(p->m_value) < 0)return 0;// добавляем значение 
		// если ошибка, то выходим 
		p = p->m_pNext; // следующий элемент 
	}
	return move(nl); // готовые данные передаем в текущий объект 
};
