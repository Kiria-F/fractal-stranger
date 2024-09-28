#ifndef DYNAMICIMAGE_H
#define DYNAMICIMAGE_H

#include <QPainter>
#include <QQuickPaintedItem>

#include "imagecontainer.h"

class DynamicImage : public QQuickPaintedItem {
    Q_OBJECT
    double scale = 1;
    int offsetX = 0;
    int offsetY = 0;

public:
    DynamicImage(QQuickItem* parent = Q_NULLPTR);

public slots:
    void setImage(QObject* imageContainer);
    void stretch(double scale, int offsetX, int offsetY);

protected:
    virtual void paint(QPainter* painter) Q_DECL_OVERRIDE;

private:
    ImageContainer imageContainer = ImageContainer(10, 10);
};

#endif // DYNAMICIMAGE_H
