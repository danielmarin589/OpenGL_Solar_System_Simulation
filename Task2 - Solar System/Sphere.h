///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: RMAO (ching_cheng_hanji@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2020-05-20
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>

class Sphere
{
public:
    // ctor/dtor
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
    ~Sphere() {}

    // getters/setters
    float getRadius() const { return radius; }
    int getSectorCount() const { return sectorCount; }
    int getStackCount() const { return stackCount; }
    void set(float radius, int sectorCount, int stackCount);


    // for vertex data
    unsigned int getPositionCount() const { return (unsigned int)positions.size() / 3; }
    
    unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
  
    unsigned int getTriangleCount() const { return getIndexCount() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)positions.size() * sizeof(float); }
   
    unsigned int getTexCoordSize() const { return (unsigned int)texCoords.size() * sizeof(float); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }

    const float* getVertices() const { return positions.data(); }
 
    const float* getTexCoords() const { return texCoords.data(); }
    const unsigned int* getIndices() const { return indices.data(); }
  

    // for interleaved vertices: V/N/T
    unsigned int getInterleavedVertexCount() const { return getPositionCount(); }    // # of vertices
    unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    const float* getInterleavedVertices() const { return interleavedVertices.data(); }


protected:

private:
    // member functions
    void buildVerticesSmooth();
   
    void buildInterleavedVertices();
    void clearArrays();
    void addPosition(float x, float y, float z);

    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
  
    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
   
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
 

    // interleaved
    std::vector<float> interleavedVertices;
   

};



#endif

