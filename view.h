#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "camera.h"
#include <GL/glu.h>
#include "bottom_terrain.h"
#include "BranchFactory.h"


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
    void paintTrunk();
    void paintBase();
    void paintSun();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int loadTexture(const QString &filename, int id);

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
    int deserttexture;
    int suntexture;
    bottom_terrain* m_bterrain;

    BranchFactory *m_factory;

};

#endif // VIEW_H

