#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "camera.h"
#include <GL/glu.h>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    void updateSettings();
    void updateCamera();
private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int loadTexture(const QString &filename);

private slots:
    void tick();
private:
    GLUquadric *m_quadric;
    MyCamera m_camera;
    QTimer m_timer;
    float m_fps;
    float m_increment;
    int fogMode;
    int barktexture;

};

#endif // VIEW_H

