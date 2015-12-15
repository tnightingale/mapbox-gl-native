#include "../fixtures/util.hpp"

#include <mbgl/annotation/point_annotation.hpp>
#include <mbgl/annotation/shape_annotation.hpp>
#include <mbgl/sprite/sprite_image.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/platform/default/headless_view.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/util/io.hpp>
#include <mbgl/util/run_loop.hpp>

#include <future>
#include <vector>

using namespace mbgl;

std::shared_ptr<SpriteImage> defaultMarker() {
    PremultipliedImage image = decodeImage(util::read_file("test/fixtures/sprites/default_marker.png"));
    return std::make_shared<SpriteImage>(image.width, image.height, 1.0, std::string(reinterpret_cast<char*>(image.data.get()), image.size()));
}

void checkRendering(Map& map, const char * name) {
    test::checkImage(std::string("test/fixtures/annotations/") + name + "/",
                     test::render(map), 0.0002, 0.1);
}

TEST(Annotations, PointAnnotation) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    map.addAnnotationIcon("default_marker", defaultMarker());
    map.addPointAnnotation(PointAnnotation({ 0, 0 }, "default_marker"));

    checkRendering(map, "point_annotation");
}

TEST(Annotations, LineAnnotation) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");

    AnnotationSegments segments = {{ {{ { 0, 0 }, { 45, 45 } }} }};

    LineAnnotationProperties properties;
    properties.color = {{ 255, 0, 0, 1 }};
    properties.width = 5;

    map.addShapeAnnotation(ShapeAnnotation(segments, properties));

    checkRendering(map, "line_annotation");
}

TEST(Annotations, FillAnnotation) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");

    AnnotationSegments segments = {{ {{ { 0, 0 }, { 0, 45 }, { 45, 45 }, { 45, 0 } }} }};

    FillAnnotationProperties properties;
    properties.color = {{ 255, 0, 0, 1 }};

    map.addShapeAnnotation(ShapeAnnotation(segments, properties));

    checkRendering(map, "fill_annotation");
}

TEST(Annotations, StyleSourcedShapeAnnotation) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/annotation.json"), "");

    AnnotationSegments segments = {{ {{ { 0, 0 }, { 0, 45 }, { 45, 45 }, { 45, 0 } }} }};

    map.addShapeAnnotation(ShapeAnnotation(segments, "annotation"));

    checkRendering(map, "style_sourced_shape_annotation");
}

TEST(Annotations, AddMultiple) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    map.addAnnotationIcon("default_marker", defaultMarker());
    map.addPointAnnotation(PointAnnotation({ 0, -10 }, "default_marker"));

    test::render(map);

    map.addPointAnnotation(PointAnnotation({ 0, 10 }, "default_marker"));

    checkRendering(map, "add_multiple");
}

TEST(Annotations, NonImmediateAdd) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");

    test::render(map);

    AnnotationSegments segments = {{ {{ { 0, 0 }, { 0, 45 }, { 45, 45 }, { 45, 0 } }} }};

    FillAnnotationProperties properties;
    properties.color = {{ 255, 0, 0, 1 }};

    map.addShapeAnnotation(ShapeAnnotation(segments, properties));

    checkRendering(map, "non_immediate_add");
}

TEST(Annotations, RemovePoint) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    map.addAnnotationIcon("default_marker", defaultMarker());
    uint32_t point = map.addPointAnnotation(PointAnnotation({ 0, 0 }, "default_marker"));

    test::render(map);

    map.removeAnnotation(point);

    checkRendering(map, "remove_point");
}

TEST(Annotations, RemoveShape) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    AnnotationSegments segments = {{ {{ { 0, 0 }, { 45, 45 } }} }};

    LineAnnotationProperties properties;
    properties.color = {{ 255, 0, 0, 1 }};
    properties.width = 5;

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    uint32_t shape = map.addShapeAnnotation(ShapeAnnotation(segments, properties));

    test::render(map);

    map.removeAnnotation(shape);

    checkRendering(map, "remove_shape");
}

TEST(Annotations, ImmediateRemoveShape) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());
    Map map(view, fileSource, MapMode::Still);

    map.removeAnnotation(map.addShapeAnnotation(ShapeAnnotation({}, {})));
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");

    test::render(map);
}

TEST(Annotations, SwitchStyle) {
    util::RunLoop loop;

    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    DefaultFileSource fileSource(nullptr, test::getFileSourceRoot());

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    map.addAnnotationIcon("default_marker", defaultMarker());
    map.addPointAnnotation(PointAnnotation({ 0, 0 }, "default_marker"));

    test::render(map);

    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");

    checkRendering(map, "switch_style");
}
