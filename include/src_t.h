#define src_t_bind
#ifdef src_t_bind
#include <tuple>       
#include <type_traits> 
#include <concepts>
// 占位符定义 - 创建一个模板结构体，用于表示参数占位符
template<int N> struct placeholder {};
extern placeholder<1> _1;  
extern placeholder<2> _2;  
extern placeholder<3> _3;  
extern placeholder<4> _4;  
extern placeholder<5> _5;  
extern placeholder<6> _6;  

template<typename T>
struct is_placeholder : std::integral_constant<int, 0> {};

// 占位符检测元函数的特化版本 - 针对placeholder<N>类型，返回N值
template<int N>
struct is_placeholder<placeholder<N>> : std::integral_constant<int, N> {};

// 检测是否为bind表达式的trait - 默认情况下，任何类型都不是bind表达式
template<typename T>
struct is_bind_expression : std::false_type {};

// 前向声明Binder类，以便在下面的特化中使用
template<typename Func, typename... BoundArgs>
class Binder;

// 将Binder标记为bind表达式 - 特化is_bind_expression，使其对任何Binder实例返回true
template<typename Func, typename... BoundArgs>
struct is_bind_expression<Binder<Func, BoundArgs...>> : std::true_type {};

// 定义概念：常量指针类型
template<typename T>
concept ConstPointer = std::is_pointer_v<std::decay_t<T>> &&
std::is_const_v<std::remove_pointer_t<std::decay_t<T>>>;

// 定义概念：非常量指针类型
template<typename T>
concept NonConstPointer = std::is_pointer_v<std::decay_t<T>> &&
!std::is_const_v<std::remove_pointer_t<std::decay_t<T>>>;

// 定义概念：常量非指针类型（用于常量通用引用）
template<typename T>
concept ConstNotPointer = !std::is_pointer_v<std::decay_t<T>> &&
std::is_const_v<std::remove_reference_t<T>>;

// 定义概念：非常量非指针类型（用于非常量通用引用）
template<typename T>
concept NonConstNotPointer = !std::is_pointer_v<std::decay_t<T>> &&
!std::is_const_v<std::remove_reference_t<T>>;

// 辅助函数0
template<typename Func, typename... Args>
decltype(auto) invoke_helper(Func&& f, Args&&... args) {
    return std::forward<Func>(f)(std::forward<Args>(args)...);
}

// 辅助函数1
template<typename R, typename C, typename... Params, NonConstPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...), Obj obj, Args&&... args) {
    return (obj->*f)(std::forward<Args>(args)...);
}

// 辅助函数2
template<typename R, typename C, typename... Params, ConstPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...), Obj obj, Args&&... args) {
    return (const_cast<C*>(obj)->*f)(std::forward<Args>(args)...);
}

// 辅助函数3
template<typename R, typename C, typename... Params, NonConstNotPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...), Obj&& obj, Args&&... args)
{
    return (obj.*f)(std::forward<Args>(args)...);
}

// 辅助函数4
template<typename R, typename C, typename... Params, ConstNotPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...), Obj&& obj, Args&&... args) {
    return (const_cast<C&>(obj).*f)(std::forward<Args>(args)...);
}

// 辅助函数5
template<typename R, typename C, typename... Params, NonConstPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...)const, Obj obj, Args&&... args) {
    return (obj->*f)(std::forward<Args>(args)...);
}

// 辅助函数6
template<typename R, typename C, typename... Params, ConstPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...)const, Obj obj, Args&&... args) {
    return (obj->*f)(std::forward<Args>(args)...);
}


// 辅助函数7
template<typename R, typename C, typename... Params, NonConstNotPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...)const, Obj&& obj, Args&&... args)
{
    return (obj.*f)(std::forward<Args>(args)...);
}

// 辅助函数8
template<typename R, typename C, typename... Params, ConstNotPointer Obj, typename... Args>
decltype(auto) invoke_helper(R(C::* f)(Params...)const, Obj&& obj, Args&&... args)
{
    return (obj.*f)(std::forward<Args>(args)...);
}


// 核心绑定器类 - 实现bind功能的主要类
template<typename Func, typename... BoundArgs>
class Binder {
private:
    Func m_func;                          // 存储要绑定的函数对象
    std::tuple<BoundArgs...> m_bound_args; // 存储绑定的参数

    // 处理参数转发 - 根据不同类型的参数执行不同的处理逻辑
    template<typename T, typename Tuple>
    decltype(auto) get_arg(T&& bound_arg,const Tuple& call_args) const
    {
        // 检查参数是否为占位符，并获取占位符的位置
        constexpr int pos = is_placeholder<std::decay_t<T>>::value;
        if constexpr (pos != 0)
        {
            // 如果是占位符，从调用参数中获取对应位置的参数
            return std::get<pos - 1>(call_args);
        }
        else if constexpr (is_bind_expression<std::decay_t<T>>::value)
        {
            // 如果是嵌套的bind表达式，递归调用该表达式
            return bound_arg(call_args);
        }
        else
        {
            // 如果是普通值，直接返回
            return std::forward<T>(bound_arg);
        }
    }

public:
    // 构造函数 - 初始化函数对象和绑定参数
    Binder(Func f, BoundArgs... args)
        : m_func(std::move(f)),                       // 移动函数对象
        m_bound_args(std::forward<BoundArgs>(args)...) { // 完美转发所有绑定参数
    }

    // 函数调用运算符 - 当绑定对象被调用时执行
    template<typename... CallArgs>
    decltype(auto) operator()(CallArgs&&... args) const
    {
        // 创建一个元组存储调用时传入的参数
        auto call_args = std::make_tuple(std::forward<CallArgs>(args)...);
        // 调用内部实现函数处理参数并执行被绑定的函数
        return invoke_impl(
            call_args,                                     // 传入调用参数
            std::make_index_sequence<sizeof...(BoundArgs)>() // 创建索引序列用于展开绑定参数
        );
    }

    // 修正的invoke_impl实现 - 实际执行函数调用的内部方法
    template<typename Tuple, std::size_t ...Is>
    decltype(auto) invoke_impl(const Tuple& call_args, std::index_sequence<Is...>) const
    {

        return invoke_helper(
            m_func,
            get_arg(std::get<Is>(m_bound_args), call_args)...
        );
    }
};

// 创建绑定器的辅助函数 - 简化Binder对象的创建过程
template<typename Func, typename... Args>
auto my_bind(Func&& f, Args&&... args) 
{
    // 创建并返回一个新的Binder对象，保留参数的值类别
    return Binder<std::decay_t<Func>, std::decay_t<Args>...>(
        std::forward<Func>(f),      // 完美转发函数对象
        std::forward<Args>(args)... // 完美转发所有绑定参数
    );
}
#endif