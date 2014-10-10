#ifndef SMART_ITER_H
#define SMART_ITER_H

#include <type_traits>
#include <iterator>

template<class C, typename T>
class SmartIter: public std::iterator<std::input_iterator_tag, T, ptrdiff_t, T*, T&> {
private:
	typename C::iterator iter;
	SmartIter(typename C::iterator iter): iter(iter) { }

	T& derefImpl(T* value)  const {return *value;}
	T& derefImpl(T& value)  const {return  value;}
	T* accessImpl(T* value) const {return  value;}
	T* accessImpl(T& value) const {return &value;}

public:
	static SmartIter<C, T> begin(C& container) {
		return SmartIter<C, T>(container.begin());
	}
	static SmartIter<C, T> end(C& container) {
		return SmartIter<C, T>(container.end());
	}

	T& operator*() const {
		return derefImpl(*iter);
	}
	T* operator->() const {
		return accessImpl(*iter);
	}

	SmartIter<C, T>& operator++() {
		iter++;
		return *this;
	}
	SmartIter<C, T> operator++(int) {
		SmartIter<C, T> copy = *this;
		copy.iter++;
		return copy;
	}

	bool operator==(const SmartIter<C, T>& rhs) const {
		return iter == rhs.iter;
	}
	bool operator!=(const SmartIter<C, T>& rhs) const {
		return iter != rhs.iter;
	}
};

#endif // SMART_ITER_H
