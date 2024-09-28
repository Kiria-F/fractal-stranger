#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QRunnable>

class Core;

class RedrawTask : public QObject, public QRunnable {
    Q_OBJECT
    Core* core;
    int width;
    int height;
    double scale;
    double offsetX;
    double offsetY;
    bool cancellationToken = false;

public:
    RedrawTask(Core* core, int width, int height, double scale, double offsetX, double offsetY)
        : core(core)
        , width(width)
        , height(height)
        , scale(scale)
        , offsetX(offsetX)
        , offsetY(offsetY) {}
    void run() override;
    void cancel();

signals:
    void done(QObject* imageContainer);
};

class Core : public QObject {
    Q_OBJECT

public:
    RedrawTask* currentTask = nullptr;
    explicit Core(QObject* parent = nullptr);

public slots:
    void redraw(int width, int height, double scale, double offsetX, double offsetY);

signals:
    void redrawFinished(QObject* imageContainer);
    void setProgress(double value);
};

#endif // CORE_H
