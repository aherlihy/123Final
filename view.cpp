#include "view.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include "settings.h"
#include "particleemitter.h"
#include "resourceloader.h"
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
    m_camera.eye.x = 45.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = 50.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 70.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 0.0f, m_camera.up.z = 1.0f;
    m_camera.fovy = 45.0f, m_camera.near = 1.0f, m_camera.far = 100000.0f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = -0.2f;

    m_bterrain = new bottom_terrain();
    m_bterrain->populateTerrain();
    m_bterrain->computeNormals();


    int gridlength = m_bterrain->m_gridLength;
    Vector3 *normalmap = m_bterrain->m_normalMap;
    WorldPoint *terrain = m_bterrain->m_terrain;
    for (int i=0; i<gridlength; i++) {
        for (int j=0; j<gridlength; j++) {
            float temp;

            temp = normalmap[i*gridlength + j].z;
            normalmap[i*gridlength + j].z = normalmap[i*gridlength + j].y;
            normalmap[i*gridlength + j].y = temp;
            normalmap[i*gridlength + j] = -normalmap[i*gridlength + j];
            normalmap[i*gridlength + j].normalize();

            temp = terrain[i*gridlength + j].z;
            terrain[i*gridlength + j].z = terrain[i*gridlength + j].y;
            terrain[i*gridlength + j].y = temp;
        }
     }




    m_branches = new RenderBranch[52];
    m_factory = new BranchFactory();
    for (int i=0; i<51; i++) {
        m_branches[i].branch = m_factory->generateBranch(6);
        m_branches[i].dir = urand(0, 360);
        m_branches[i].height = (float)i/7.0+2;
        m_branches[i].slope = urand(20, 60);
    }
    m_branches[51].branch = m_factory->generateBranch(6);
    m_branches[51].dir = urand(0, 360);
    m_branches[51].height = (float)51/7.0+2;
    m_branches[51].slope = 90;

//    m_branches = new std::deque<Branch >();
//    m_factory = NULL;

}

View::~View()
{
    gluDeleteQuadric(m_quadric);

    delete [] m_branches;
//    m_branches->clear();
//    delete m_branches;
//    if (m_factory) delete m_factory;
}
void View::initializeShaders() {
    const QGLContext *ctx = context();
    m_shaderPrograms["blur"] = ResourceLoader::newShaderProgram(ctx, "/home/aherlihy/course/cs123/123Final/fog.vert", "/home/aherlihy/course/cs123/123Final/fog.frag");
}

void View::initializeGL()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//    glLightfv(GL_LIGHT0, GL_)





    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    current_mid = 5;
    time.start();
    timer.start(1000 / m_fps);

    // Start Initialize GL

    // Get a new quadrics object
    m_quadric = gluNewQuadric();
//    m_factory = new BranchFactory(m_quadric);

    // init textures
    gluQuadricDrawStyle(m_quadric, GLU_FILL);

//    gluQuadricTexture(m_quadric, GL_TRUE);

//    // Load textures
//    // Generate a new OpenGL texture ID to put our image into
    GLuint id[3];
    glGenTextures(3, id);
//    glEnable(GL_TEXTURE_2D);

//    if(loadTexture("data/bark1.jpg", id[0])==-1) {
//        printf("BARK TEXTURE DOESN'T EXIST\n");
//    }
//    if(loadTexture("data/desert2.jpg&size=1024", id[1])==-1) {
//        printf("DESERT TEXTURE DOESNT EXIST\n");
//    }
//    deserttexture = id[1];
//    barktexture = id[0];

    // Bind the ambient and diffuse color of each vertex to the current glColor() value
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Start with flat shading
//    glShadeModel(GL_SMOOTH);

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
//    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Set up global (ambient) lighting

//    glEnable(GL_LIGHTING);
//    GLfloat global_ambient[] = { 0.5f,  0.5f,  0.5f, 1.0f };
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
//    GLfloat ambientLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
//    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
//    GLfloat position[] = { 1000.0f, 1000.0f, 2000.0f, 0.0f };

    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//    glLightfv(GL_LIGHT0, GL_POSITION, position);

//    glEnable(GL_LIGHT0);

//    GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
//    GLfloat position[] = {1000, 1000, 10, 0};
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//    glLightfv(GL_LIGHT0, GL_POSITION, position);



    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    //glLightfv(GL_LIGHT0, GL_POSITION, position);


    // Set up fog

//    GLfloat fogColor[4]= {0.5,0.5,0.5,.0f};
//    glEnable(GL_FOG);
//    fogMode = GL_EXP2;
//    glFogi(GL_FOG_MODE, fogMode);
//    glFogfv(GL_FOG_COLOR, fogColor);
//    glFogf(GL_FOG_DENSITY, 0.0015);
//    glHint(GL_FOG_HINT, GL_NICEST);
//    glFogf(GL_FOG_START, 1.0);
//    glFogf(GL_FOG_END, 200.0);


    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.8, 0.8, 0.8, 1.0);

    // Load the initial settings
    updateSettings();
    updateCamera();

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // End Initialize gl


    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(this->width() / 2, this->height() / 2)));
//    float dir=0.0;
//    for (int i=0; i<10; i++) {
//        m_branches->push_back(*m_factory->generateBranch(9));
//        m_branches->at(i).setPosition(Vector3(1.0, 1.0, (float)i));
//        m_branches->at(i).setDirection(dir+=30);
//    }
//    m_branch = m_factory->generateBranch(5);


    initializeShaders();
    m_emitter = new ParticleEmitter(0, float3(1.0, 1.0, 1.0), float3(0.0, 5.0, -30.0), float3(0.0, 0.0, -0.004), 0.1, 20.0, 30.0/10000.0, 5000);
//    m_emitter->force().z = -0.1;
//    m_emitter->velocity().z = -5.0;
}

void View::paintTrunk() {

    glPushMatrix();
//    glScalef(10, 10, 10);
//    m_branch->drawBranch();
//    glScalef(0.1, 0.1, 0.1);
//    float dir_counter = 0;
//    for (int i=0; i<m_branches->size(); i++) {
//        m_branches->at(i).setPosition(Vector3(0.0, 0.0, current_mid+1*(i-5)));
//        m_branches->at(i).setDirection(dir_counter);
//        m_branches->at(i).drawBranch();

//    }


    float r_cir = 2.5 * TREE_RAD;
    int tess = 20;
    float incr = r_cir/(float)tess;

    glColor4f(0.0f, 0.0, 0.0, 1.0f);
    float old_r = r_cir + TREE_RAD;
    for (float h = incr; h <= r_cir; h += incr) {
        float x = sqrt(r_cir*r_cir - (h - r_cir)*(h - r_cir));
        float new_r = r_cir - x + TREE_RAD;
        gluCylinder(m_quadric, old_r, new_r, incr, 20, 2);
        glTranslatef(0.0, 0.0, incr);
        old_r = new_r;
    }
    gluCylinder(m_quadric, TREE_RAD, TREE_RAD/3.0, TREE_HEIGHT, 20, 10);

    glScalef(20.0, 20.0, 20.0);
    //glRotatef(90, 1.0, 0.0, 0.0);
    for (int i=0; i<52; i++) {
        glPushMatrix();
        glTranslatef(0, 0, m_branches[i].height);

        glRotatef(m_branches[i].dir, 0.0, 0.0, 1.0);
        glRotatef(-m_branches[i].slope, 1.0, 0.0, 0.0);

        glRotatef(90, 1.0, 0.0, 0.0);
        m_branches[i].branch->drawBranch(m_quadric);
        glPopMatrix();
    }

    glPopMatrix();

}


void View::paintMountains() {
    glPushMatrix();
    glScalef(80.0,80.0,80.0);
    glColor4f(1.0,1.0,1.0,1.0);

    int gridlength = m_bterrain->m_gridLength;

    for (int i=0; i<gridlength-1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j=0; j<gridlength-1; j++) {

            WorldPoint p1 = (m_bterrain->m_terrain[i*gridlength + j]);
            WorldPoint p2 = (m_bterrain->m_terrain[(i+1)*gridlength + j]);

            if(sqrt(p1.x*p1.x + p1.y*p1.y)<5) {
                p1.z = 0;
            }
            if(sqrt(p2.x*p2.x + p2.y*p2.y)<5) {
                p2.z = 0;
            }

            glNormal3dv(m_bterrain->m_normalMap[i*gridlength + j].data);
            glVertex3dv(p1.data);

            glNormal3dv(m_bterrain->m_normalMap[(i+1)*gridlength + j].data);
            glVertex3dv(p2.data);

        }
        glEnd();
    }
    glPopMatrix();
}

void View::paintEverything() {
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(settings.mountains)
        paintMountains();

    m_shaderPrograms["blur"]->bind();

    m_shaderPrograms["blur"]->setUniformValue((const char*)"posx", (GLfloat)m_camera.eye.x);
    m_shaderPrograms["blur"]->setUniformValue((const char*)"posy", (GLfloat)m_camera.eye.y);
    m_shaderPrograms["blur"]->setUniformValue((const char*)"posz",  (GLfloat)m_camera.eye.z);


    paintTrunk();


    m_emitter->updateParticles();       //Move the particles
    glPushMatrix();
    glScalef(6,6,6);
    glTranslatef(0, 0, 50);

    m_emitter->drawParticles(m_quadric);         //Draw the particles
    glPopMatrix();

    m_shaderPrograms["blur"]->release();


}

void View::paintGL()
{

    /**
      MOVE CAMERA
    **/
    float time = m_increment++ / (float) m_fps;

    //comment out to get it to ze when click
    m_camera.eye.x=3*settings.view_rad*cos(time);
    m_camera.eye.y=3*settings.view_rad*sin(time);
    if(settings.direction==UPWARDS) {
        m_camera.eye.z=time + 50;
        if(m_camera.eye.z > 250) {
            settings.direction=DOWNWARDS;
        }
    }
    else {
        m_camera.eye.z--;
        if(m_camera.eye.z<40) {
            settings.direction=UPWARDS;
        }
    }
    if(settings.camera_control==TIMER_CONTROL) {
        m_camera.center.x = 0.0f, m_camera.center.y = 0.0f;
    //uncomment to get it to freeze when clicking
//        m_camera.eye.x=settings.view_rad*cos(time);
//        m_camera.eye.y=settings.view_rad*sin(time);
//        m_camera.eye.z=time+2;
        }
    /**
      SET UP VALUES FOR SHADER
    **/

    updateCamera();
    paintEverything();

}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    // Resize the viewport

    // Reallocate the framebuffers with the new window dimensions
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
    {
        const QString &key = m_framebufferObjects.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        m_framebufferObjects[key] = new QGLFramebufferObject(w, h, format);
    }    // Update the camera
    updateCamera();
}

void View::updateCamera()
{

    float aspectRatio = 1.0f * this->width() / this->height();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, aspectRatio, m_camera.near, m_camera.far);
    gluLookAt(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z, m_camera.center.x, m_camera.center.y, m_camera.center.z, m_camera.up.x, m_camera.up.y, m_camera.up.z);

    float pos[] = {0.0, 0.0, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, pos);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

void View::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, width, 0.f, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    int deltaX = event->x() - this->width() / 2;
    int deltaY = event->y() - this->height() / 2;
    if (!deltaX && !deltaY) return;
    QCursor::setPos(mapToGlobal(QPoint(this->width() / 2, this->height() / 2)));


    // TODO: Handle mouse movements here
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {

        m_camera.center.x=0;
        m_camera.center.y=0;
        m_camera.center.z=m_camera.center.z + deltaY;
        updateCamera();

    }



}
void View::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        float to_add = settings.view_rad + (event->delta()/25);
        if(to_add > -120 && to_add < 120) { //over higher bound
            settings.view_rad = to_add;
        }
        else {
            float max_h = (m_camera.eye.z<100.0)? 13 : 2; //determine lower bound based on height
                if(to_add < max_h && to_add > -max_h) {
                    if(event->delta()>0) {
                        settings.view_rad=max_h;
                    }
                    else {
                        settings.view_rad=-max_h;
                    }
                }
        }
    }
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
    if(event->key()==Qt::Key_M) {
        if(settings.mountains) {
            settings.mountains=false;
        }
        else {
            settings.mountains=true;
        }
    }
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
