#include <gtest/gtest.h>
#include "Mesh.h"

std::string oName = "models/zylinder_for_tests.obj";
Mesh m(oName);

TEST(Mesh, loadObj)
{
    bool loaded = m.m_mesh.isLoaded();
    EXPECT_TRUE(loaded);

    size_t numOfVertices = m.m_mesh.getNumVerts();

    EXPECT_EQ(18, numOfVertices);

}

TEST(Mesh, getClosestVertex)
{
    std::pair<ngl::Vec3, int> resVec1 = m.getClosestVertex(ngl::Vec3(-1.0f, 0.0f, -1.0f));

    EXPECT_FLOAT_EQ(-0.707107f, resVec1.first.m_x);
    EXPECT_FLOAT_EQ(0.0f, resVec1.first.m_y);
    EXPECT_FLOAT_EQ(-0.707107f, resVec1.first.m_z);
    EXPECT_FLOAT_EQ(2, resVec1.second);

    std::pair<ngl::Vec3, int> resVec2 = m.getClosestVertex(ngl::Vec3(2.0f, 2.0f, 0.0f));

    EXPECT_EQ(1.0f, resVec2.first.m_x);
    EXPECT_EQ(2.0f, resVec2.first.m_y);
    EXPECT_EQ(0.0f, resVec2.first.m_z);
    EXPECT_EQ(15, resVec2.second);
}

TEST(Mesh, getFacesWithVertex)
{
    std::pair<ngl::Vec3, int> resVec1 = m.getClosestVertex(ngl::Vec3(1.0f, 2.0f, 0.0f));
    EXPECT_EQ(15, resVec1.second);

    std::vector<ngl::Face> faceList = m.getFacesWithVertex(resVec1.second);

    EXPECT_EQ(5, faceList.size());

    //face 0
    //vertex1
    EXPECT_EQ(14, faceList.at(0).m_vert.at(0));
    EXPECT_EQ(13, faceList.at(0).m_uv.at(0));
    EXPECT_EQ(39, faceList.at(0).m_norm.at(0));

    //vertex2
    EXPECT_EQ(7, faceList.at(0).m_vert.at(1));
    EXPECT_EQ(14, faceList.at(0).m_uv.at(1));
    EXPECT_EQ(40, faceList.at(0).m_norm.at(1));
}

TEST(Mesh, getNormalVertices)
{
    std::pair<ngl::Vec3, int> _vertex = m.getClosestVertex(ngl::Vec3(1.0f, 2.0f, 0.0f));
    std::vector<ngl::Face> faceList = m.getFacesWithVertex(_vertex.second);

    std::vector<ngl::Vec3> vertexNormals = m.getNormalVertices(_vertex.second);

    EXPECT_EQ(3, vertexNormals.size());

    //first vertex
    EXPECT_FLOAT_EQ(0.923880, vertexNormals.at(0).m_x);
    EXPECT_FLOAT_EQ(0.0, vertexNormals.at(0).m_y);
    EXPECT_FLOAT_EQ(0.382683, vertexNormals.at(0).m_z);
}

TEST(Mesh, getNormalVector)
{
    ngl::Vec3 resVec = m.getNormalVector(ngl::Vec3(-1.0f, 2.0f, 0.0f));

    EXPECT_FLOAT_EQ(-0.87946534, resVec.m_x);
    EXPECT_FLOAT_EQ(0.4759629, resVec.m_y);
    EXPECT_FLOAT_EQ(0, resVec.m_z);
}

TEST(Mesh, distanceVector)
{
    ngl::Vec3 vec1 = {2,1,0};
    ngl::Vec3 vec2 = {0,1,2};

    float distance = m.getDistance(vec1, vec2);

    EXPECT_FLOAT_EQ(2.828427,distance);

}

TEST(Mesh, facesToCheck)
{
    ngl::Vec3 resVec = {-0.2f, 1.5f, 1.0f};
    ngl::Vec3 closestVertex = m.getClosestVertex(resVec).first;
    ngl::Vec3 distVec = resVec - closestVertex;

    std::vector<ngl::Face> _faces = m.boxIntersection(resVec,  distVec.length());
    EXPECT_EQ(5, _faces.size());

}

TEST(Mesh, isInsideBox)
{
    bool inSideBox = m.isInsideBox({1,2,3}, {0,0,0}, 4);
    EXPECT_TRUE(inSideBox);
}

TEST(Mesh, getFaceNormal)
{
    ngl::Vec3 faceNormal = m.getFaceNormal({0,2,0}, {-1,2,0}, {0,2,-1} );

    EXPECT_FLOAT_EQ(0, faceNormal.m_x);
    EXPECT_FLOAT_EQ(-1, faceNormal.m_y);
    EXPECT_FLOAT_EQ(0,faceNormal.m_z);
}

TEST(Mesh, isOnTriangle)
{
    bool insideTriangle = m.isOnTriangle({0,2,0}, {-1,2,0}, {0,2,-1}, {-0.33,2, -0.332} );

    EXPECT_TRUE(insideTriangle);
}

TEST(Mesh, closestPointOnPlane)
{
    ngl::Vec3 closestPoint = m.getClosestPointOnAPlane({0,2,0}, {-0.707,2.239,-0.707}, {0,1,0} );


    EXPECT_FLOAT_EQ(-0.707, closestPoint.m_x);
    EXPECT_FLOAT_EQ(2, closestPoint.m_y);
    EXPECT_FLOAT_EQ(-0.707, closestPoint.m_z);
}