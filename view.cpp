#include "view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include "settings.h"
View::View(QWidget *parent) : QGLWidget(parent), m_timer(this), m_fps(60.0f), m_increment(0)
{
    showMaximized();
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

    m_branches = new std::deque<Branch >();
    m_factory = NULL;
}

View::~View()
{
    gluDeleteQuadric(m_quadric);
    m_branches->clear();
    delete m_branches;
    if (m_factory) delete m_factory;
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
    m_factory = new BranchFactory(m_quadric);

    // init textures
    gluQuadricDrawStyle(m_quadric, GLU_FILL);

    gluQuadricTexture(m_quadric, GL_TRUE);

    // Load textures
    // Generate a new OpenGL texture ID to put our image into
    GLuint id[3];
    glGenTextures(3, id);
    if(loadTexture("/home/aherlihy/course/cs123/123Final/data/bark1.jpg", id[0])==-1) {
        printf("BARK TEXTURE DOESN'T EXIST\n");
    }
    if(loadTexture("/home/aherlihy/course/cs123/123Final/data/desert2.jpg&size=1024", id[1])==-1) {
        printf("DESERT TEXTURE DOESNT EXIST\n");
    }
    deserttexture = id[1];
    barktexture = id[0];

    // Bind the ambient and diffuse color of each vertex to the current glColor() value
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Start with flat shading
    glShadeModel(GL_SMOOTH);

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Set up global (ambient) lighting

    GLfloat global_ambient[] = { 0.5f,  0.5f,  0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
//    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
//    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
//    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//    GLfloat position[] = { 0.0f, 0.0f, 3.0f, 1.0f };
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
//    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);


    // Set up fog

    GLfloat fogColor[4]= {(238.0f/255.0f),(229.0f/255.0f),(202.0f/255.0f), 1.0f};
    glEnable(GL_FOG);
    fogMode = GL_EXP;
    glFogi(GL_FOG_MODE, fogMode);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.015);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 25.0);


    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor((238.0f/255.0f),(229.0f/255.0f),(202.0f/255.0f), 1.0f);

    // Load the initial settings
    updateSettings();
    updateCamera();


    // TODO: Put any other initialization here
//    glEnable(GL_LIGHT0);

    // End Initialize gl


    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    for (int i=0; i<10; i++) {
        m_branches->push_back(*m_factory->generateBranch(5));
    }
    m_branch = m_factory->generateBranch(5);
}

void View::paintTrunk() {


    glEnable(GL_TEXTURE_2D);

//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);            // Set The Texture Generation Mode For S To Sphere Mapping ( NEW )
//    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);            // Set The Texture Generation Mode For T To Sphere Mapping ( NEW )
//    glEnable(GL_TEXTURE_GEN_S);                     // Enable Texture Coord Generation For S ( NEW )
//    glEnable(GL_TEXTURE_GEN_T);                     // Enable Texture Coord Generation For T ( NEW )
    glBindTexture(GL_TEXTURE_2D, barktexture);

    float time = m_increment++ / (float) m_fps;
    glPushMatrix();

    glScalef(10, 10, 10);
    m_branch->drawBranch();

    //gluCylinder(m_quadric, 1.0f, 1.0f, 1200.0f, 10, 1);
//    for (int i=0; i<m_branches->size(); i++) {
//        glTranslatef(0, 0, 10.0);
//        glRotatef(time, 0.0, 0.0, 1.0);
//        glPushMatrix();
//        glRotatef(60.0, 0.0, 1.0, 0.0);
//        m_branches->at(i).drawBranch();
//        glPopMatrix();
//    }

//    m_camera.eye.x=settings.view_rad*cos(time);
//    m_camera.eye.y=settings.view_rad*sin(time);
//    m_camera.eye.z=time;//if you want to look down upon the origin can set to increment with time.

    if(settings.camera_control==TIMER_CONTROL) {

        //if you want to look purpendicular to the trunk, you need to reset the look vector
        m_camera.center.x = 0.0f, m_camera.center.y = 0.0f;//, m_camera.center.z = time;

        m_camera.eye.x=settings.view_rad*cos(time);
        m_camera.eye.y=settings.view_rad*sin(time);
        m_camera.eye.z=time;//if you want to look down upon the origin can set to increment with time.
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
//    glDisable(GL_TEXTURE_GEN_S);
//    glDisable(GL_TEXTURE_GEN_T);

}
void View::paintSun() {
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();

    glColor3f(1.0f,1.0f,1.0f);
    glTranslatef(50, 50,200);

    gluSphere(m_quadric, 20, 20,20);

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

}

void View::paintBase() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, deserttexture);

    glPushMatrix();

    float length = m_bterrain->m_gridLength;
    float num_t = 1000/length;
    printf("length = %f\n", length);
    printf("num_t=%f\n", num_t);

//    for(int r=0;r<num_t;r++) {
//        for(int q = 0;q<num_t;q++) {
            float r = 0.0; float q=0.0;
            float x_offset = 0 + r*length;
            float y_offset = 0 + q*length;

//            printf("offset=(%f,%f)\n", x_offset, y_offset);

            for(int i=0;i<m_bterrain->m_gridLength-1;i++) {
                glBegin(GL_TRIANGLE_STRIP);
                for (int j=0;j<m_bterrain->m_gridLength;j++) {
                    int index = m_bterrain->getIndex(GridIndex(i,j));
                    int index2 = m_bterrain->getIndex(GridIndex(i+1,j));

                    Vector3 ter1 = m_bterrain->m_terrain[index];
                    Vector3 ter2 = m_bterrain->m_terrain[index2];
                    Vector3 nor1 = m_bterrain->m_normalMap[index];
                    Vector3 nor2 = m_bterrain->m_normalMap[index2];

                    float temp = ter1.z+x_offset;ter1.z = ter1.y; ter1.y = temp-1+y_offset;
                    temp = ter2.z+x_offset;ter2.z=ter2.y; ter2.y = temp-1+y_offset;


                    temp = nor1.z; nor1.y=nor1.z; nor1.z=temp;
                    temp = nor2.z; nor2.y=nor2.z; nor2.z=temp;

                    glTexCoord2f(
                            ((float)i/(float)length),
                            1.0f-((float)j/(float)length));
                    glNormal3dv(nor2.data);
                    glVertex3dv(ter2.data);



                    glTexCoord2f(
                            ((float)i+1.0f)/(float)length,
                            1.0f-((float)j/(float)length));
                    glNormal3dv(nor1.data);
                    glVertex3dv(ter1.data);

                }
                glEnd();
//            }
//        }
    }
    glBegin(GL_QUADS);


    for(int x = -490; x<490; x+=20) {
        for(int y = -490; y<490; y+=20) {


            glVertex3f(x+10,y+10,0);
            glTexCoord2f(1,1);


            glVertex3f(x-10, y+10,0);
            glTexCoord2f(0,1);


            glVertex3f(x-10,y-10,0);
            glTexCoord2f(0,0);


            glVertex3f(x+10,y-10,0);
            glTexCoord2f(1,0);
        }

    }
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void View::paintGL()
{
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintTrunk();
    //paintBase();
    paintSun();

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
    int now = time.second();

    // TODO: Handle mouse movements here
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {

        m_camera.center.x=0;
        m_camera.center.y=0;
        m_camera.center.z=m_camera.center.z + deltaY;

//        float t = acos((m_camera.eye.x/settings.view_rad));
//        m_camera.eye.x = settings.view_rad*cos(t + deltaX/(2*M_PI));
//        m_camera.eye.y = settings.view_rad*sin(t + deltaX/(2*M_PI));
        updateCamera();

    }



}
void View::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        float to_add = settings.view_rad + (event->delta()/25);
        if(to_add < 2 && to_add > -2) {
            if(event->delta()>0) {
                settings.view_rad=2.5;
            }
            else {
                settings.view_rad=-2.5;
            }
        }
        else if(to_add > -450 && to_add < 450) {
            settings.view_rad = to_add;
        }
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

int View::loadTexture(const QString &filename, int id)
{
    printf("loading file: %s\n", filename.toStdString().c_str());
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists())
        return -1;

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);


    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return id;
}
