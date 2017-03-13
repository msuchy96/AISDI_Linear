#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#define SIZE_OF_ARRAY 5000
namespace aisdi
{


    template <typename Type>
    class Vector
    {

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        Vector()
        {
            first_empty=0;
            current_size=SIZE_OF_ARRAY;
            array=new Type[current_size];

        }

        Vector(std::initializer_list<Type> l)
        {

            first_empty=0;
            current_size=l.size()+5;
            array=new Type[current_size];

            auto iterator1=l.begin();
            while(iterator1!= l.end())
            {
                this->append(*iterator1);
                ++iterator1;
            }


        }

        Vector(const Vector& other) //costructor: taking elements from other to this vector
        {
            first_empty = 0;
            current_size = other.getSize() + 5;
            array = new Type[current_size];

            auto iterator1=other.begin();
            while(iterator1!= other.end())
            {
                this->append(*iterator1);
                ++iterator1;
            }

        }
        Vector(Vector&& other)  // cunstructor: taking elements from other to this vector and erasing them from other
        {

            array = other.array;
            current_size = other.getSize() + 5;
            first_empty = other.first_empty;

            other.array = new Type[SIZE_OF_ARRAY];
            other.current_size = 0;
            other.first_empty = 0;


        }

        ~Vector()
        {
            delete[] array;
        }

        Vector& operator=(const Vector& other)
        {
            delete[]array;
            first_empty=0;
            current_size = other.getSize() + 5;
            array = new Type[current_size];

            auto iterator1=other.begin();
            while(iterator1!= other.end())
            {
                this->append(*iterator1);
                ++iterator1;
            }
            return *this;
        }

        Vector& operator=(Vector&& other)
        {
            delete[]array;
            first_empty=0;
            current_size=other.getSize()+5;
            array = new Type[current_size];
            auto iterator1=other.begin();
            while(iterator1!= other.end())
            {
                this->append(*iterator1);
                ++iterator1;
            }

            other.erase(other.begin(),other.end());
            return *this;
        }

        bool isEmpty() const
        {
            return (first_empty==0);
        }

        size_type getSize() const
        {
            return first_empty;
        }


        void append(const Type& item)
        {
            if(current_size<first_empty+10)
                allocMoreMemory();
            array[first_empty]=item;
            first_empty++;

        }

        void prepend(const Type& item)
        {
            if(current_size<first_empty+10)
                allocMoreMemory();
            int i = first_empty;
            while(i>0)
            {
                array[i]=array[i-1];
                i--;
            }
            array[i] = item;
            first_empty++;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if(insertPosition.index>first_empty)
                throw std::logic_error("Logic: Can't insert element not in vector");
            if (current_size<first_empty + 10 ) allocMoreMemory();
            if (insertPosition == cend()) {
                append(item);
                return;
            }
            if (insertPosition == cbegin()) {
                prepend(item);
                return;
            }

            int whereInsert = insertPosition.index;
            int i=first_empty;
            while(i>whereInsert)
            {
                array[i]=array[i-1];
                i--;
            }
            array[whereInsert] = item;
            first_empty++;

        }

        Type popFirst()
        {
            if(this->isEmpty())
                throw std::out_of_range("Range: Can't pop from empty vector");
            Type temp = array[0];

            for(int i=0;i<(int)first_empty-1;i++)
                array[i]=array[i+1];
            first_empty--;

            return temp;
        }


        void allocMoreMemory()
        {
            Type *newArray;
            current_size = 2*current_size;
            newArray = new Type[current_size];
            for (int i = 0; i < (int)first_empty; i++)
                newArray[i] = array[i];
            delete [] array;
            array=newArray;
        }

        Type popLast()
        {
            if(this->isEmpty())
                throw std::out_of_range("Range: Can't pop from empty vector");
            Type temp = array[first_empty-1];
            first_empty--;
            return temp;
        }

        void erase(const const_iterator& possition)
        {

            if(this->isEmpty() || possition.index>=first_empty)
                throw std::out_of_range("Range: Iterator position is not proper");

            int ToEraseIndex=possition.index;

            for(int i=ToEraseIndex;i<(int)first_empty;i++)
                array[i]=array[i+1];
            --first_empty;
        }


        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {
            if(firstIncluded==lastExcluded)
                return;

            size_type begin_delete_index = firstIncluded.index;
            size_type end_delete_ex_index = lastExcluded.index;

            if(begin_delete_index>=end_delete_ex_index)
                throw std::logic_error("Logic: Begin of delete is further than End");

            if(end_delete_ex_index - begin_delete_index > first_empty/2)
            {
                size_type newSize= this->getSize() - (end_delete_ex_index - begin_delete_index) + 10;
                Type *newArray = new Type[newSize];
                int i = 0;
                auto currently_iterator=cbegin();
                while(currently_iterator!=cend())
                {
                    if (currently_iterator.index < begin_delete_index || currently_iterator.index >= end_delete_ex_index)
                    {
                        newArray[i] = *currently_iterator;
                        i++;
                    }
                    currently_iterator++;
                }
                delete[] array;
                array = newArray;
                first_empty = i;
                current_size = newSize;
            }
            else
            {
                while (end_delete_ex_index < first_empty) {
                    array[begin_delete_index] = array[end_delete_ex_index];
                    begin_delete_index++;
                    end_delete_ex_index++;
                }

                while (end_delete_ex_index > begin_delete_index) {
                    this->popLast();
                    end_delete_ex_index--;
                }
            }
        }

        iterator begin()
        {

            return iterator(cbegin());

        }

        iterator end()
        {
            return iterator(cend());
        }


        const_iterator cbegin() const
        {
            ConstIterator iterator1(this,&(array[0]),0);
            return iterator1;
        }

        const_iterator cend() const
        {
            if(this->isEmpty())
            {
                ConstIterator iterator1(this,&(array[0]),0);
                return iterator1;
            }
            ConstIterator iterator1(this,&(array[first_empty]),first_empty);
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
        Type *array;

        size_type first_empty;
        size_type current_size;

    };

    template <typename Type>
    class Vector<Type>::ConstIterator
    {


    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;



        explicit ConstIterator()
        {
            pter= nullptr;
        }
        ConstIterator(const Vector<Type> *Cont2,Type* actual,int index2):index(index2),Cont(Cont2),pter(actual)
        {

        }

        reference operator*() const
        {
            if(Cont->getSize()<=index)
                throw std::out_of_range("Range: Iterator is not on element of vector");
            return *pter;
        }

        ConstIterator& operator++() //pre
        {
            if(Cont->getSize()<=index)
                throw std::out_of_range("Range: Out of range ");
            pter++;
            index++;
            return *this;
        }

        ConstIterator operator++(int) //post
        {
            if(Cont->getSize()<=index)
                throw std::out_of_range("Range: Out of range");
            ConstIterator temp= *this;
            pter++;
            index++;
            return temp;

        }

        ConstIterator& operator--() // pre
        {
            if(index==0)
                throw std::out_of_range("Range: Out of range");
            pter--;
            index--;
            return *this;

        }

        ConstIterator operator--(int) // post
        {
            if(index==0)
                throw std::out_of_range("Range: Out of range");
            ConstIterator temp= *this;
            pter--;
            index--;
            return temp;
        }

        ConstIterator operator+(difference_type d) const
        {
            int n = static_cast<int>(d);
            if((index+n)>(Cont->first_empty))
                throw std::out_of_range("Range: Out of range");
            else
                return ConstIterator(Cont,&(Cont->array[index+n]) , index+n);
        }

        ConstIterator operator-(difference_type d) const
        {
            int n = static_cast<int>(d);
            if((int)(index-n)<0)
                throw std::out_of_range("Range: Out of range");
            else
                return ConstIterator(Cont,&(Cont->array[index-n]) , index-n);
        }

        bool operator==(const ConstIterator& other) const
        {
            if(pter==other.pter) return true;
            else return false;
        }

        bool operator!=(const ConstIterator& other) const
        {
            if(pter!=other.pter) return true;
            else return false;
        }

        size_type index;
    private:
        const Vector<Type> *Cont;
        Type *pter;



    };

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
    {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;


        explicit Iterator() {
        }

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

#endif // AISDI_LINEAR_VECTOR_H
