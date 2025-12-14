#include"List.h"

template <class T>
class TStack
{
	TList<T> stack;
public:
    TStack();
    TStack(const TStack& obj);
    TStack(TStack&& obj);
    ~TStack();

    int GetLen();
    void Push(T value);
    T Pop();
    bool IsEmpty();

    TStack& operator=(const TStack<T>& obj);
    TStack& operator=(TStack<T>&& obj);
    bool operator==(const TStack<T>& obj);
    bool operator!=(const TStack<T>& obj);
};

template<class T>
inline TStack<T>::TStack()
{
	stack = TList<T>();
}

template<class T>
inline TStack<T>::TStack(const TStack& obj)
{
	stack = obj.stack;
}

template<class T>
inline TStack<T>::TStack(TStack&& obj)
{
	stack = std::move(obj.stack);
}


template<class T>
inline TStack<T>::~TStack()
{}

template<class T>
inline int TStack<T>::GetLen()
{
    return stack.getSize();
}

template<class T>
inline void TStack<T>::Push(T value)
{
	stack.PushBack(value);
}

template<class T>
inline T TStack<T>::Pop()
{
    return stack.PopBack();
}

template<class T>
inline bool TStack<T>::IsEmpty()
{
    return stack.isEmpty();
}


template<class T>
inline TStack<T>& TStack<T>::operator=(const TStack<T>& obj)
{
	stack = obj.stack;
	return *this;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(TStack<T>&& obj)
{
	stack = std::move(obj.stack);
	return *this;
}

template<class T>
inline bool TStack<T>::operator==(const TStack<T>& obj)
{
    return (stack == obj.stack);
}

template<class T>
inline bool TStack<T>::operator!=(const TStack<T>& obj)
{
    return !(*this == obj);
}
