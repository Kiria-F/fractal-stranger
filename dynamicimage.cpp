#include "dynamicimage.h"

DynamicImage::DynamicImage(QQuickItem* parent)
    : QQuickPaintedItem(parent) {}

void DynamicImage::setImage(QObject* imageContainer) {
    ImageContainer* pc = qobject_cast<ImageContainer*>(imageContainer);
    this->scale = 1;
    this->offsetX = 0;
    this->offsetY = 0;
    this->imageContainer.image = pc->image;
    update();
}

void DynamicImage::stretch(double scale, int offsetX, int offsetY) {
    this->scale = scale;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    update();
}

void DynamicImage::paint(QPainter* painter) {
    int baseOffsetX;
    int baseOffsetY;
    if (scale > 1) {
        baseOffsetX = (scale - 1) / -2 * imageContainer.image.width();
        baseOffsetY = (scale - 1) / -2 * imageContainer.image.height();
    } else {
        baseOffsetX = (1.0 / scale - 1) / 2 * imageContainer.image.width() * scale;
        baseOffsetY = (1.0 / scale - 1) / 2 * imageContainer.image.height() * scale;
    }
    painter->drawImage(QRect(baseOffsetX + offsetX,
                             baseOffsetY + offsetY,
                             scale * imageContainer.image.width(),
                             scale * imageContainer.image.height()),
                       imageContainer.image);
}
