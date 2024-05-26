#pragma once

#include <iostream>

template <class T>
class Container
{

public:
    struct Node
    {
        T data;
        Node *next;
        Node *prev;

        explicit Node(const T &val, Node *p = nullptr, Node *n = nullptr)
        {
            data = val;
            prev = p;
            next = n;
        }
        explicit Node(T &&val, Node *p = nullptr, Node *n = nullptr)
        {
            data = std::move(val);
            prev = p;
            next = n;
        }
    };

    class iterator
    {
    public:
        iterator() : current{nullptr} {}

        T &operator*()
        {
            if (current == nullptr)
            {
                throw std::out_of_range("Null iterator error");
            }
            return current->data;
        }

        iterator &operator++()
        {
            if (current == nullptr)
            {
                throw std::out_of_range("Incrementing null iterator error");
            }
            current = current->next;
            return *this;
        }

        bool operator==(const iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const iterator &other) const
        {
            return !(*this == other);
        }

        iterator &operator--()
        {
            if (current == nullptr)
            {
                throw std::out_of_range("Decrementing null iterator error");
            }
            current = current->prev;
            return *this;
        }

    private:
        Node *current;
        explicit iterator(Node *p) : current{p} {}
        friend class Container<T>;
    };

public:
    iterator begin() noexcept
    {
        return iterator(head);
    }

    iterator end() noexcept
    {
        return iterator(nullptr);
    }

public:
    Container()
    {
        create();
    }

    ~Container()
    {
        clear();
    }

public:
    uint32_t get_size()
    {
        return container_size;
    }

    bool is_empty()
    {
        return container_size == 0;
    }

    void pop_front()
    {
        if (is_empty())
        {
            throw std::out_of_range("PopFrontError: container is empty");
        }
        else
        {
            Node *t = head;
            head = head->next;
            if (head)
                head->prev = nullptr;
            delete t;
            --container_size;
        }
    }

    void pop_back()
    {
        if (is_empty())
        {
            throw std::out_of_range("PopBackError: container is empty");
        }
        else
        {
            Node *t = tail;
            tail = tail->prev;
            if (tail)
                tail->next = nullptr;
            delete t;
            --container_size;
        }
    }

    void push_front(const T &val)
    {
        if (is_empty())
            create_first_element(val);
        else
        {
            Node *t = new Node(val);
            t->next = head;
            head->prev = t;
            head = t;
            container_size += 1;
        }
    }

    void push_back(const T &val)
    {
        if (is_empty())
            create_first_element(val);
        else
        {
            Node *t = new Node(val);
            t->prev = tail;
            tail->next = t;
            tail = t;
            container_size += 1;
        }
    }
    void push_front(T &&val)
    {
        if (is_empty())
            create_first_element(std::move(val));
        else
        {
            Node *t = new Node(std::move(val));
            t->next = head;
            head->prev = t;
            head = t;
            ++container_size;
        }
    }
    void push_back(T &&val)
    {
        if (is_empty())
            create_first_element(std::move(val));
        else
        {
            Node *t = new Node(std::move(val));
            t->prev = tail;
            tail->next = t;
            tail = t;
            ++container_size;
        }
    }

    T &get_first_element()
    {
        if (is_empty())
        {
            throw std::out_of_range("GetFirstElementError: container is empty");
        }
        else
        {
            return head->data;
        }
    }

    T &get_last_element()
    {
        if (is_empty())
        {
            throw std::out_of_range("GetLastElementError: container is empty");
        }
        else
        {
            return tail->data;
        }
    }

    void clear()
    {
        while (container_size > 0)
        {
            pop_front();
        }
    }

    void swap(Container &other)
    {
        swap_elems(head, other.head);
        swap_elems(tail, other.tail);
        swap_elems(container_size, other.container_size);
    }

    void printList()
    {
        Node *node;
        node = head;
        while (node != NULL)
        {
            std::cout << "* " << node->data << std::endl;
            node = node->next;
        }
    }

    void reverse()
    {
        struct Node *temp = NULL;
        struct Node *current = head;

        while (current != NULL)
        {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }

        if (temp != NULL)
            head = temp->prev;
    }

private:
    void create()
    {
        container_size = 0;
        head = nullptr;
        tail = nullptr;
    }

    void create_first_element(const T &val)
    {
        head = new Node(val, nullptr, nullptr);
        tail = head;
        container_size = 1;
    }

    template <typename U>
    void swap_elems(U &a, U &b)
    {
        U t = a;
        a = b;
        b = t;
    }

private:
    Node *head;
    Node *tail;
    uint32_t container_size{};
};