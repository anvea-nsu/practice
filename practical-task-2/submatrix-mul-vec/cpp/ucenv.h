#ifndef UCENV_H
#define UCENV_H

#include <cstdlib>

class DF {
private:
    void* data;
    size_t size;
public:
    DF() : data(nullptr), size(0) {}
    ~DF() { if (data) free(data); }
    
    template<typename T>
    T* create(size_t count) {
        size = count * sizeof(T);
        data = malloc(size);
        return (T*)data;
    }
    
    template<typename T>
    T* getData() { return (T*)data; }
    
    template<typename T>
    const T* getData() const { return (const T*)data; }
    
    void operator=(double val) {
        if (!data) {
            data = malloc(sizeof(double));
            size = sizeof(double);
        }
        *(double*)data = val;
    }
};

#endif // UCENV_H
