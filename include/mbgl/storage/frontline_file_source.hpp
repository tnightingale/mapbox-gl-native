#ifndef MBGL_STORAGE_FRONTLINE_FILE_SOURCE
#define MBGL_STORAGE_FRONTLINE_FILE_SOURCE

#include <mbgl/storage/file_source.hpp>
#include <mbgl/util/work_request.hpp>

namespace mbgl {

namespace util {
template <typename T> class Thread;
} // namespace util


class FrontlineFileSource : public FileSource {
public:
    FrontlineFileSource(const std::string& path);
    ~FrontlineFileSource() override;

    bool handlesResource(const Resource&) override;
    std::unique_ptr<FileRequest> request(const Resource&, Callback) override;

private:
    friend class FrontlineFileRequest;

    class Impl;
    const std::unique_ptr<util::Thread<Impl>> thread;
};

class FrontlineFileRequest : public FileRequest {
public:
    FrontlineFileRequest(const Resource& resource_,
                         FrontlineFileSource& fileSource_)
        : resource(resource_),
          fileSource(fileSource_) {
    }

    Resource resource;
    FrontlineFileSource& fileSource;

    std::unique_ptr<WorkRequest> workRequest;
};

} // namespace mbgl

#endif
