#include "particleemitter.h"

ParticleEmitter::ParticleEmitter(GLuint textureId, float3 color, float3 velocity,
                                 float3 force, float scale, float fuzziness, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_textureID(textureId), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)//?
{
    m_particles = new Particle[maxParticles];
    resetParticles();
    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }
}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void ParticleEmitter::resetParticle(unsigned i)
{
    m_particles[i].pos.x=0;
    m_particles[i].pos.y=0;
    m_particles[i].pos.z=50;

    m_particles[i].life = 10;
    m_particles[i].decay = urand(0.0025f, 0.15f);

    m_particles[i].color.r = m_color.x;
    m_particles[i].color.g = m_color.y;
    m_particles[i].color.b = m_color.z;

    m_particles[i].force.x = urand((-m_fuzziness*0.01f), (m_fuzziness*0.01f+m_force.x));
    m_particles[i].force.y = urand((-m_fuzziness*0.01f), (m_fuzziness*0.01f+m_force.y));
    m_particles[i].force.z = urand((-m_fuzziness*0.01f), (m_fuzziness*0.01f+m_force.z));
    m_particles[i].dir.x = urand(-m_fuzziness, m_fuzziness+m_velocity.x);
    m_particles[i].dir.y = urand(-m_fuzziness, m_fuzziness+m_velocity.y);
    m_particles[i].dir.z = urand(-m_fuzziness, m_fuzziness+m_velocity.z);

    //m_particles[i].dir.normalize();
}

/**
  * Resets all particles in this emitter to their initial states
  */
void ParticleEmitter::resetParticles()
{
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

/**
  * You need to fill this in.
  *
  * Performs one step of the particle simulation. Should perform all physics
  * calculations and maintain the life property of each particle.
  */
void ParticleEmitter::updateParticles()
{
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        if(!m_particles[i].active==1) {
            m_particles[i].active=1;
            resetParticle(i);

        }
        else {
            m_particles[i].pos.x+=(m_particles[i].dir.x*m_speed);
            m_particles[i].pos.y+=(m_particles[i].dir.y*m_speed);
            m_particles[i].pos.z+=(m_particles[i].dir.z*m_speed);

            m_particles[i].dir.x+=m_particles[i].force.x;
            m_particles[i].dir.y+=m_particles[i].force.y;
            m_particles[i].dir.z+=m_particles[i].force.z;

            m_particles[i].life-=m_particles[i].decay;
            if(m_particles[i].life<0) {
                m_particles[i].active=0;
            }
        }

    }
}

/**
  * You need to fill this in.
  *
  * Draws each particle as a small, texture-mapped square of side-length m_scale.
  * Each square should be in the X/Y plane at Z = the particle's position's Z-coordinate.
  */
void ParticleEmitter::drawParticles(GLUquadric *m_quadric)
{

    //need to disable depth testing
    glDepthMask(0);

    //bind texture
//   --- glBindTexture(GL_TEXTURE_2D, m_textureID);
    //blend function

    glMatrixMode(GL_MODELVIEW);

    //decrease previous


    //glLoadIdentity();
    glPushMatrix();
    glBegin(GL_QUADS);
    for(unsigned i = 0; i < m_maxParticles; ++i)
    {
        Particle p = m_particles[i];

        float length = m_scale/2;

        glColor4f(1.0,1.0,1.0, 1.0);//sqrt(p.life));

        glTranslatef(p.pos.x, p.pos.y, p.pos.z);
        gluSphere(m_quadric, 0.05, 5,5);
        glTranslatef(-p.pos.x, -p.pos.y, -p.pos.z);
//        glVertex3f(p.pos.x-length, p.pos.y+length, p.pos.z);
//////     ---   glTexCoord2f(0,1);
//        glVertex3f(p.pos.x-length, p.pos.y-length, p.pos.z);
//////     ---   glTexCoord2f(0,0);
//        glVertex3f(p.pos.x+length, p.pos.y-length, p.pos.z);
//////      ---  glTexCoord2f(1,0);
//        glVertex3f(p.pos.x+length, p.pos.y+length, p.pos.z);
//      ---  glTexCoord2f(1,1);


    }
    glEnd();
//    glAccum(GL_MULT, .8);
//    glAccum(GL_ACCUM, .2);
//    glAccum(GL_RETURN, 1.0);

    glPopMatrix();
    glDepthMask(1);
}
