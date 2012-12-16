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
    m_camera.eye.x = 45.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = 2.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 0.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 0.0f, m_camera.up.z = 1.0f;
    m_camera.fovy = 45.0f, m_camera.near = 1.0f, m_camera.far = 1000.0f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = -0.2f;

    m_bterrain = new bottom_terrain();
    m_bterrain->populateTerrain();
    m_bterrain->computeNormals();
    //setAutoBufferSwap(false);
    //setFocusPolicy(Qt::StrongFocus);

//    m_branches = new std::deque<Branch >();
//    m_factory = NULL;

}

View::~View()
{
    gluDeleteQuadric(m_quadric);
//    m_branches->clear();
//    delete m_branches;
//    if (m_factory) delete m_factory;
}
void View::initializeShaders() {
    const QGLContext *ctx = context();
    m_shaderPrograms["blur"] = ResourceLoader::newShaderProgram(ctx, "/home/aherlihy/course/cs123/123Final/t_blur.vert", "/home/aherlihy/course/cs123/123Final/t_blur.frag");



    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
    m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width(), height(), QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["fbo_0"]->format().setSamples(16);
    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width(), height(), QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);

//    m_framebufferObjects["fbo_2"] = new QGLFramebufferObject(width(), height(), QGLFramebufferObject::NoAttachment,
//                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
}

void View::initializeGL()
{
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
    glEnable(GL_TEXTURE_2D);

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

    glEnable(GL_LIGHTING);
    GLfloat global_ambient[] = { 0.5f,  0.5f,  0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 1000.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);


    // Set up fog

    GLfloat fogColor[4]= {1.0,1.0,1.0, 1.0f};
    glEnable(GL_FOG);
    fogMode = GL_EXP;
    glFogi(GL_FOG_MODE, fogMode);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.015);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 25.0);


    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(1.0,1.0,1.0, 1.0f);

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
    m_emitter = new ParticleEmitter(loadTexture(":textures/particle1.bmp", id[2]));
}

void View::paintTrunk() {


//    glEnable(GL_TEXTURE_2D);

//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);            // Set The Texture Generation Mode For S To Sphere Mapping ( NEW )
//    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);            // Set The Texture Generation Mode For T To Sphere Mapping ( NEW )
//    glEnable(GL_TEXTURE_GEN_S);                     // Enable Texture Coord Generation For S ( NEW )
//    glEnable(GL_TEXTURE_GEN_T);                     // Enable Texture Coord Generation For T ( NEW )
//    glBindTexture(GL_TEXTURE_2D, barktexture);

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

    float old_r = 50;
    float new_r = 50;
    for(float h = 1;h<100;h++) {
        glColor4f(1.0f,0.0,0.0,1.0f);
        new_r = 100.0f/h;
        gluCylinder(m_quadric, old_r, new_r, 0.25f, 100, 100);
        glTranslatef(0.0,0.0,0.25);
        old_r = new_r;
    }
    gluCylinder(m_quadric, new_r, new_r, 1150.0f, 100, 100);


    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//    glDisable(GL_TEXTURE_GEN_S);
//    glDisable(GL_TEXTURE_GEN_T);

}


void View::paintMountains() {
    glPushMatrix();
    glScalef(20.0,20.0,20.0);
    glColor4f(0.0,0.0,0.0,1.0);
    for(int i=0;i<m_bterrain->m_gridLength-1;i++) {
            glBegin(GL_TRIANGLE_STRIP);

            for (int j=0;j<m_bterrain->m_gridLength;j++) {

                    int index = m_bterrain->getIndex(GridIndex(i,j));
                    int index2 = m_bterrain->getIndex(GridIndex(i+1,j));

                    Vector3 ter1 = m_bterrain->m_terrain[index];
                    Vector3 ter2 = m_bterrain->m_terrain[index2];
                    Vector3 nor1 = m_bterrain->m_normalMap[index];
                    Vector3 nor2 = m_bterrain->m_normalMap[index2];

                    float temp = ter1.z;
                    ter1.z = ter1.y;
                    ter1.y = temp;
                    temp = ter2.z;
                    ter2.z=ter2.y;
                    ter2.y = temp;


                    temp = nor1.z; nor1.y=nor1.z; nor1.z=temp;
                    temp = nor2.z; nor2.y=nor2.z; nor2.z=temp;


                    if(sqrt((ter1.x)*(ter1.x) + (ter1.y)*(ter1.y))<2.5f) {
                        ter1.z=-1;
                    }
                    if(sqrt(ter2.x*ter2.x + ter2.y*ter2.y)<2.5f) {
                        ter2.z=-1;
                    }

        //            glTexCoord2f(
        //                    ((float)i/(float)length),
        //                    1.0f-((float)j/(float)length));
                    glNormal3dv(nor2.data);
                    glVertex3dv(ter2.data);



        //            glTexCoord2f(
        //                    ((float)i+1.0f)/(float)length,
        //                    1.0f-((float)j/(float)length));
                    glNormal3dv(nor1.data);
                    glVertex3dv(ter1.data);
            }
        glEnd();

    }
    glPopMatrix();
}

void View::paintEverything() {
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    paintTrunk();
    paintMountains();
    m_emitter->updateParticles();       //Move the particles
    m_emitter->drawParticles(m_quadric);         //Draw the particles

    glDisable(GL_DEPTH_TEST);

}

void View::createBlurKernel(int radius, int width, int height,
                            GLfloat* kernel, GLfloat* offsets) {
    int size = radius * 2 + 1;
    float sigma = radius / 3.0f;
    float twoSigmaSigma = 2.0f * sigma * sigma;
    float rootSigma = sqrt(twoSigmaSigma * M_PI);
    float total = 0.0f;
    float xOff = 1.0f / width, yOff = 1.0f / height;
    int offsetIndex = 0;
    for (int y = -radius, idx = 0; y <= radius; ++y){
        for (int x = -radius; x <= radius; ++x,++idx){
            float d = x * x + y * y;
            kernel[idx] = exp(-d / twoSigmaSigma) / rootSigma;
            total += kernel[idx];
            offsets[offsetIndex++] = x * xOff;
            offsets[offsetIndex++] = y * yOff;
        }
    }
    for (int i = 0; i < size * size; ++i){
        kernel[i] /= total;
    }
}
void View::renderTexturedQuad(int width, int height) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glColor4f(1.0,1.0,1.0,1.0);
    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(width, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(width, height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, height);
    glEnd();
}
void View::paintGL()
{

    /**
      MOVE CAMERA
    **/
    float time = m_increment++ / (float) m_fps;
    //comment out to get it to freeze when click
    //m_camera.eye.x=settings.view_rad*cos(time);
    //m_camera.eye.y=settings.view_rad*sin(time);
    //m_camera.eye.z=time;
    if(settings.camera_control==TIMER_CONTROL) {
        m_camera.center.x = 0.0f, m_camera.center.y = 0.0f;
    //uncomment to get it to freeze when clicking
        m_camera.eye.x=settings.view_rad*cos(time);
        m_camera.eye.y=settings.view_rad*sin(time);
        m_camera.eye.z=time+2;
    }
    /**
      SET UP VALUES FOR SHADER
    **/

    int radius = 2;
    int dim = radius * 2 + 1;
    GLfloat kernel[dim * dim];
    GLfloat offsets[dim * dim * 2];
    createBlurKernel(radius, this->width(), this->height(), &kernel[0], &offsets[0]);

    /**
      RENDER TO FBO 1
    **/
    // Render the normal scene to fb0
    m_framebufferObjects["fbo_0"]->bind();

    updateCamera();
    paintEverything();

    m_framebufferObjects["fbo_0"]->release();

    //(now normal scene is stored in fb0)

    // Copy the rendered scene into framebuffer 1
    m_framebufferObjects["fbo_0"]->blitFramebuffer(m_framebufferObjects["fbo_1"],
                                                   QRect(0, 0, this->width(), this->height()), m_framebufferObjects["fbo_0"],
                                                   QRect(0, 0, this->width(), this->height()), GL_COLOR_BUFFER_BIT, GL_NEAREST); //normal scene is stored in fb1
    /**
      BIND SHADER + DRAW
    **/
    const QString name1 = "offsets";
    m_shaderPrograms["blur"]->setUniformValueArray(m_shaderPrograms["blur"]->uniformLocation(name1), &offsets[0], dim*dim*2, 2);
    const QString name2 = "kernel";
    m_shaderPrograms["blur"]->setUniformValueArray(m_shaderPrograms["blur"]->uniformLocation(name2), &kernel[0], dim*dim, 1);

    m_shaderPrograms["blur"]->bind();


    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

    applyOrthogonalCamera(this->width(),this->height());

    renderTexturedQuad(this->width(),this->height());

    m_shaderPrograms["blur"]->release();//release shader

    glBindTexture(GL_TEXTURE_2D, 0);

    //NOT USING
//    m_framebufferObjects["fbo_2"]->release();
   // updateCamera();

//    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_2"]->texture());
//    renderTexturedQuad(width,height);
//    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
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
        if(to_add > -45 && to_add < 45) { //over higher bound
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
