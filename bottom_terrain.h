#ifndef BOTTOM_TERRAIN_H
#define BOTTOM_TERRAIN_H
#include "CS123Algebra.h"
#include "camera.h"
#include <QList>
#include "GL/gl.h"

typedef Vector2 GridIndex;
typedef Vector3 WorldPoint;
class bottom_terrain
{
public:
    bottom_terrain();
    ~bottom_terrain();
    /**
      * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
      * Can be used to index into m_terrain or m_normalMap.
      * Returns -1 if the grid coordinate entered is not valid.
      */
    inline int getIndex(const GridIndex &c);
    /**
      * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
      * Can be used to index into m_terrain or m_normalMap.
      * Returns -1 if the grid coordinate entered is not valid.
      */
    inline int getIndex(int row, int column);

    /**
     * Computes the amount to perturb the height of the vertex currently being processed.
     * Feel free to modify this.
     *
     * @param depth The current recursion depth
     */
    double getPerturb(int cur_depth);
    /**
     * Retrieves the position of each neighbor of the given grid coordinate (i.e. all grid
     * coordinates that can be found one unit horizontally, vertically, or diagonally from
     * the specified grid coordinate).
     *
     * @param coordinate The grid coordinate whose neighbors are to be retrieved
     */
    QList<WorldPoint*> getSurroundingVertices(const GridIndex &coordinate);

    // Methods you will have to fill in

    /**
     * You need to fill this in.
     *
     * Subdivides a square by finding the vertices at its corners, the midpoints of each side, and
     * the center (as the algorithm describes). Then recurs on each of the four sub-squares created.
     *
     * @param topLeft The grid coordinate of the top-left corner of the square to subdivide
     * @param bottomRight The grid coordinate of the bottom-right corner of the square to subdivide
     * @param depth The current recursion depth, decreasing as this function recurses deeper. The
     *              function should stop recurring when this value reaches zero.
     */
    void subdivideSquare(GridIndex tlg, GridIndex brg, GLint curDepth);
    /**
     * You need to fill this in.
     *
     * Computes the normal vector of each terrain vertex and stores it in the corresponding vertex.
     */
    void computeNormals();
    /**
     * You need to fill this in.
     *
     * Renders the terrain to the canvas. This is where you should make all of your OpenGL calls.
     */
    void populateTerrain();
    /**
     * Draws a line at each vertex showing the direction of that vertex's normal. You may find
     * this to be a useful tool if you're having trouble getting the lighting to look right.
     * By default, this function is called in paintGL(), but only renders anything if
     * m_renderNormals is true. You do not need to modify this function.
     */
    void drawNormals();

    //Member variables

    /** Contains the position of the vertex at each grid point in the terrain */
    WorldPoint *m_terrain;
    /** Contains the normal vector of the vertex at each grid point in the terrain */
    Vector3 *m_normalMap;
    /** The number of grid points on one side of the square terrain grid. */
    GLint m_gridLength;
    /** The number of recursion levels to use to generate terrain. Can be used as a level-of-detail parameter. */
    GLint m_depth;
    /** Controls the height of the terrain. Large values generate higher peaks and lower valleys. */
    GLfloat m_roughness;
    /** Controls how much heights can vary per recursion depth level. Higher values generate smoother terrain. */
    GLint m_decay;
    /** Whether or not drawNormals() should do anything when called. */
    bool m_renderNormals;

};

#endif // BOTTOM_TERRAIN_H
