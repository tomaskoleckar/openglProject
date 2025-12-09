#pragma once
#include "ISubject.h"
#include <vector>
#include <algorithm>

template<typename T>
class Subject : public ISubject<T> {
public:
    void attach(IObserver<T>* obs) override {
        observers.push_back(obs);
    }

    void detach(IObserver<T>* obs) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

    void notify(const T& value) override {
        for (auto* o : observers) {
            o->onNotified(value);
        }
    }

protected:
    std::vector<IObserver<T>*> observers;
};
