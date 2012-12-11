#include "view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include "settings.h"
View::View(QWidget *parent) : QGLWidget(parent), m_timer(this), m_fps(60.0f), m_increment(0)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    setCursor(Qt::BlankCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    // Set up the camera
    m_camera.eye.x = 5.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = 0.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 0.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 0.0f, m_camera.up.z = 1.0f;
    m_camera.fovy = 45.0f, m_camera.near = 1.0f, m_camera.far = 1000.0f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = -0.2f;

    m_bterrain = new bottom_terrain();
    m_bterrain->populateTerrain();
    m_bterrain->computeNormals();
}

View::~View()
{
    gluDeleteQuadric(m_quadric);
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / m_fps);

    // Start Initialize GL

    // Get a new quadrics obect
    m_quadric = gluNewQuadric();

    // Init textures
    gluQuadricDrawStyle(m_quadric, GLU_FILL);
    gluQuadricTexture(m_quadric, GL_TRUE);
    // Load texture for trunk
    if(barktexture = loadTexture("/home/aherlihy/course/cs123/123Final/data/pink.jpg")==-1) {
        printf("PICTURE NO EXISTS\n");
    }

    // Start with flat shading
    glShadeModel(GL_SMOOTH);

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
//    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    // Set up fog

    GLfloat fogColor[4]= {1.0f,1.0f,1.0f, 1.0f};
    glEnable(GL_FOG);
    fogMode = GL_EXP;
    glFogi(GL_FOG_MODE, fogMode);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.015);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 25.0);


    // Set the screen color when the color buffer is cleared (in RGBA format)
//    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClearColor(1.0f,1.0f,1.0f, 1.0f);



    // Load the initial settings
    updateSettings();
    updateCamera();


    // TODO: Put any other initialization here
    glEnable(GL_LIGHT0);

    // End Initialize gl
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void View::paintTrunk() {
    float time = m_increment++ / (float) m_fps;
    glPushMatrix();

    glColor3f(1.0f,0.0f,0.0f);
    gluCylinder(m_quadric, 1.0f, 1.0f, 1200.0f, 10, 1);

    if(settings.camera_control==TIMER_CONTROL) {
        m_camera.eye.x=15*cos(time);
        m_camera.eye.y=15*sin(time);
        m_camera.eye.z=time;//if you want to look down upon the origin can set to increment with time.

        //if you want to look purpendicular to the trunk, you need to reset the look vector
        m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = time;
    }

    glPopMatrix();
}
void View::paintBase() {

//    if(loadTexture("/home/aherlihy/course/cs123/123Final/data/pink.jpg")==-1) {
//        printf("PICTURE NO EXISTS\n");
//    }
    glPushMatrix();

    for(int i=0;i<m_bterrain->m_gridLength-1;i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j=0;j<m_bterrain->m_gridLength;j++) {
            int index = m_bterrain->getIndex(GridIndex(i,j));
            int index2 = m_bterrain->getIndex(GridIndex(i+1,j));

            Vector3 ter1 = m_bterrain->m_terrain[index];
            Vector3 ter2 = m_bterrain->m_terrain[index2];
            Vector3 nor1 = m_bterrain->m_normalMap[index];
            Vector3 nor2 = m_bterrain->m_normalMap[index2];

            float temp = ter1.z;ter1.z = ter1.y; ter1.y = temp-1;
            temp = ter2.z;ter2.z=ter2.y; ter2.y = temp-1;
            temp = nor1.z; nor1.y=nor1.z; nor1.z=temp;
            temp = nor2.z; nor2.y=nor2.z; nor2.z=temp;

            glTexCoord2f(((float)i+1)/m_bterrain->m_gridLength, 1.0f-((float)j/m_bterrain->m_gridLength));
            glNormal3dv(nor2.data);
            glVertex3dv(ter2.data);

            glTexCoord2f((float)i/m_bterrain->m_gridLength, 1.0f-((float)j/m_bterrain->m_gridLength));
            glNormal3dv(nor1.data);
            glVertex3dv(ter1.data);
        }
        glEnd();
    }

    glBegin(GL_QUADS);
    glVertex3f(-500,500,-1);
    glVertex3f(-500,-500,-1);

    glVertex3f(500,-500,-1);
    glVertex3f(500,500,-1);
    glEnd();

        glPopMatrix();

}

void View::paintGL()
{
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintTrunk();
    paintBase();


    updateCamera();

    glFlush();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    // Update the camera
    updateCamera();
}

void View::updateCamera()
{
    float w = width();
    float h = height();
    float aspectRatio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, aspectRatio, m_camera.near, m_camera.far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z, m_camera.center.x, m_camera.center.y, m_camera.center.z, m_camera.up.x, m_camera.up.y, m_camera.up.z);

}

void View::updateSettings()
{

}

void View::mousePressEvent(QMouseEvent *event) {
    settings.camera_control=USER_CONTROL;
}
void View::mouseReleaseEvent ( QMouseEvent *event ) {
    settings.camera_control=TIMER_CONTROL;
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;
    if (!deltaX && !deltaY) return;
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    // TODO: Handle mouse movements here
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {

        m_camera.center.x=0;
        m_camera.center.y=0;
        m_camera.center.z=m_camera.center.z + deltaY;

        updateCamera();

    }



}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

int View::loadTexture(const QString &filename)
{
    printf("loading file: %s\n", filename.toStdString().c_str());
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists()) {
        return -1;
    }

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);


    return id;
}
