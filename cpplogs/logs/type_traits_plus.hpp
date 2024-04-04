#pragma once

#include <memory>
#include <type_traits>
#include <functional>

// #define enable_if(type) std::enable_if_t< type ## ::value , std::nullptr_t> = nullptr 

#define inherited_from 

namespace stdp {

    template<typename arg>
    using remove_cr = std::remove_const<std::remove_reference_t<arg>>;

    template<typename arg>
    using remove_cr_t = typename remove_cr<arg>::type;

    template <class T>
    struct type_identity {
        using type = T;
    };

    template <class T>
    using type_identity_t = typename type_identity<T>::type;

    template<class T>
    struct is_shared_ptr : std::false_type {};

    template<class T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

    template<class T, class B>
    using universal_t = typename std::enable_if_t<std::is_same<std::remove_reference_t<T>, B>::value, std::nullptr_t>;

    template<class T, class B>
    using universal_base_of_t = typename std::enable_if_t<std::is_base_of<B, std::remove_reference_t<T>>::value, std::nullptr_t>;
    

    template<class F>
    using return_type_t = typename decltype(std::function{std::declval<F>()})::result_type;
}


