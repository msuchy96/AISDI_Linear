#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

    template <typename Type> // typ TYPE
    struct Node
    {
        const Type data; // wartosc elementu
        Node *previous;
        Node *next;

        ~Node()
        {
            next=nullptr;
            previous=nullptr;
        }

        Node(const Type &A):data(A)
        {
            next=nullptr;
            previous=nullptr;

        }

    };

    template <typename Type>
    class LinkedList
    {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class Iterator;
        using iterator = Iterator;
        class ConstIterator;
        using const_iterator = ConstIterator;



        LinkedList()   //make empty list
        {
            first=new Node<Type>(Type{});
            last=new Node<Type>(Type{});
            last->next= nullptr;
            first->previous= nullptr;
            first->next=last;
            last->previous=first;

        }

        LinkedList(std::initializer_list<Type> l):LinkedList() // make initialized list
        {


            auto iterator1=l.begin();
            while(iterator1!=l.end())
            {
                this->append(*iterator1);
                iterator1++;
            }
        }

        LinkedList(const LinkedList& other):LinkedList() //costructor: taking elements from other to this List
        {

            auto iterator1=other.begin();
            while(iterator1!=other.end())
            {
                this->append(*iterator1);
                iterator1++;
            }
        }

        LinkedList(LinkedList&& other):LinkedList() // cunstructor: taking elements from other to this List and erasing them from other
        {

            first->next=other.first->next;
            last->previous=other.last->previous;
            other.first->next->previous=first;
            other.last->previous->next=last;
            other.first->next= other.last;
            other.last->previous= other.first;

        }


        ~LinkedList()
        {
            erase(begin(),end());
            delete last;
            delete first;
        }

        LinkedList& operator=(const LinkedList& other) // erasing all elements from List and copy all elements from other
        {
            this->erase(this->begin(), this->end());

            for(auto iterator1=other.begin();iterator1!=(other.end());++iterator1)
            {
                this->append(*iterator1);

            }
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) // erasing all elements from List.copy elements from other to List and erase all elements from other
        {
            this->erase(this->begin(), this->end());
            first->next=other.first->next;
            last->previous=other.last->previous;
            other.first->next->previous=first;
            other.last->previous->next=last;
            other.first->next= other.last;
            other.last->previous= other.first;

            return *this;

        }

        bool isEmpty() const
        {

            return first->next==last;
        }

        size_type getSize() const
        {
            if(this->isEmpty()) return 0;
            else
            {

                Node<Type>* temp = first->next;
                int SizeOfList=0;
                while(temp!=last)
                {
                    SizeOfList++;
                    temp=temp->next;
                }

                return SizeOfList;
            }
        }

        void append(const Type& item) //adding element at the end of list
        {
            Node<Type>* newNode= new Node<Type>(item);
            Node<Type>* CurrentlyLastElement=last->previous;
            last->previous=newNode;
            newNode->next=last;
            newNode->previous=CurrentlyLastElement;
            CurrentlyLastElement->next=newNode;
        }

        void prepend(const Type& item) //adding element at the beginning
        {
            Node<Type>* newNode= new Node<Type>(item);
            Node<Type>* CurrentlyFirstElement=first->next;
            first->next=newNode;
            newNode->next=CurrentlyFirstElement;
            newNode->previous=first;
            CurrentlyFirstElement->previous=newNode;

        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {

            Node<Type>* newNode = new Node<Type>(item);
            Node<Type>* prevNode = insertPosition.pter->previous;
            newNode->next=insertPosition.pter;
            newNode->previous=prevNode;
            prevNode->next=newNode;
            (insertPosition.pter)->previous=newNode;
        }


        Type popFirst() // returning erased first element
        {
            if(this->isEmpty()) throw std::out_of_range("Range Error: Can't erase element from empty list");
            auto temp=*begin();
            erase(begin());
            return temp;
        }
        Type popLast() // returning erased last element
        {
            if(this->isEmpty()) throw std::out_of_range("Range Error: Can't erase element from empty list");
            auto temp=*(end()-1);
            erase(end()-1);
            return temp;
        }

        void erase(const const_iterator& possition)
        {
            if(possition.pter->next==nullptr || possition.pter->previous==nullptr)
            {
                throw std::out_of_range("Range Error: Can't erase guardian");
            }

            possition.pter->next->previous=possition.pter->previous;
            possition.pter->previous->next=possition.pter->next;
            delete possition.pter;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {

            for(auto iterator1=firstIncluded;iterator1!=lastExcluded;)
            {
                auto iterator2=iterator1+1;
                this->erase(iterator1);
                iterator1=iterator2;
            }
        }

        iterator begin()
        {
            if(this->isEmpty())
            {
                Iterator iterator1(last);
                return iterator1;
            }
            else
            {
                Iterator iterator1(first->next);
                return iterator1;
            }

        }

        iterator end()
        {
            Iterator iterator1(last);
            return iterator1;
        }

        const_iterator cbegin() const
        {
            if(this->isEmpty())
            {
                ConstIterator iterator1(last);
                return iterator1;
            }
            else
            {
                ConstIterator iterator1(first->next);
                return iterator1;
            }
        }

        const_iterator cend() const
        {
            ConstIterator iterator1(last);
            return iterator1;
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }
    private:
        Node<Type> *first;
        Node<Type> *last;
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        explicit ConstIterator()
        {
        }

        ConstIterator(Node<Type>* actual)
        {
            pter=actual;
        }


        reference operator*() const
        {
            if(pter->next!=nullptr && pter->previous!=nullptr)
                return pter->data;
            else throw std::out_of_range("Range Error: Can't get data from guardian");
        }

        ConstIterator& operator++() // preincrementation
        {
            if(pter->next!=nullptr)
            {
                pter=pter->next;
                return *this;
            }
            else throw std::out_of_range("Range Error: Can't go further than last guardian");
        }

        ConstIterator operator++(int) // postincrementation
        {
            if(pter->next!=nullptr)
            {
                ConstIterator temp= *this;
                pter=pter->next;
                return temp;
            }
            else throw std::out_of_range("Range Error: Can't go forward - last guardian");
        }

        ConstIterator& operator--()
        {
            if(pter->previous->previous!=nullptr )
            {
                pter=pter->previous;
                return *this;
            }
            else throw std::out_of_range("Range Error: Can't go backward - first  guardian");
        }

        ConstIterator operator--(int)
        {
            if(pter->previous->previous!=nullptr)
            {
                ConstIterator temp= *this;
                pter=pter->previous;
                return temp;
            }
            else throw std::out_of_range("Range Error: Can't go backward - first  guardian" );
        }

        ConstIterator operator+(difference_type d) const
        {
            ConstIterator temp=*this;
            if(d>=0)
                while(d>0)
                {
                    --d;
                    ++temp;
                    if(temp.pter == nullptr) throw std::out_of_range("Range Error: Can't go forward - last  guardian" );
                }
            else
                while(d<0)
                {
                    ++d;
                    --temp;
                    if(temp.pter == nullptr) throw std::out_of_range("Range Error: Can't go backward - first  guardian" );
                }
            return temp;
        }

        ConstIterator operator-(difference_type d) const
        {
            ConstIterator temp=*this;
            if(d<=0)
                while(d<0)
                {
                    ++d;
                    --temp;
                    if(temp.pter == nullptr) throw std::out_of_range("Range Error: Can't go forward - last  guardian" );
                }
            else
                while(d>0)
                {
                    --d;
                    --temp;
                    if(temp.pter == nullptr) throw std::out_of_range("Range Error: Can't go backward - first  guardian" );
                }
            return temp;
        }

        bool operator==(const ConstIterator& other) const
        {

            return pter==other.pter;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return pter!=other.pter;

        }

    private:
        Node<Type>* pter;
        friend void LinkedList <Type>::insert(const const_iterator&, const Type&);
        friend void LinkedList <Type>::erase(const const_iterator&);


    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
