#include <iostream>
#include <algorithm>
#include "Mesh.h"


Mesh::Mesh(const std::string &_oName)
{

     m_mesh.load(_oName);
     if(m_mesh.isLoaded())
     {
         std::cout << "loaded\n";
         m_vertexList = m_mesh.getVertexList();
         m_faceList = m_mesh.getFaceList();
     }
     else
     {
         std::cout << "not loaded\n";
     }
}

//average normalvector of faces that share the same vertexPoint
//vertexPoint == closest to given point
ngl::Vec3 Mesh::getNormalVector(ngl::Vec3 _point)
{
    ngl::Vec3 normalVector;
    int index = 0;
    auto normalVerticesList = getNormalVertices(getClosestVertex(_point).second);

    while (index < normalVerticesList.size())
    {
        normalVector.m_x += normalVerticesList[index].m_x;
        normalVector.m_y += normalVerticesList[index].m_y;
        normalVector.m_z += normalVerticesList[index].m_z;
        ++index;
    }

    normalVector.operator/=(static_cast<float>(normalVerticesList.size()));
    normalVector.normalize();

    return normalVector;
}


// Modified from:-
// Mahankali (2024) Tutorial: Mesh Closest Point Algorithm (by ranjeethmahankali)
// Accessed [09.01.2024]
// Available from // see https://www.food4rhino.com/en/resource/tutorial-mesh-closest-point-algorithm for Tutorial
//               // see https://github.com/ranjeethmahankali/galproject/blob/main/galcore/Mesh.cpp for Source Code commit 2aa56a4

ngl::Vec3 Mesh::getClosestPoint(ngl::Vec3 _point, const std::vector<ngl::Vec3> &_pointList)
{
    //goes over the pointList and checks if a point is closer than 0.5 to the input point -> takes that point instead of the input point
    for(auto point : _pointList)
    {

        if((getDistance(_point, point)) < 0.5 )
        {
            m_closestVertex = point;
            return m_closestVertex;
        }
    }

    //closest vertexpoint on mesh
    m_closestVertex = getClosestVertex(_point).first;
    auto distVec = _point - m_closestVertex;
    m_distance = distVec.lengthSquared();

    //faces that are inside the bounding box ( dimensions : distance from the center to each side
    std::vector<ngl::Face> facesToCheck = boxIntersection(_point,distVec.length());

    //goes over the faces that might have a closer vertex(inside the bounding box) and checks if there is a closer point
    // and if that point is inside the face
    for(const auto &face : facesToCheck)
    {
        pointInsideTriangle(face,_point);
    }
    return m_closestVertex;
}

//face is inside the box if the distance between the given point and
// one facevertex smaller than half of the dimension of the box == initial distance to closest vertex
std::vector<ngl::Face> Mesh::boxIntersection(ngl::Vec3 _centerPoint, float _halfBoxSize)
{
    std::vector<ngl::Face> _facesToCheck;
    bool faceInsideBox = false;

    for (const auto &face : m_faceList)
    {
        for(const auto &vertex : face.m_vert)
            {
                if(isInsideBox(m_vertexList[vertex], _centerPoint, _halfBoxSize))
                {
                    faceInsideBox = true;
                }

            }
            if(faceInsideBox)
            {
                _facesToCheck.push_back(face);
                faceInsideBox = false;
            }
    }
    return _facesToCheck;
}

bool Mesh::isInsideBox(ngl::Vec3 _point, ngl::Vec3 _centerPoint, float _halfBoxSize)
{
    return (((_point - _centerPoint).length()) <= _halfBoxSize);
}

//endOfCitation


//searches for the closest Vertex on the Mesh
std::pair<ngl::Vec3,int> Mesh::getClosestVertex(ngl::Vec3 _point)
{
    std::pair<ngl::Vec3, int> currentVertex = {m_vertexList[0], 0};

    for( int index = 0; index < m_vertexList.size(); ++index)
    {
        if(std::abs(getDistance(_point, currentVertex.first)) > std::abs(getDistance(_point, m_vertexList[index])))
        {
            currentVertex.first = m_vertexList[index];
            currentVertex.second = index;
        }
    }
    return currentVertex;

}

// calculates the dist between to vectors
float Mesh::getDistance(ngl::Vec3 _vec1, ngl::Vec3 _vec2)
{
    return ((_vec2.operator-=(_vec1)).length());
}


void Mesh::pointInsideTriangle(const ngl::Face &_face, ngl::Vec3 _point)
{
    auto firstFaceVertex = m_vertexList[_face.m_vert[0]];
    auto secondFaceVertex = m_vertexList[_face.m_vert[1]];
    auto thirdFaceVertex = m_vertexList[_face.m_vert[2]];

    auto faceNormal = getFaceNormal(firstFaceVertex, secondFaceVertex, thirdFaceVertex);

    // calculates the point which has the least distance to the given point and lays on the same plane as the face
    auto newPoint = getClosestPointOnAPlane(firstFaceVertex, _point, faceNormal);
    auto distance = (_point - newPoint).lengthSquared();

    //if the point lies also inside the triangle update closest vertex
    if(isOnTriangle(firstFaceVertex, secondFaceVertex, thirdFaceVertex, newPoint))
    {
        if(distance < m_distance)
        {

            m_distance = distance;
            m_closestVertex = newPoint;
        }

    }
}

// Cited from:-
// Marschner and Shirley (2021) 2.9 Triangles p. 53f
// Accessed [22.01.2024]
// Available from: ProQuest Ebook Central

//barycentric method
bool Mesh::isOnTriangle(ngl::Vec3 _firstFaceVertex, ngl::Vec3 _secondFaceVertex, ngl::Vec3 _thirdFaceVertex, ngl::Vec3 _point)
{
    auto normalVec = (_secondFaceVertex.operator-(_firstFaceVertex)).cross((_thirdFaceVertex.operator-(_firstFaceVertex)));
    auto normalVecA = (_thirdFaceVertex.operator-(_secondFaceVertex)).cross((_point.operator-(_secondFaceVertex)));
    auto normalVecB = (_firstFaceVertex.operator-(_thirdFaceVertex)).cross((_point.operator-(_thirdFaceVertex)));
    auto normalVecC = (_secondFaceVertex.operator-(_firstFaceVertex)).cross((_point.operator-(_firstFaceVertex)));

    auto distance = normalVec.lengthSquared();

    auto alpha = (normalVec.dot(normalVecA))/(distance);
    auto beta = (normalVec.dot(normalVecB))/(distance);
    auto gamma = (normalVec.dot(normalVecC))/(distance);

    return (alpha <= 1 ) && (beta <= 1) && (gamma <= 1 ) && ((alpha + beta + gamma) == 1);
}

//endfCitation


//hesse norm form
//returns point on plane with the least distance to the given point
ngl::Vec3 Mesh::getClosestPointOnAPlane(ngl::Vec3 _pointOnPlane,  ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec)
{
    ngl::Vec3 closestPointOnPlane;
    float parameter = getParameter(_pointOnPlane, _distanceVec, _normalVec);
    closestPointOnPlane.m_x = _distanceVec.m_x + _normalVec.m_x * parameter;
    closestPointOnPlane.m_y = _distanceVec.m_y + _normalVec.m_y * parameter;
    closestPointOnPlane.m_z = _distanceVec.m_z + _normalVec.m_z * parameter;

    return closestPointOnPlane;
}

float Mesh::getParameter(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec)
{
    auto numerator = ((_normalVec.m_x * ( _pointOnPlane.m_x - _distanceVec.m_x))
                            + (_normalVec.m_y * ( _pointOnPlane.m_y - _distanceVec.m_y))
                            + (_normalVec.m_z * ( _pointOnPlane.m_z - _distanceVec.m_z)));
    return numerator/_normalVec.length();
}



ngl::Vec3 Mesh::getFaceNormal(ngl::Vec3 _firstFaceVertex, ngl::Vec3 _secondFaceVertex, ngl::Vec3 _thirdFaceVertex)
{
    auto faceNormal = ((_secondFaceVertex.operator-(_firstFaceVertex)).cross((_thirdFaceVertex.operator-(_firstFaceVertex))));
    faceNormal.normalize();
    return faceNormal;
}



std::vector<ngl::Vec3> Mesh::getNormalVertices(int _vertexNumber)
{
    auto normalList = m_mesh.getNormalList();
    auto facesWithVertex = getFacesWithVertex(_vertexNumber);
    std::vector<ngl::Vec3> normalVertices;

    for(auto face :facesWithVertex)
    {

        for (int index = 0; index < face.m_norm.size(); ++index)
        {

            if ((face.m_vert[index] ) == _vertexNumber)
            {
                if ((std::find(normalVertices.begin(), normalVertices.end(), normalList[face.m_norm[index]])) == normalVertices.end())
                {
                    normalVertices.push_back(normalList[face.m_norm[index]]);
                }
            }
        }
    }
    return normalVertices;
}

std::vector<ngl::Face> Mesh::getFacesWithVertex(int _vertexNumber)
{

    std::vector<ngl::Face> facesWithVertex;

    for (auto face: m_faceList)
    {


        if ((std::find(face.m_vert.begin(), face.m_vert.end(), _vertexNumber)) != face.m_vert.end())
        {
            facesWithVertex.push_back(face);
        }
    }
    return facesWithVertex;
}


















