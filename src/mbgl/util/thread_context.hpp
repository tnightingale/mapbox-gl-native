#ifndef MBGL_UTIL_THREAD_CONTEXT
#define MBGL_UTIL_THREAD_CONTEXT

#include <mbgl/storage/file_source.hpp>

#include "resource.hpp"

#include <cstdint>
#include <string>
#include <thread>
#include <vector>

namespace mbgl {

class FileSource;

namespace util {

class GLObjectStore;

enum class ThreadPriority : bool {
    Regular,
    Low,
};

enum class ThreadType : uint8_t {
    Main,
    Map,
    Worker,
    Unknown,
};

struct ThreadContext {
public:
    ThreadContext(const std::string& name, ThreadType type, ThreadPriority priority);

    static void Set(ThreadContext* context);

    static bool currentlyOn(ThreadType type);
    static std::string getName();
    static ThreadPriority getPriority();

    static FileSource* getFileSourceHandlingResource(const Resource& res);
    static void addFileSource(FileSource* fileSource);
    static GLObjectStore* getGLObjectStore();
    static void setGLObjectStore(GLObjectStore* glObjectStore);

    std::string name;
    ThreadType type;
    ThreadPriority priority;

    std::vector<FileSource *> fileSources;
    GLObjectStore* glObjectStore = nullptr;
};

} // namespace util
} // namespace mbgl

#endif
