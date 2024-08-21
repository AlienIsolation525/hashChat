#pragma once
#define NAMELENGTH 10  // Зафикcируем длину имени

#include <string.h>
#include <iostream>

typedef char UserName[NAMELENGTH]; // тип имя фрукта

class HashTable { // хэш таблица
public:

    HashTable();
    ~HashTable();
    void add(UserName fr_name, int fr_count);

    void del(UserName fr_name);
    int find(UserName fr_name);
    void resize();
    //friend void test(HashTable& ht);

private:

    enum enPairStatus {
        free,
        engaged,
        deleted
    };

    struct Pair { // пара ключ-значение

        Pair() :
            fruit_name(""),
            fruit_count(-1),
            status(enPairStatus::free)
        {}
        Pair(UserName fr_name, int fr_count) :
            fruit_count(fr_count),
            status(enPairStatus::engaged) {
            memcpy(fruit_name, fr_name, NAMELENGTH);
        }
        Pair& operator = (const Pair& other) {
            fruit_count = other.fruit_count;
            memcpy(fruit_name, other.fruit_name, NAMELENGTH);
            status = other.status;

            return *this;
        }

        bool operator == (const Pair& other) {
            return  status == other.status &&
                (status != enPairStatus::engaged || (fruit_count == other.fruit_count && !strcmp(fruit_name, other.fruit_name)));
        }

        UserName fruit_name;
        int fruit_count;

        enPairStatus status;
    };

    int hash_func(UserName fr_name, int offset);

    Pair* array;
    int mem_size;
    int count;
};