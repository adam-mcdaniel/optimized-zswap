#include "allocator.hpp"
/*
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


template<class T>
void Mallocator<T>::report(T* p, std::size_t n, bool alloc) const
{
    // boost::pfr::for_each_field(*p, [&](const auto& v) {
    //     std::cout << v;
    // });

    std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                << " bytes at " << std::hex << std::showbase
                << reinterpret_cast<void*>(p) << std::dec << '\n';
}
*/
