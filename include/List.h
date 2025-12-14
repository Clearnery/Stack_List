#pragma once
#include<iostream>
#include<functional>
#include<vector>

template <class T>
class TNodeList
{
    T value;
    TNodeList* previous;
    TNodeList* next;

public:
    TNodeList();
    TNodeList(const T& value);
    TNodeList(TNodeList&& obj);
    ~TNodeList();

    bool operator==(TNodeList& obj);
    bool operator!=(TNodeList& obj);
    TNodeList& operator=(TNodeList& obj);
    TNodeList& operator=(TNodeList&& obj);

    const T& getValue() const;
    T& getValue() { return value; }
    TNodeList* getNext() const;
    TNodeList* getPrevious() const;

    void setValue(const T& newValue);
    void setNext(TNodeList* newNext);
    void setPrevious(TNodeList* newPrevious);
};

template<class T>
inline TNodeList<T>::TNodeList()
{
    next = nullptr;
    previous = nullptr;
    value = T();
}

template<class T>
inline TNodeList<T>::TNodeList(const T& value_)
{
    value = value_;
    next = nullptr;
    previous = nullptr;
}

template<class T>
inline TNodeList<T>::TNodeList(TNodeList&& obj)
{
    value = obj.value;
    previous = obj.previous;
    next = obj.next;
    obj.previous = nullptr;
    obj.next = nullptr;
    obj.value = 0;
}

template<class T>
inline TNodeList<T>::~TNodeList()
{
}

template<class T>
inline bool TNodeList<T>::operator==(TNodeList& obj)
{
    return value == obj.value;
}

template<class T>
inline bool TNodeList<T>::operator!=(TNodeList& obj)
{
    return !(*this == obj);
}

template<class T>
inline TNodeList<T>& TNodeList<T>::operator=(TNodeList& obj)
{
    if (this != &obj)
    {
        value = obj.value;
    }
    return *this;
}

template<class T>
inline TNodeList<T>& TNodeList<T>::operator=(TNodeList&& obj)
{
    if (this != &obj)
    {
        if (previous)
        {
            previous->next = next;
        }
        if (next)
        {
            next->previous = previous;
        }

        value = obj.value;
        previous = obj.previous;
        next = obj.next;

        obj.previous = nullptr;
        obj.next = nullptr;

        if (previous)
        {
            previous->next = this;
        }
        if (next)
        {
            next->previous = this;
        }
    }
    return *this;
}

template<class T>
inline const T& TNodeList<T>::getValue() const
{
    return value;
}

template<class T>
inline TNodeList<T>* TNodeList<T>::getNext() const
{
    return next;
}

template<class T>
inline TNodeList<T>* TNodeList<T>::getPrevious() const
{
    return previous;
}

template<class T>
inline void TNodeList<T>::setValue(const T& newValue)
{
    value = newValue;
}

template<class T>
inline void TNodeList<T>::setNext(TNodeList* newNext)
{
    next = newNext;
}

template<class T>
inline void TNodeList<T>::setPrevious(TNodeList* newPrevious)
{
    previous = newPrevious;
}

template<class T>
class TList
{
    TNodeList<T>* head;
    int size;

public:
    class Iterator
    {
    private:
        TNodeList<T>* current;

    public:
        Iterator() : current(nullptr) {}
        Iterator(TNodeList<T>* node) : current(node) {}

        T& operator*()
        {
            return current->getValue();
        }

        T* operator->()
        {
            return &(current->getValue());
        }

        Iterator& operator++()
        {
            if (current)
            {
                current = current->getNext();
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--()
        {
            if (current)
            {
                current = current->getPrevious();
            }
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Iterator& other)
        {
            return current == other.current;
        }

        bool operator!=(const Iterator& other)
        {
            return current != other.current;
        }

        explicit operator bool()
        {
            return current != nullptr;
        }

        TNodeList<T>* getNode() const
        {
            return current;
        }
    };

    TList();
    TList(const TList& other);
    TList(TList&& other) noexcept;
    ~TList();

    int getSize();
    TNodeList<T>* getHead();

    TList& operator=(const TList& other);
    bool operator==(const TList& other);
    bool operator!=(const TList& other);
    TList operator+(const TList& other);
    T operator[](int index);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const TList<U>& list);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, TList<U>& list);

    bool isEmpty();
    bool isFull();

    void PushFront(const T& value);
    void PushBack(const T& value);
    void PushAfter(int index, const T& value);
    T PopFront();
    T PopBack();
    void Remove(int index);

    int count(const T& value) const;
    std::vector<int> findAll(const T& value) const;
    void forEach(std::function<void(T&)> func);
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

template<class T>
inline TList<T>::TList()
{
    head = nullptr;
    size = 0;
}

template<class T>
inline TList<T>::TList(const TList& other)
{
    head = nullptr;
    size = 0;

    if (other.head == nullptr || other.size == 0)
    {
        return;
    }

    head = new TNodeList<T>(other.head->getValue());
    size = 1;

    TNodeList<T>* currentOther = other.head->getNext();
    TNodeList<T>* currentThis = head;

    for (int i = 1; i < other.size && currentOther != other.head; i++)
    {
        TNodeList<T>* newNode = new TNodeList<T>(currentOther->getValue());

        currentThis->setNext(newNode);
        newNode->setPrevious(currentThis);

        currentThis = newNode;
        currentOther = currentOther->getNext();

        size++;
    }

    if (size > 0 && currentThis != nullptr)
    {
        currentThis->setNext(head);
        head->setPrevious(currentThis);
    }
}

template<class T>
inline TList<T>::TList(TList&& other) noexcept
{
    head = other.head;
    size = other.size;
    other.head = nullptr;
    other.size = 0;
}

template<class T>
inline TList<T>::~TList()
{
    for (int i = 0; i < size; i++)
    {
        TNodeList<T>* temp = head;
        head = head->getNext();
        delete temp;
    }
}

template<class T>
inline int TList<T>::getSize()
{
    return size;
}

template<class T>
inline TNodeList<T>* TList<T>::getHead()
{
    return head;
}

template<class T>
inline TList<T>& TList<T>::operator=(const TList<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    while (head != nullptr)
    {
        TNodeList<T>* temp = head;
        head = head->getNext();
        delete temp;
        if (head == temp) break;
    }

    head = nullptr;
    size = 0;

    if (other.head == nullptr)
    {
        return *this;
    }

    head = new TNodeList<T>(other.head->getValue());
    size = 1;

    TNodeList<T>* currentOther = other.head->getNext();
    TNodeList<T>* currentThis = head;

    for (int i = 1; i < other.size && currentOther != other.head; i++)
    {
        currentThis->setNext(new TNodeList<T>(currentOther->getValue()));
        currentThis->getNext()->setPrevious(currentThis);
        currentThis = currentThis->getNext();
        currentOther = currentOther->getNext();
        size++;
    }

    if (head != nullptr && currentThis != nullptr)
    {
        currentThis->setNext(head);
        head->setPrevious(currentThis);
    }

    return *this;
}

template<class T>
inline bool TList<T>::operator==(const TList& other)
{
    if (this == &other)
    {
        return true;
    }

    if (size != other.size)
    {
        return false;
    }

    TNodeList<T>* nodeThis = head;
    TNodeList<T>* nodeOther = other.head;

    for (int i = 0; i < size; i++)
    {
        if (nodeThis->getValue() != nodeOther->getValue())
        {
            return false;
        }
        nodeThis = nodeThis->getNext();
        nodeOther = nodeOther->getNext();
    }

    return true;
}

template<class T>
inline bool TList<T>::operator!=(const TList& other)
{
    return !(*this == other);
}

template<class T>
inline TList<T> TList<T>::operator+(const TList& other)
{
    TList<T> result(*this);

    if (other.head != nullptr)
    {
        const TNodeList<T>* current = other.head;
        int count = 0;

        do
        {
            result.PushBack(current->getValue());
            current = current->getNext();
            count++;

            if (count > other.size * 2) break;

        } while (current != other.head && count < other.size);
    }

    return result;
}

template<class T>
inline T TList<T>::operator[](int index)
{
    if (this == nullptr)
    {
        throw std::runtime_error("List object is null");
    }

    if (head == nullptr)
    {
        throw std::runtime_error("List is empty");
    }

    if (index < 0)
    {
        throw std::invalid_argument("Index cannot be negative");
    }

    if (index >= size)
    {
        throw std::out_of_range("Index " + std::to_string(index) +
            " is too large for list of size " + std::to_string(size));
    }

    TNodeList<T>* current = head;

    for (int i = 0; i < index; i++)
    {
        if (current == nullptr)
        {
            throw std::runtime_error("Found null pointer while traversing to index " +
                std::to_string(index) + " at step " + std::to_string(i));
        }

        current = current->getNext();

        if (current == nullptr)
        {
            throw std::runtime_error("getNext() returned null at step " +
                std::to_string(i) + " while looking for index " +
                std::to_string(index));
        }
    }

    if (current == nullptr)
    {
        throw std::runtime_error("Null pointer at index " + std::to_string(index));
    }

    return current->getValue();
}

template<class T>
inline bool TList<T>::isEmpty()
{
    return (size == 0);
}

template<class T>
inline bool TList<T>::isFull()
{
    return !(size == 0);
}

template<class T>
inline void TList<T>::PushFront(const T& value)
{
    TNodeList<T>* newNode = new TNodeList<T>(value);

    if (head == nullptr)
    {
        head = newNode;
        head->setNext(head);
        head->setPrevious(head);
    }
    else
    {
        TNodeList<T>* tail = head->getPrevious();

        newNode->setNext(head);
        newNode->setPrevious(tail);

        head->setPrevious(newNode);
        tail->setNext(newNode);

        head = newNode;
    }

    size++;
}

template<class T>
inline void TList<T>::PushBack(const T& value)
{
    TNodeList<T>* newNode = new TNodeList<T>(value);

    if (head == nullptr)
    {
        head = newNode;
        head->setNext(head);
        head->setPrevious(head);
    }
    else
    {
        TNodeList<T>* tail = head->getPrevious();

        newNode->setNext(head);
        newNode->setPrevious(tail);

        tail->setNext(newNode);
        head->setPrevious(newNode);
    }

    size++;
}

template<class T>
inline void TList<T>::PushAfter(int index, const T& value)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("PushAfter: index " + std::to_string(index) +
            " out of range [0, " + std::to_string(size - 1) + "]");
    }

    if (head == nullptr)
    {
        throw std::runtime_error("PushAfter: list is empty");
    }

    TNodeList<T>* currentNode = head;
    for (int i = 0; i < index; i++)
    {
        currentNode = currentNode->getNext();
    }

    TNodeList<T>* newNode = new TNodeList<T>(value);

    newNode->setNext(currentNode->getNext());
    newNode->setPrevious(currentNode);

    currentNode->getNext()->setPrevious(newNode);
    currentNode->setNext(newNode);

    size++;
}

template<class T>
inline T TList<T>::PopFront()
{
    if (head == nullptr)
    {
        throw std::runtime_error("Cannot pop from empty list");
    }

    T value = head->getValue();

    if (size == 1)
    {
        delete head;
        head = nullptr;
    }
    else
    {
        TNodeList<T>* tail = head->getPrevious();
        TNodeList<T>* newHead = head->getNext();

        tail->setNext(newHead);
        newHead->setPrevious(tail);

        delete head;

        head = newHead;
    }

    size--;
    return value;
}

template<class T>
inline T TList<T>::PopBack()
{
    if (head == nullptr || size == 0)
    {
        throw std::runtime_error("Cannot pop from empty list");
    }

    T value;

    if (size == 1)
    {
        value = head->getValue();
        delete head;
        head = nullptr;
    }
    else
    {
        TNodeList<T>* tail = head->getPrevious();
        TNodeList<T>* newTail = tail->getPrevious();

        value = tail->getValue();

        newTail->setNext(head);
        head->setPrevious(newTail);

        delete tail;
    }

    size--;
    return value;
}

template<class T>
inline void TList<T>::Remove(int index)
{
    if (head == nullptr)
    {
        throw std::runtime_error("Remove: list is empty");
    }

    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Remove: index " + std::to_string(index) +
            " out of range [0, " + std::to_string(size - 1) + "]");
    }

    TNodeList<T>* nodeToRemove = head;
    for (int i = 0; i < index; i++)
    {
        nodeToRemove = nodeToRemove->getNext();
    }

    if (size == 1)
    {
        delete nodeToRemove;
        head = nullptr;
    }
    else
    {
        TNodeList<T>* prevNode = nodeToRemove->getPrevious();
        TNodeList<T>* nextNode = nodeToRemove->getNext();

        prevNode->setNext(nextNode);
        nextNode->setPrevious(prevNode);

        if (nodeToRemove == head)
        {
            head = nextNode;
        }

        delete nodeToRemove;
    }

    size--;

    if (size == 0 && head != nullptr)
    {
        head = nullptr;
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const TList<T>& list)
{
    out << "[";

    if (!list.isEmpty())
    {
        const TNodeList<T>* current = list.getHead();
        int printed = 0;

        do
        {
            if (printed > 0)
            {
                out << " <-> ";
            }
            out << current->getValue();
            current = current->getNext();
            printed++;

            if (printed >= list.getSize())
            {
                break;
            }

        } while (current != list.getHead());

        if (list.getSize() > 0)
        {
            out << " <-> ...";
        }
    }
    else
    {
        out << "empty";
    }

    out << "] (size: " << list.getSize() << ")";
    return out;
}

template<typename T>
std::istream& operator>>(std::istream& in, TList<T>& list)
{
    list.clear();

    size_t count;
    if (!(in >> count))
    {
        return in;
    }

    for (size_t i = 0; i < count; i++)
    {
        T value;
        if (!(in >> value))
        {
            break;
        }
        list.PushBack(value);
    }

    return in;
}


template<class T>
inline int TList<T>::count(const T& value) const
{
    if (head == nullptr || size == 0)
    {
        return 0;
    }

    int count = 0;
    const TNodeList<T>* current = head;
    int visited = 0;

    do
    {
        if (current->getValue() == value)
        {
            count++;
        }
        current = current->getNext();
        visited++;

        if (visited > size)
        {
            break;
        }

    } while (current != head);

    return count;
}

template<class T>
inline std::vector<int> TList<T>::findAll(const T& value) const
{
    std::vector<int> positions;

    if (head == nullptr || size == 0)
    {
        return positions;
    }

    const TNodeList<T>* current = head;
    int index = 0;
    int visited = 0;

    do
    {
        if (current->getValue() == value)
        {
            positions.push_back(index);
        }

        current = current->getNext();
        index++;
        visited++;

        if (visited > size)
        {
            break;
        }

    } while (current != head);

    return positions;
}

template<class T>
inline void TList<T>::forEach(std::function<void(T&)> func)
{
    if (head == nullptr || size == 0)
    {
        return;
    }

    TNodeList<T>* current = head;
    int visited = 0;

    do
    {
        func(current->getValue());
        current = current->getNext();
        visited++;

        if (visited > size)
        {
            break;
        }

    } while (current != head);
}

template<class T>
inline bool TList<T>::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    if (head != nullptr)
    {
        const TNodeList<T>* current = head;
        int saved = 0;

        do
        {
            T value = current->getValue();
            file.write(reinterpret_cast<const char*>(&value), sizeof(T));
            current = current->getNext();
            saved++;

            if (saved > size)
            {
                break;
            }

        } while (current != head);
    }

    file.close();
    return true;
}

template<class T>
inline bool TList<T>::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    while (head != nullptr)
    {
        TNodeList<T>* temp = head;
        head = head->getNext();
        delete temp;
        if (head == temp) break;
    }
    head = nullptr;
    size = 0;

    int fileSize;
    if (!file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize)))
    {
        file.close();
        return false;
    }

    for (int i = 0; i < fileSize; i++)
    {
        T value;
        if (!file.read(reinterpret_cast<char*>(&value), sizeof(T)))
        {
            file.close();
            return false;
        }
        PushBack(value);
    }

    file.close();
    return true;
}