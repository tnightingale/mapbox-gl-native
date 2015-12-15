#include "qmapboxgl_p.hpp"

#include <mbgl/annotation/point_annotation.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/platform/gl.hpp>
#include <mbgl/platform/qt/qmapboxgl.hpp>
#include <mbgl/sprite/sprite_image.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/util/geo.hpp>
#include <mbgl/util/vec.hpp>

#include <QImage>
#include <QString>

#include <chrono>

QMapboxGL::QMapboxGL(QObject *parent_) : QObject(parent_), d_ptr(new QMapboxGLPrivate(this))
{
}

QMapboxGL::~QMapboxGL()
{
}

void QMapboxGL::setAccessToken(const QString &token)
{
    d_ptr->fileSourceObj.setAccessToken(token.toUtf8().constData());
}

void QMapboxGL::setStyleJSON(const QString &style)
{
    d_ptr->mapObj.setStyleJSON(style.toUtf8().constData());
}

void QMapboxGL::setStyleURL(const QString &url)
{
    d_ptr->mapObj.setStyleURL(url.toUtf8().constData());
}

double QMapboxGL::latitude() const
{
    return d_ptr->mapObj.getLatLng().latitude;
}

void QMapboxGL::setLatitude(double latitude_)
{
    d_ptr->mapObj.setLatLng({ latitude_, longitude() });
}

double QMapboxGL::longitude() const
{
    return d_ptr->mapObj.getLatLng().longitude;
}

void QMapboxGL::setLongitude(double longitude_)
{
    d_ptr->mapObj.setLatLng({ latitude(), longitude_ });
}

double QMapboxGL::zoom() const
{
    return d_ptr->mapObj.getZoom();
}

void QMapboxGL::setZoom(double zoom_, int milliseconds)
{
    d_ptr->mapObj.setZoom(zoom_, std::chrono::milliseconds(milliseconds));
}

double QMapboxGL::minimumZoom() const
{
    return d_ptr->mapObj.getMinZoom();
}

double QMapboxGL::maximumZoom() const
{
    return d_ptr->mapObj.getMaxZoom();
}

QPointF QMapboxGL::coordinate() const
{
    const mbgl::LatLng& latLng = d_ptr->mapObj.getLatLng();

    return QPointF(latLng.latitude, latLng.longitude);
}

void QMapboxGL::setCoordinate(const QPointF &coordinate_, int milliseconds)
{
    d_ptr->mapObj.setLatLng(
        { coordinate_.x(), coordinate_.y() }, std::chrono::milliseconds(milliseconds));
}

void QMapboxGL::setCoordinateZoom(const QPointF &coordinate_, double zoom_, int milliseconds)
{
    d_ptr->mapObj.setLatLngZoom(
        { coordinate_.x(), coordinate_.y() }, zoom_, std::chrono::milliseconds(milliseconds));
}

double QMapboxGL::bearing() const
{
    return d_ptr->mapObj.getBearing();
}

void QMapboxGL::setBearing(double degrees, int milliseconds)
{
    d_ptr->mapObj.setBearing(degrees, std::chrono::milliseconds(milliseconds));
}

void QMapboxGL::setBearing(double degrees, const QPointF &center)
{
    d_ptr->mapObj.setBearing(degrees, { center.x(), d_ptr->size.height() - center.y() });
}

double QMapboxGL::pitch() const
{
    return d_ptr->mapObj.getPitch();
}

void QMapboxGL::setPitch(double pitch_, int milliseconds)
{
    d_ptr->mapObj.setPitch(pitch_, std::chrono::milliseconds(milliseconds));
}

void QMapboxGL::setGestureInProgress(bool inProgress)
{
    d_ptr->mapObj.setGestureInProgress(inProgress);
}

bool QMapboxGL::isRotating() const
{
    return d_ptr->mapObj.isRotating();
}

bool QMapboxGL::isScaling() const
{
    return d_ptr->mapObj.isScaling();
}

bool QMapboxGL::isPanning() const
{
    return d_ptr->mapObj.isPanning();
}

bool QMapboxGL::isFullyLoaded() const
{
    return d_ptr->mapObj.isFullyLoaded();
}

void QMapboxGL::moveBy(const QPointF &offset)
{
    d_ptr->mapObj.moveBy({ offset.x(), offset.y() });
}

void QMapboxGL::scaleBy(double scale, const QPointF &centerPixel, int milliseconds) {
    d_ptr->mapObj.scaleBy(
        scale, centerPixel.x(), centerPixel.y(), std::chrono::milliseconds(milliseconds));
}

void QMapboxGL::rotateBy(const QPointF &first, const QPointF &second)
{
    d_ptr->mapObj.rotateBy({ first.x(), first.y() }, { second.x(), second.y() });
}

void QMapboxGL::resize(const QSize& size)
{
    if (d_ptr->size == size) {
        return;
    }

    d_ptr->size = size;
    d_ptr->mapObj.update(mbgl::Update::Dimensions);
}

void QMapboxGL::addAnnotationIcon(const QString &name, const QImage &sprite)
{
    if (sprite.isNull()) {
        return;
    }

    const QImage swapped = sprite.rgbSwapped();

    d_ptr->mapObj.addAnnotationIcon(name.toStdString(), std::make_shared<mbgl::SpriteImage>(
        swapped.width(), swapped.height(), 1.0,
        std::string(reinterpret_cast<const char*>(swapped.constBits()), swapped.byteCount())));
}

QPointF QMapboxGL::pixelForCoordinate(const QPointF &coordinate_) const
{
    const mbgl::vec2<double> pixel =
        d_ptr->mapObj.pixelForLatLng({ coordinate_.x(), coordinate_.y() });

    return QPointF(pixel.x, d_ptr->size.height() - pixel.y);
}

QPointF QMapboxGL::coordinateForPixel(const QPointF &pixel) const
{
    const mbgl::LatLng latLng =
        d_ptr->mapObj.latLngForPixel({ pixel.x(), d_ptr->size.height() - pixel.y() });

    return QPointF(latLng.latitude, latLng.longitude);
}

void QMapboxGL::render()
{
    d_ptr->mapObj.render();
}

QMapboxGLPrivate::QMapboxGLPrivate(QMapboxGL *q)
    : QObject(q)
    , q_ptr(q)
    , fileSourceObj(nullptr)
    , mapObj(*this, fileSourceObj, mbgl::MapMode::Continuous, mbgl::GLContextMode::Shared)
{
    connect(this, SIGNAL(needsRendering()), q_ptr, SIGNAL(needsRendering()));
    connect(this, SIGNAL(mapRegionDidChange()), q_ptr, SIGNAL(mapRegionDidChange()));
}

QMapboxGLPrivate::~QMapboxGLPrivate()
{
}

float QMapboxGLPrivate::getPixelRatio() const
{
    // FIXME: Should handle pixel ratio.
    return 1.0;
}

std::array<uint16_t, 2> QMapboxGLPrivate::getSize() const
{
    return {{ static_cast<uint16_t>(size.width()), static_cast<uint16_t>(size.height()) }};
}

std::array<uint16_t, 2> QMapboxGLPrivate::getFramebufferSize() const
{
    return getSize();
}

void QMapboxGLPrivate::invalidate()
{
    emit needsRendering();
}

void QMapboxGLPrivate::notifyMapChange(mbgl::MapChange change)
{
    // Map thread.
    switch (change) {
    case mbgl::MapChangeRegionDidChange:
    case mbgl::MapChangeRegionDidChangeAnimated:
    case mbgl::MapChangeRegionIsChanging:
        emit mapRegionDidChange();
        break;
    default:
        break;
    }
}
