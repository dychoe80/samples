#include <utility>

template <typename F>
struct scope_exit
{
    explicit scope_exit(F f) noexcept
        : func_(std::move(f))
    {
    }

    scope_exit(scope_exit&& other) noexcept
        : func_((other.run_ = false, std::move(other.func_)))
        , run_(true)
    {
    }

    ~scope_exit()
    {
        if (m_run)
            func_();
    }

    F func_;
    bool run_;
};

template <typename F>
scope_exit<F> make_scope_exit(F&& f) noexcept
{
    return scope_exit<F>{ std::forward<F>(f) };
}
