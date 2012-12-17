#include "bottom_terrain.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "CS123Algebra.h"

bottom_terrain::bottom_terrain()
{
    // Determines how much each vertex gets perturbed. The larger the value, the less pertubration will occur per recursive value
    m_decay = 2;

    // The number of levels of recursion your terrain uses (can be considered the level of detail of your terrain)
    m_depth = 8;

    // The roughness of your terrain. Higher roughnesses entail taller mountains and deeper valleys. Lower values entail small hills and shallow valleys
    m_roughness = 6;

    // Seed a random number, your terrain will change based on what number you seed
    srand(0);


    // Imagining your terrain as a square grid of points, gridLength is the number of elements on one side, or the length of a side of the grid
    m_gridLength = (1 << m_depth)+1;

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

    int lxi = TL.x;
    int mxi = (TL.x + BR.x)/2;
    int rxi = BR.x;
    int tyi = TL.y;
    int myi = (TL.y + BR.y)/2;
    int byi = BR.y;

    GridIndex TM = GridIndex((TL.x + BR.x)/2, TL.y);
    GridIndex ML = GridIndex(TL.x, (TL.y + BL.y)/2);
    GridIndex MM = GridIndex((TL.x + TR.x)/2, (TL.y + BL.y)/2);
    GridIndex MR = GridIndex(TR.x, (TR.y + BR.y)/2);
    GridIndex BM = GridIndex((BL.x + BR.x)/2, BL.y);

    // corner vertices on the terrain (in the grid space [x,y,z])
    WorldPoint &vTL = m_terrain[getIndex(TL)];
    WorldPoint &vTR = m_terrain[getIndex(TR)];
    WorldPoint &vBL = m_terrain[getIndex(BL)];
    WorldPoint &vBR = m_terrain[getIndex(BR)];

    WorldPoint &vTM = m_terrain[getIndex(TM)];
    WorldPoint &vML = m_terrain[getIndex(ML)];
    WorldPoint &vMM = m_terrain[getIndex(MM)];
    WorldPoint &vMR = m_terrain[getIndex(MR)];
    WorldPoint &vBM = m_terrain[getIndex(BM)];

    float scale = 20.0 / (float)m_gridLength;
    vTM.x = scale * (float)TM.x - 10.0;
    vML.x = scale * (float)ML.x - 10.0;
    vMM.x = scale * (float)MM.x - 10.0;
    vMR.x = scale * (float)MR.x - 10.0;
    vBM.x = scale * (float)BM.x - 10.0;
    vTM.z = scale * (float)TM.y - 10.0;
    vML.z = scale * (float)ML.y - 10.0;
    vMM.z = scale * (float)MM.y - 10.0;
    vMR.z = scale * (float)MR.y - 10.0;
    vBM.z = scale * (float)BM.y - 10.0;

    vTM.y = (vTL.y + vTR.y)/2;
    vML.y = (vTL.y + vBL.y)/2;
    vMR.y = (vTR.y + vBR.y)/2;
    vBM.y = (vBL.y + vBR.y)/2;
    vMM.y = (vTL.y + vTR.y + vBL.y + vBR.y)/4;
    vMM.y += getPerturb(curDepth);

    if (curDepth > 0) {
        curDepth -= 1;
        subdivideSquare(TL, MM, curDepth);
        subdivideSquare(TM, MR, curDepth);
        subdivideSquare(ML, BM, curDepth);
        subdivideSquare(MM, BR, curDepth);
    }
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
                offsets[i] = *neighbors[i] - vertexPosition;

            // @TODO: [Lab 5] Compute cross products for each neighbor
            Vector3 *normals = new Vector3[numNeighbors];
            for (int i = 0; i < numNeighbors; ++i)
                normals[i] = offsets[i].cross(offsets[(i+1)%numNeighbors].data);

            // Average the normals and store the final value in the normal map
            Vector3 sum = Vector3::zero();
            for (int i = 0; i < numNeighbors; ++i)
                sum += normals[i];
            m_normalMap[terrainIndex] = sum.getNormalized();

            //m_normalMap[terrainIndex] = Vector3(0,1,0);
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
    return m_roughness
           * pow((double)cur_depth / m_depth, m_decay)
           * ((rand() % 200-100) / 100.0);
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

    for (int i=0; i<m_gridLength; i++) {
        for (int j=0; j<m_gridLength; j++) {

        }
    }


//    float halflen = (float)m_gridLength/2.0;
//    for(int i=0;i<m_gridLength;i++) {
//        for(int j=0;j<m_gridLength;j++) {
//            float di = ((float)(i - halflen))/halflen;
//            float dj = ((float)(j - halflen))/halflen;
//            float rad = di*di + dj*dj;
//            if (rad < 0.1) {
//                m_terrain[i*m_gridLength + j].y = 0.0;
//            } else if (rad < 0.3) {
//                m_terrain[i*m_gridLength + j].y *= (rad - 0.1)/0.3;
//            }
//            //int index = getIndex(j,i);
//           // printf("[%f, %f, %f]", m_terrain[index].x, m_terrain[index].y, m_terrain[index].z);
//        }
//      //  printf("\n");
//    }
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


