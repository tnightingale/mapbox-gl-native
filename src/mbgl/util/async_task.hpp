#ifndef MBGL_UTIL_ASYNC_TASK
#define MBGL_UTIL_ASYNC_TASK

#include <mbgl/util/noncopyable.hpp>
#include <mbgl/util/time.hpp>

#include <memory>
#include <functional>

namespace mbgl {
namespace util {

class AsyncTask : private util::noncopyable {
public:
    AsyncTask(std::function<void()>&&);
    ~AsyncTask();

    void send();
    void unref();

    void setThrottle(Duration timeout);

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace util
} // namespace mbgl

#endif
