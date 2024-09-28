#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QImage>
#include <QObject>

class ImageContainer : public QObject {
    Q_OBJECT
public:
    explicit ImageContainer(int width, int height, QObject* parent = nullptr);
    QImage image;
};

#endif // IMAGECONTAINER_H
