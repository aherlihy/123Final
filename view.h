#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "camera.h"
#include <GL/glu.h>
#include "bottom_terrain.h"
#include "BranchFactory.h"
#include <deque>
#include "particleemitter.h"


#define TREE_RAD 7.0
#define TREE_HEIGHT 200.0
#define TREE_RATIO 4.0

typedef struct {
    Branch *branch;
    float dir;
    float slope;
    float height;
} RenderBranch;


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
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int loadTexture(const QString &filename, int id);
    void paintMountains();
    void initializeShaders();
    void paintEverything();
    void renderTexturedQuad(int width, int height);
    void applyOrthogonalCamera(float width, float height);
    void createBlurKernel(int radius, int width, int height, GLfloat* kernel, GLfloat* offsets);
    void renderBlur(int width, int height);


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
    RenderBranch *m_branches;
    float current_mid;
    ParticleEmitter* m_emitter;
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects



};

#endif // VIEW_H

