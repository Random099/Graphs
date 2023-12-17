#pragma once

#include <set>
#include <unordered_map>
#include <algorithm>
#include <vector>

template <typename T>
class DisjointSet
{
public:
    void setMake(const std::vector<T>& arr);
    T setFind(T element);
    void setUnion(const T& element1, const T& element2);
    const std::unordered_map<T, T>& dataGet();
private:
	std::unordered_map<T, T> _data;
};

//methods
template <typename T>
void DisjointSet<T>::setMake(const std::vector<T>& arr)
{
    for (auto& a : arr)
    {
        _data[a] = a;
    }
}

template <typename T>
T DisjointSet<T>::setFind(T element)
{
    if (_data[element] == element)
        return element;
    return setFind(_data[element]);
}

template <typename T>
void DisjointSet<T>::setUnion(const T& element1, const T& element2)
{
    T a = setFind(element1);
    T b = setFind(element2);
    _data[a] = b;
}

template <typename T>
const std::unordered_map<T, T>& DisjointSet<T>::dataGet()
{
    return _data;
}