#include <iostream>
#include <string>
#include <vector>


#define CREATE_HIVE_PROP_MEMBER(type, name)                                                        \
    hive_prop<type> name;                                                                          \
                                                                                                   \
    hive_script_prop<hive_prop<type>>::ref & get_##name()                                          \
    {                                                                                              \
        hive_script_prop<hive_prop<type>>::convert(name)                                           \
    }


template <class T> struct hive_prop {

    T data;

    std::ostream & serialize(std::ostream & stream)
    {

        // auto d = get_streamable_instance<T>(data);

        return stream;
    };

    constexpr hive_prop() noexcept : data() {}
    constexpr hive_prop(T const & value) noexcept : data(value) {}
};

template <class T> std::ostream & operator<<(std::ostream & lhs, hive_prop<T> const & rhs)
{
    return lhs << rhs.data;
}

struct Test {

    CREATE_HIVE_PROP_MEMBER(double, test)

    Test(double v) : test(v){};
};

int main()
{
    Test t(2.065);

    std::cout << t.test << std::endl;
}
