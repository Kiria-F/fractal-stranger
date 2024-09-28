#include "core.h"

#include <QQmlEngine>
#include <QThreadPool>

#include "imagecontainer.h"

Core::Core(QObject* parent)
    : QObject(parent) {}

struct DPoint {
    double x;
    double y;
};

double doMagic(DPoint p) {
    auto result = p;
    int i = 0;
    int iMax = 250;
    for (; i < iMax; i++) {
        auto buf = result;
        result.x = buf.x * buf.x - buf.y * buf.y + p.x;
        result.y = 2 * buf.x * buf.y + p.y;
        if (result.x == INFINITY || result.x == -INFINITY || result.y == INFINITY
            || result.y == -INFINITY) {
            break;
        }
        if (result.x == 0 && result.y == 0) {
            i = iMax;
            break;
        }
    }
    return (double) i / iMax;
}

void RedrawTask::run() {
    ImageContainer* ic = new ImageContainer(width, height);
    QVector<double> buffer(width * height);
    double visibleWidth = width / scale;
    double visibleHeight = height / scale;
    double halfWidth = visibleWidth / 2;
    double halfHeight = visibleHeight / 2;
    double magicMin = doMagic({-halfWidth - offsetX, -halfHeight - offsetY});
    double magicMax = magicMin;
    int counter = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (cancellationToken) {
                delete ic;
                return;
            }
            double magic = doMagic({x * visibleWidth / width - halfWidth - offsetX,
                                    y * visibleHeight / height - halfHeight - offsetY});
            buffer[x * height + y] = magic;
            if (magic < magicMin)
                magicMin = magic;
            else if (magic > magicMax)
                magicMax = magic;
            counter++;
            if (counter == 1000) {
                emit core->setProgress((double) (x * height + y) / width / height);
                counter = 0;
            }
        }
    }
    double correction;
    if (magicMin < magicMax) {
        correction = 1.0 / (magicMax - magicMin) * 255;
    } else {
        correction = -1;
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int value;
            if (correction > 0) {
                value = (buffer[x * height + y] - magicMin) * correction;
            } else {
                value = 127;
            }
            ic->image.setPixelColor(x, y, QColor(value, value, value));
        }
    }
    QQmlEngine::setObjectOwnership(ic, QQmlEngine::JavaScriptOwnership);
    core->currentTask = nullptr;
    emit RedrawTask::done(ic);
}

void RedrawTask::cancel() {
    this->cancellationToken = true;
}

void Core::redraw(int width, int height, double scale, double offsetX, double offsetY) {
    if (currentTask) {
        currentTask->cancel();
    }

    auto task = new RedrawTask(this, width, height, scale, offsetX, offsetY);
    QObject::connect(task, &RedrawTask::done, this, &Core::redrawFinished);
    QThreadPool::globalInstance()->start(task);
    currentTask = task;
}
