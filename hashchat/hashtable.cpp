#include "hashtable.h"

HashTable::HashTable() {
    count = 0;
    mem_size = 8;
    array = new Pair[mem_size];
}
HashTable::~HashTable() {
    delete[] array;
}
void HashTable::add(UserName fr_name, int fr_count) {

    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }



    //resize if
    if (i >= mem_size)
    {
        resize();
        add(fr_name, fr_count);
    }
    else
    {
        array[index] = Pair(fr_name, fr_count);
        count++;
    }
}
// HF
int HashTable::hash_func(UserName fr_name, int offset) {

    int index = -1, i = 0;

    const float A = 0.7;
    float midVal;

    int sum = 0;
    for (; i < strlen(fr_name); i++)
    {
        sum += fr_name[i];
    }

    midVal = sum / i;

    return int(offset * (A * midVal - int(A * midVal)));
}
// delete
void HashTable::del(UserName fr_name) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].fruit_name, fr_name)) {
            array[index].status = enPairStatus::deleted;
            count--;
            return;
        }
        else if (array[index].status == enPairStatus::free) {
            return;
        }
    }
}
// find
int HashTable::find(UserName fr_name) {


    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].fruit_name, fr_name)) {
            return array[index].fruit_count;
        }
        else if (array[index].status == enPairStatus::free) {
            return -1;
        }
    }
    return -1;
}
void HashTable::resize()
{
    int past_buffer_size = mem_size;
    mem_size *= 2;
    count = 0;
    Pair* ar2 = new Pair[mem_size];
    std::swap(array, ar2);

    for (int i = 0; i < past_buffer_size; ++i)
    {
        if (ar2[i].fruit_name && ar2[i].status == enPairStatus::engaged)
            add(ar2[i].fruit_name, ar2[i].fruit_count); // добавляем элементы в новый массив
    }

    delete[]ar2;
}