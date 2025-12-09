template<typename T>
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotified(const T& value) = 0;
};
