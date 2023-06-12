#pragma once
#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <boost/pfr.hpp>
#include <boost/pfr/core.hpp>
#include "pfr/traits.hpp"
#include <zlib.h>


// This type stores feature information about each type.
// It is used to help determine if a type is trivially compressible.
// The allocator will use this information to determine if it should
// allocate using zsmalloc or std::malloc.
struct TypeFeatures {
    std::string type_name;
    long unsigned int type_hash;
    float total_dealloc_entropy;
    int total_alloc_size;
    int total_allocs;
    int total_deallocs;

    TypeFeatures() : type_name(""), type_hash(0) {
        total_dealloc_entropy = 0;
        total_alloc_size = 0;
        total_allocs = 0;
        total_deallocs = 0;
    }

    TypeFeatures(const std::type_info &type) : type_name(type.name()), type_hash(type.hash_code()) {
        total_dealloc_entropy = 0;
        total_alloc_size = 0;
        total_allocs = 0;
        total_deallocs = 0;
    }

    float average_dealloc_entropy() {
        return total_dealloc_entropy / total_deallocs;
    }

    float average_alloc_size() {
        return total_alloc_size / total_allocs;
    }

    float average_dealloc_size() {
        return total_alloc_size / total_deallocs;
    }

    friend std::ostream &operator<<(std::ostream &os, const TypeFeatures &features) {
        os << "{" << "name: " << features.type_name << ", total_dealloc_entropy: " << features.total_dealloc_entropy << ", total_alloc_size: "
           << features.total_alloc_size << ", total_allocs: " << features.total_allocs << ", total_deallocs: "
           << features.total_deallocs << "}";
        return os;
    }
};

// A map of type_info hash codes to TypeFeatures.
// This is used to store feature information about each type.
// It is used to help determine if a type is trivially compressible.
// The allocator will use this information to determine if it should
// allocate using zsmalloc or std::malloc.
static std::map<long unsigned int, TypeFeatures> type_features;

template<class T>
struct Mallocator
{
    typedef T value_type;
 
    Mallocator () = default;
 
    template<class U>
    constexpr Mallocator (const Mallocator <U>&) noexcept {}
 
    T* allocate(std::size_t n);
 
    void deallocate(T* p, std::size_t n) noexcept;
private:
    void report(T* p, std::size_t n, bool alloc = true) const;
};
 
template<class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }

template<class T>
T* Mallocator<T>::allocate(std::size_t n)
{
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        throw std::bad_array_new_length();

    if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
    {
        report(p, n);
        return p;
    }

    throw std::bad_alloc();
}

template<class T>
void Mallocator<T>::deallocate(T* p, std::size_t n) noexcept
{
    report(p, n, 0);
    std::free(p);
}


template <typename T>
static float ShannonEntropy(T data[],int elements) {
    float entropy=0;
    std::map<T, long> counts;
    typename std::map<T, long>::iterator it;

    for (int dataIndex = 0; dataIndex < elements; ++dataIndex) {
        counts[data[dataIndex]]++;
    }

    it = counts.begin();
    while(it != counts.end()){
        float p_x = (float)it->second/elements;
        if (p_x>0) entropy-=p_x*log(p_x)/log(2);
        it++;
    }
    return entropy;
}

static float ShannonEntropy(std::string str, int elements){
    float entropy=0;
    std::map<char, long> counts;
    typename std::map<char, long>::iterator it;

    for (int dataIndex = 0; dataIndex < elements; ++dataIndex) {
        counts[str[dataIndex]]++;
    }

    it = counts.begin();
    while(it != counts.end()) {
        float p_x = (float)it->second/elements;
        if (p_x>0) entropy-=p_x*log(p_x)/log(2);
        it++;
    }
    return entropy;
}

static float IdealEntropy(int elements){
    float prob = 1.0/elements;
    return -1.0 * elements * prob * log(prob)/log(2);
}


// template<typename T, typename U = typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<T>()), bool>::value>::type>
// constexpr bool is_operator_greater_defined()
// {
//     return true;
// }

template<typename T>
constexpr auto has_operator_less(int) -> decltype(std::declval<T>() < std::declval<T>(), std::true_type{});

template<typename T>
constexpr std::false_type has_operator_less(...) { return std::false_type{}; }

template<typename T>
constexpr bool is_operator_less_defined()
{
    return decltype(has_operator_less<T>(0))::value;
}

template<typename T>
constexpr bool is_type_reflectable()
{
    return pfr::is_implicitly_reflectable<T, T>::value;
}

template<class T>
void Mallocator<T>::report(T* p, std::size_t n, bool alloc) const
{
    /*
    This code implements reporting for data that can be reflected using boost::pfr
    */
    /*
    if constexpr (is_type_reflectable<T>()) {
        // std::cout << typeid(*p).name() << " is reflectable\n";
        // boost::pfr::for_each_field(*p, [&](const auto& v) {
        //     std::cout << v << '\n';
        // });
        if (!alloc)
            for (int i = 0; i < n; i++)
                std::cout << boost::pfr::io_fields(*p) << '\n';
    }

    std::map<long unsigned int, TypeFeatures>::iterator features = type_features.find(typeid(*p).hash_code());
    if (features == type_features.end()) {
        type_features[typeid(*p).hash_code()] = TypeFeatures(typeid(*p));
        features = type_features.find(typeid(*p).hash_code());
    }

    if (alloc) {
        features->second.total_allocs++;
        features->second.total_alloc_size += sizeof(T) * n;
    }
    else {
        features->second.total_deallocs++;
        if constexpr (is_operator_less_defined<T>()) {
            // std::cout << "has_operator_less\n";
            features->second.total_dealloc_entropy += ShannonEntropy(p, n);
        } else if constexpr (is_type_reflectable<T>()) {
            float sum = 0.0;

            boost::pfr::for_each_field(*p, [&](const auto& v) {
                if constexpr (is_operator_less_defined<decltype(v)>()) {
                    sum += ShannonEntropy(v, n);
                }
            });

            features->second.total_dealloc_entropy += sum;
        }
    }

    std::cout << features->second << '\n';
    */


    /*
    Test the compressibility of the bytes in the deallocated memory.    
    */
    const char* input_data = (const char*)p;
    const size_t input_size = sizeof(T) * n;
    uLong compressed_size = compressBound(input_size);  // Get an upper bound for the compressed size
    Bytef* compressed_data = (Bytef*)malloc(compressed_size);

    int result = compress2(compressed_data, &compressed_size, (const Bytef*)input_data, input_size, Z_BEST_COMPRESSION);
    if (result == Z_OK) {
        printf("Compression successful!\n");
        printf("Original size: %lu bytes\n", input_size);
        printf("Compressed size: %lu bytes\n", compressed_size);
    } else {
        printf("Compression failed. Error code: %d\n", result);
    }



    std::map<long unsigned int, TypeFeatures>::iterator features = type_features.find(typeid(*p).hash_code());
    if (features == type_features.end()) {
        type_features[typeid(*p).hash_code()] = TypeFeatures(typeid(*p));
        features = type_features.find(typeid(*p).hash_code());
    }
}
