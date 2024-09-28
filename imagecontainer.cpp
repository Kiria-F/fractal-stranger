#include "imagecontainer.h"

ImageContainer::ImageContainer(int width, int height, QObject* parent)
    : QObject(parent) {
    this->image = QImage(width, height, QImage::Format_RGB32);
}
