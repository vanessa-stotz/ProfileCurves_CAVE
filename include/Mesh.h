#ifndef MESH_H_
#define MESH_H_

#include <iostream>

#include <ngl/Obj.h>
#include <ngl/Vec3.h>

#include <gtest/gtest.h>

class Mesh
{
    friend class MeshTests;
    FRIEND_TEST(Mesh, getClosestVertex);
    FRIEND_TEST(Mesh, getFacesWithVertex);
    FRIEND_TEST(Mesh, getNormalVertices);
    FRIEND_TEST(Mesh, facesToCheck);
    FRIEND_TEST(Mesh, distanceVector);
    FRIEND_TEST(Mesh, isInsideBox);
    FRIEND_TEST(Mesh, getFaceNormal);
    FRIEND_TEST(Mesh, isOnTriangle);
    FRIEND_TEST(Mesh, closestPointOnPlane);

public:
    ngl::Obj m_mesh;

    Mesh() = default;
    explicit Mesh(const std::string &_oName);

    [[nodiscard]] ngl::Vec3 getNormalVector(ngl::Vec3 _point);
    [[nodiscard]] ngl::Vec3 getClosestPoint(ngl::Vec3 _point, const std::vector<ngl::Vec3> &_pointList);


private:

    std::vector<ngl::Vec3> m_vertexList;
    std::vector<ngl::Face> m_faceList;

    ngl::Vec3 m_closestVertex;
    float m_distance;

    std::vector<ngl::Face> boxIntersection(ngl::Vec3 _centerPoint, float _halfBoxSize);
    bool isInsideBox(ngl::Vec3 _point, ngl::Vec3 _centerPoint, float _halfBoxSize);
    [[nodiscard]] std::pair<ngl::Vec3,int> getClosestVertex(ngl::Vec3 _point);
    [[nodiscard]] float getDistance(ngl::Vec3 _vec1, ngl::Vec3 _vec2);
    void pointInsideTriangle(const ngl::Face &_face, ngl::Vec3 _point);
    bool isOnTriangle(ngl::Vec3 _firstFaceVertex, ngl::Vec3 _secondFaceVertex, ngl::Vec3 _thirdFaceVertex, ngl::Vec3 _point);

    [[nodiscard]] ngl::Vec3 getClosestPointOnAPlane(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec);
    [[nodiscard]] float getParameter(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec);
    [[nodiscard]] ngl::Vec3 getFaceNormal(ngl::Vec3 _firstFaceVertex, ngl::Vec3 _secondFaceVertex, ngl::Vec3 _thirdFaceVertex);

    [[nodiscard]] std::vector<ngl::Face> getFacesWithVertex(int _vertexNumber);
    [[nodiscard]] std::vector<ngl::Vec3> getNormalVertices(int _vertexNumber);

};

#endif