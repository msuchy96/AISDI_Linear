#include <cstddef>
#include <cstdlib>
#include <string>

#include "Vector.h"
#include "LinkedList.h"
#include <chrono>
#include <iostream>
namespace
{
/*
UWAGI:



od razu przy robieniu zmiennej dawac jej wartosc

jesli takie same rzeczy operacje wywolywac metode ktora to robi nawet jesli sprowadza sie to do tego samego warunku

+ w vectorze to nie przechodzenie po kolei tylko przeskok

przy std move PRZEPINAC a nie kopiowac i usuwac

GENERALNIE vector nie powinien dealokowac sobie pamieci na mniej



Liczba ptk: 7ptk

*/
    template <typename T>
    using Vector = aisdi::Vector<T>;

    template <typename T>
    using List = aisdi::LinkedList<T>;


    void perfomTest()
    {
        List<std::string> collection;
        collection.append("TODO");
    }

    void Test_append(int a)
    {
        List <std::string> collection_list;


        Vector <std::string> collection_vector;
        auto clock_b_vector = std::chrono::high_resolution_clock::now();
        for(int i=1;i<=a;i++)
        {

            collection_vector.append("text");

        }
        auto clock_e_vector = std::chrono::high_resolution_clock::now();
        std::cout<<"Adding using append "<<a<<" elements for vector takes: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_vector-clock_b_vector).count()<<" nanoseconds \n";


        auto clock_b_list = std::chrono::high_resolution_clock::now();
        for(int i=1;i<+a;i++)
        {
            collection_list.append("text");
        }
        auto clock_e_list = std::chrono::high_resolution_clock::now();
        std::cout<<"Adding using append "<<a<<" elements for list takes: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_list-clock_b_list).count()<<" nanoseconds \n";


    }

    void Test_prepend(int a)
    {
        List <std::string> collection_list;
        Vector <std::string> collection_vector;

        auto clock_b_vector = std::chrono::high_resolution_clock::now();
        for(int i=1;i<=a;i++)
        {

            collection_vector.prepend("text");

        }
        auto clock_e_vector = std::chrono::high_resolution_clock::now();
        std::cout<<"Adding using prepend "<<a<<" elements for vector takes: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_vector-clock_b_vector).count()<<" nanoseconds \n";


        auto clock_b_list = std::chrono::high_resolution_clock::now();
        for(int i=1;i<+a;i++)
        {
            collection_list.prepend("text");
        }
        auto clock_e_list = std::chrono::high_resolution_clock::now();
        std::cout<<"Adding using prepend "<<a<<" elements for list takes: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_list-clock_b_list).count()<<" nanoseconds \n";


    }
    void Test_ErasingAllElements(int a)
    {
        List <std::string> collection_list;
        Vector <std::string> collection_vector;
        for(int i=1;i<a;i++)
        {
            collection_list.append("text");
            collection_vector.append("text");
        }

        auto clock_b_vector = std::chrono::high_resolution_clock::now();
        collection_vector.erase(collection_vector.begin(),collection_vector.end());
        auto clock_e_vector = std::chrono::high_resolution_clock::now();
        std::cout<<"Erasing all ("<<a<<") elements from vector takes:  "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_vector-clock_b_vector).count()<<" nanoseconds \n";


        auto clock_b_list = std::chrono::high_resolution_clock::now();
        collection_list.erase(collection_list.begin(),collection_list.end());
        auto clock_e_list = std::chrono::high_resolution_clock::now();
        std::cout<<"Erasing all ("<<a<<") elements from list takes: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(clock_e_list-clock_b_list).count()<<" nanoseconds \n";





    }


} // namespace

int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
    for (std::size_t i = 0; i < repeatCount; ++i)
        perfomTest();

    Test_ErasingAllElements(3000);
    Test_append(1000);
    Test_prepend(1000);




    return 0;
}

