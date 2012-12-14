#include "bottom_terrain.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "CS123Algebra.h"

bottom_terrain::bottom_terrain()
{
    // set up base terrain

    // Determines how much each vertex gets perturbed. The larger the value, the less pertubration will occur per recursive value
    m_decay = 2;

    // The number of levels of recursion your terrain uses (can be considered the level of detail of your terrain)
    m_depth = 8;

    // The roughness of your terrain. Higher roughnesses entail taller mountains and deeper valleys. Lower values entail small hills and shallow valleys
    m_roughness = 5;

    // Seed a random number, the terrain will change based on what number is seeded
    srand(0);
    // Imagining your terrain as a square grid of points, gridLength is the number of elements on one side, or the length of a side of the grid
    m_gridLength = (1 << m_depth)+1;
    printf("gridlength = %i\n", m_gridLength);

    // Knowing the number of elements on a side gives us the total number of elements in the grid
    int terrain_array_size = m_gridLength * m_gridLength;

    // Represents all of our vertices. Picture this as a grid
    m_terrain = new WorldPoint[terrain_array_size];

    // Represents all the normals, one per each vertex
    m_normalMap = new Vector3[terrain_array_size];
}

bottom_terrain::~bottom_terrain() {


    delete[] m_terrain;
    delete[] m_normalMap;
}
void bottom_terrain::subdivideSquare(GridIndex topleft, GridIndex botright, GLint curDepth)
{
    //printf("recurrs %i times\n", curDepth);

    if(curDepth<=0) {
        //printf("reached end of recursion\n");
        return;
    }
    // TL--TM--TR    +---> x
    // |   |   |     |
    // ML--MM--MR    V
    // |   |   |     y
    // BL--BM--BR

    // corner coordinates (in the grid space [x,y])
    GridIndex TL = GridIndex(topleft.x, topleft.y);
    GridIndex TR = GridIndex(botright.x, topleft.y);
    GridIndex BL = GridIndex(topleft.x, botright.y);
    GridIndex BR = GridIndex(botright.x, botright.y);

    // corner vertices on the terrain (in the grid space [x,y,z])
    WorldPoint &vTL = m_terrain[getIndex(TL)];
    WorldPoint &vTR = m_terrain[getIndex(TR)];
    WorldPoint &vBL = m_terrain[getIndex(BL)];
    WorldPoint &vBR = m_terrain[getIndex(BR)];

    //
    // @TODO: [Lab 5] Above, we demonstrate how to find
    //                  a) the coordinates in the grid of the corner vertices (in grid space)
    //                  b) the actual (X,Y,Z) points in the terrain map
    //
    //         Now, you just need to:
    //                    compute the midpoints in grid space
    //                    average the heights of the neighboring vertices
    //                    write the new points to the terrain map.
    //                    Remember to perturb the new center vertex by a random amount.
    //         Scale this perturbation by curDepth so that the perturbation factor is less
    //         as you recur deeper.
    //
    //         Remember that [X,Y] grid points map to [X,Z] in world space -- the Y
    //         coordinate in world space will be used for the height.
    //
    //         Once you compute these remaining 5 points, you will need to recur on the
    //         four sub-squares you've just created. To do this, you'll call subdivideSquare
    //         recursively, decrementing curDepth by one.

    int mid_x_index = (TL.x + TR.x)/2.00;
    int mid_y_index = (TL.y + BL.y)/2.00;

    Vector3 l_mid = (vBL+vTL)/2;
    Vector3 r_mid = (vTR+vBR)/2;
    Vector3 t_mid = (vTL+vTR)/2;
    Vector3 b_mid = (vBL+vBR)/2;

    Vector3 center = (vBL+vTL+vBR+vTR)/4;
    center.y+=getPerturb(curDepth);

    GridIndex lm = (TL+BL)/2;
    GridIndex rm = (TR+BR)/2;
    GridIndex tm = (TR + TL)/2;
    GridIndex bm = (BL + BR)/2;
    GridIndex c(mid_x_index, mid_y_index);

    m_terrain[getIndex(lm)] = l_mid;
    m_terrain[getIndex(rm)] = r_mid;
    m_terrain[getIndex(tm)] = t_mid;
    m_terrain[getIndex(bm)] = b_mid;
    m_terrain[getIndex(c)] = center;

    int newDepth = curDepth-1;

    subdivideSquare(TL, c, newDepth);
    subdivideSquare(lm, bm, newDepth);
    subdivideSquare(tm, rm, newDepth);
    subdivideSquare(c, BR, newDepth);

}

void bottom_terrain::computeNormals()
{
    // For each vertex in the 2D grid...
    for (int row = 0; row < m_gridLength; row++)
    {
        for (int column = 0; column < m_gridLength; column++)
        {
            const GridIndex gridPosition(row, column);                // 2D coordinate of the vertex on the terrain grid
            const int terrainIndex = getIndex(gridPosition);          // Index into the 1D position and normal arrays
            const WorldPoint& vertexPosition  = m_terrain[terrainIndex]; // Position of the vertex

            // Get the neighbors of the vertex at (a,b)
            const QList<WorldPoint*>& neighbors = getSurroundingVertices(gridPosition);
            int numNeighbors = neighbors.size();

            // @TODO: [Lab 5] Compute a list of vectors from vertexPosition to each neighbor in neighbors
            Vector3 *offsets = new Vector3[numNeighbors];
            for (int i = 0; i < numNeighbors; ++i)
                offsets[i] = Vector3(-vertexPosition.x + neighbors.at(i)->x, -vertexPosition.y + neighbors.at(i)->y, -vertexPosition.z + neighbors.at(i)->z);

            // @TODO: [Lab 5] Compute cross products for each neighbor
            Vector3 *normals = new Vector3[numNeighbors];
            for (int i = 0; i < numNeighbors; ++i)
                normals[i] = offsets[i].cross(offsets[((i+1)%numNeighbors)]); // TODO

            // Average the normals and store the final value in the normal map
            Vector3 sum = Vector3(0.0,0.0,0.0);
            for (int i = 0; i < numNeighbors; ++i)
                sum += normals[i];
            m_normalMap[terrainIndex] = sum.getNormalized();

            delete[] offsets;
            delete[] normals;
        }
    }
}
/**
 * Computes the amount to perturb the height of the vertex currently being processed.
 * Feel free to modify this.
 *
 * @param depth The current recursion depth
 */
double bottom_terrain::getPerturb(int cur_depth)
{
    double toret = (m_roughness * pow((double)cur_depth / m_depth, m_decay) * ((rand() %    100) / 100.0));
    return /*(toret<0)? 0.5+toret :*/ toret;
}

/****************************************************************************************************************/
/**********************************DO NOT MODIFY ANY OF THE FOLLOWING CODE***************************************/
/****************************************************************************************************************/
/**
 * Sets default values for the four corners of the terrain grid and calls subdivideSquare()
 * to begin the terrain generation process. You do not need to modify this function.
 */
void bottom_terrain::populateTerrain()
{
    WorldPoint tl(-10, 0, -10);
    WorldPoint tr(10, 0, -10);
    WorldPoint bl(-10, 0, 10);
    WorldPoint br(10, 0, 10);
    GridIndex tlg(0,0);
    GridIndex trg(0,m_gridLength-1);
    GridIndex blg(m_gridLength-1, 0);
    GridIndex brg(m_gridLength-1, m_gridLength-1);
    m_terrain[getIndex(tlg)] = tl;
    m_terrain[getIndex(trg)] = tr;
    m_terrain[getIndex(blg)] = bl;
    m_terrain[getIndex(brg)] = br;
    subdivideSquare(tlg, brg, m_depth);
    for(int i=0;i<m_gridLength;i++) {
        for(int j=0;j<m_gridLength;j++) {
            int index = getIndex(j,i);
           // printf("[%f, %f, %f]", m_terrain[index].x, m_terrain[index].y, m_terrain[index].z);
        }
      //  printf("\n");
    }
}

/**
  * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
  * Can be used to index into m_terrain or m_normalMap.
  * Returns -1 if the grid coordinate entered is not valid.
  */
inline int bottom_terrain::getIndex(const GridIndex &c)
{
    return getIndex(c.x, c.y);
}

/**
  * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
  * Can be used to index into m_terrain or m_normalMap.
  * Returns -1 if the grid coordinate entered is not valid.
  */
inline int bottom_terrain::getIndex(int row, int col)
{
    if (row < 0 || row >= m_gridLength || col < 0 || col >= m_gridLength)
        return -1;

    return row * m_gridLength + col;
}

// All of the following methods are to be used in finding the normals

/**
 * Retrieves the position of each neighbor of the given grid coordinate (i.e. all grid
 * coordinates that can be found one unit horizontally, vertically, or diagonally from
 * the specified grid coordinate).
 *
 * @param coordinate The grid coordinate whose neighbors are to be retrieved
 */
QList<WorldPoint*> bottom_terrain::getSurroundingVertices(const GridIndex &coordinate)
{
    GridIndex coords[8];
    coords[0] = GridIndex(coordinate.x,     coordinate.y - 1);
    coords[1] = GridIndex(coordinate.x + 1, coordinate.y - 1);
    coords[2] = GridIndex(coordinate.x + 1, coordinate.y);
    coords[3] = GridIndex(coordinate.x + 1, coordinate.y + 1);
    coords[4] = GridIndex(coordinate.x,     coordinate.y + 1);
    coords[5] = GridIndex(coordinate.x - 1, coordinate.y + 1);
    coords[6] = GridIndex(coordinate.x - 1, coordinate.y);
    coords[7] = GridIndex(coordinate.x - 1, coordinate.y - 1);

    int index;
    QList<WorldPoint*> vecs;

    for (int i = 0; i < 8; i++)
    {
        index = getIndex(coords[i]);
        if (index != -1)
            vecs.push_back(& m_terrain[index]);
    }

    return vecs;
}


