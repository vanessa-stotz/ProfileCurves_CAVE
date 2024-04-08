#include <algorithm>
#include <iostream>

#include <ngl/SimpleIndexVAO.h>

#include "ControlPoints.h"



ControlPoints::ControlPoints(const std::vector<ngl::Vec3> &_definedCtrlPoints, Mesh &_mesh)
{
    //constructor which gets all the input points
    //stores them in the profileCurves
    // count%2 == startpoint, else endpoint
    int count = 0;
    bool startPoint;
    for (auto point: _definedCtrlPoints)
    {
        ngl::Vec3 closestPoint = _mesh.getClosestPoint(point, m_pointList);
        if (count % 2 != 0)
        {
            startPoint = false;
            storeCtrlPoints(closestPoint, startPoint);
            ++count;
        }
        else
        {
            startPoint = true;
            m_profileCurves.resize(m_profileCurves.size()+1);
            storeCtrlPoints(closestPoint, startPoint);
            ++count;
        }
    }

    //calculate missing controlPoints
    calcInnerControlPoints(_mesh);

    // check if it worked
    for(auto curves : m_profileCurves)
    {
        for(auto &curve : curves)
        {
            std::cout << curve.m_x << " /";
            std::cout << curve.m_y << " /";
            std::cout << curve.m_z << "\n";
        }
        std::cout << "-----------------\n";
    }
    std::cout << "-----------------\n";
}


void ControlPoints::storeCtrlPoints(ngl::Vec3 _point, bool _startPoint)
{
    appendPoint(_point);

    // stores the points in different places whether they are start or endpoints
    if(!_startPoint)
    {
        setCtrlPoint(_point, static_cast<int>(m_profileCurves.size()-1), 3);
    } else
    {
        setCtrlPoint(_point, static_cast<int>(m_profileCurves.size()-1), 0);
    }
}

void ControlPoints::appendPoint(ngl::Vec3 _point)
{
    if(!pointExists(_point))
    {
        m_pointList.push_back(_point);
    }
}



// Modified from:-
// Varun (2023) C++: How to find an element in vector and get its index?
// Accessed [20.11.2023]
// Available from // see https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/

std::pair<bool,int> ControlPoints::findPoint(ngl::Vec3 _point) const
{
    std::pair<bool, int> result;
    auto iterator = std::find(m_pointList.begin(), m_pointList.end(), _point);

    if(iterator != m_pointList.end())
    {
        result.first = true;
        result.second = std::distance(m_pointList.begin(), iterator);
    }
    else
    {
        result.first = false;
        result.second = -1;
    }

    return result;
}

int ControlPoints::getIndex(ngl::Vec3 _point) const
{
    return findPoint(_point).second;
}

bool ControlPoints::pointExists(ngl::Vec3 _point) const
{
    return findPoint(_point).first;
}
//endOfCitation



void ControlPoints::setCtrlPoint(ngl::Vec3 _point, int _numProfileCurve, int _indexCtrlPoint)
{
    auto &ctrlPoint = m_pointList[getIndex(_point)];
    m_profileCurves[_numProfileCurve][_indexCtrlPoint] = ctrlPoint;
}


//calculates the closest point on the plane to a given point
ngl::Vec3 ControlPoints::getPointOnPlane(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec)
{
    ngl::Vec3 pointOnPlane;
    float parameter = getParameter(_pointOnPlane, _distanceVec, _normalVec);
    pointOnPlane.m_x = _distanceVec.m_x + _normalVec.m_x * parameter;
    pointOnPlane.m_y = _distanceVec.m_y + _normalVec.m_y * parameter;
    pointOnPlane.m_z = _distanceVec.m_z + _normalVec.m_z * parameter;

    return pointOnPlane;
}

//paramenter needed for getPointOnPlane calculation
float ControlPoints::getParameter(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec)
{
    auto numerator = ((_normalVec.m_x * (_pointOnPlane.m_x - _distanceVec.m_x))
                            + (_normalVec.m_y * (_pointOnPlane.m_y - _distanceVec.m_y))
                            + (_normalVec.m_z * (_pointOnPlane.m_z - _distanceVec.m_z)));
    return numerator / _normalVec.length();
}

void ControlPoints::calcInnerControlPoints(Mesh &_mesh)
{
    int _indexCurve = 0;
    for( auto curve : m_profileCurves)
    {
        setInnerControlPoints(curve,_indexCurve, 0,3, _mesh, true);
        setInnerControlPoints(curve, _indexCurve, 3, 0, _mesh, false);
        ++_indexCurve;
    }
}


void ControlPoints::setInnerControlPoints(std::array<ngl::Vec3,4> _curve, int _indexCurve, int _mainPoint, int _secondaryPoint, Mesh &_mesh, bool _startPoint)
{
    int _indexCtrlPoint;
    if(_startPoint)
    {
        _indexCtrlPoint = 1;
    } else
    {
        _indexCtrlPoint = 2;
    }
    auto ctrlPoint = calcCtrlPoint(_curve[_mainPoint], _curve[_secondaryPoint], _mesh);
    appendPoint(ctrlPoint);
    setCtrlPoint(ctrlPoint, _indexCurve, _indexCtrlPoint);

}




ngl::Vec3 ControlPoints::calcCtrlPoint(ngl::Vec3 _mainPoint, ngl::Vec3 _secondaryPoint, Mesh &_mesh)
{
    auto dist = (_mainPoint.operator-(_secondaryPoint).length());
    auto normalVector = _mesh.getNormalVector(_mainPoint);
    auto pointOnPlane = getPointOnPlane(_mainPoint,_secondaryPoint,normalVector);
    pointOnPlane.operator-=(_mainPoint);
    pointOnPlane.normalize();
    pointOnPlane.operator*=(dist/3);
    _mainPoint.operator+=(pointOnPlane);
    return _mainPoint;

}



//DRAWING FUNCTIONS

void ControlPoints::createVAO() noexcept
{
    if(m_vaoCurve != nullptr && m_vaoPoints != nullptr)
    {
        m_vaoPoints->unbind();
        m_vaoPoints->removeVAO();
        m_vaoCurve->unbind();
        m_vaoCurve->removeVAO();
    }

    m_vaoPoints=ngl::VAOFactory::createVAO("simpleVAO", GL_POINTS);
    m_vaoCurve = ngl::VAOFactory::createVAO(ngl::simpleIndexVAO, GL_LINE_STRIP_ADJACENCY);

    std::vector<ngl::Vec3> points;

    constexpr GLushort restart = 9999;
    std::vector<GLshort> indexList;
    GLshort index = -1;

    for(auto curve : m_profileCurves)
    {
        for(auto i : curve)
        {
            points.push_back(i);
            indexList.push_back(++index);
        }
        indexList.push_back(restart);
    }

    m_vaoPoints->bind();
    m_vaoPoints->setData(ngl::SimpleVAO::VertexData(points.size()*sizeof(ngl::Vec3),points[0].m_x));
    m_vaoPoints->setNumIndices(points.size());
    m_vaoPoints->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vaoPoints->unbind();

    m_vaoCurve->bind();
    m_vaoCurve->setData(ngl::SimpleIndexVAO::VertexData(points.size() * sizeof(ngl::Vec3), points[0].m_x, indexList.size(), &indexList[0], GL_UNSIGNED_SHORT));
    m_vaoCurve->setVertexAttributePointer(0,3,GL_FLOAT, 0, 0);
    m_vaoCurve->setNumIndices(indexList.size());
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart);
    m_vaoCurve -> unbind();


}

void ControlPoints::drawBezier() const noexcept
{
    m_vaoCurve->bind();
    m_vaoCurve->setMode(GL_LINES_ADJACENCY);
    m_vaoCurve->draw();
    m_vaoCurve->unbind();
}

void ControlPoints::drawControlPoints() const noexcept
{
    m_vaoPoints->bind();
    m_vaoPoints->setMode(GL_POINTS);
    m_vaoPoints->draw();
    m_vaoPoints->unbind();

}

void ControlPoints::drawTangents() const noexcept
{
    m_vaoPoints->bind();
    m_vaoPoints->setMode(GL_LINES);
    m_vaoPoints->draw();
    m_vaoPoints->unbind();
}
