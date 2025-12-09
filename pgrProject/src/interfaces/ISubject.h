#include "IObserver.h"

template<typename T>
class ISubject {
public:
    virtual ~ISubject() = default;

    virtual void attach(IObserver<T>* obs) = 0;
    virtual void detach(IObserver<T>* obs) = 0;
    virtual void notify(const T& value) = 0;
};
