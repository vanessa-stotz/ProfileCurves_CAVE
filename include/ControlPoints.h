#ifndef CONTROLPOINTS_H_
#define CONTROLPOINTS_H_

#include <gtest/gtest.h>

#include <ngl/Vec3.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>

#include "Mesh.h"


class ControlPoints
{
    friend class ControlPointsTests;
    FRIEND_TEST(ControlPoints, findPoint);
    FRIEND_TEST(ControlPoints, calcControlPoint);

public:

    std::vector<ngl::Vec3> m_pointList;
    std::vector<std::array<ngl::Vec3, 4>> m_profileCurves;

    ControlPoints() = default;
    explicit ControlPoints(const std::vector<ngl::Vec3> &_definedCtrlPoints, Mesh &_mesh);

    void appendPoint( ngl::Vec3 _point);
    [[nodiscard]] int getIndex(ngl::Vec3 _point) const;
    [[nodiscard]] bool pointExists(ngl::Vec3 _point) const;
    void setCtrlPoint(ngl::Vec3 _point, int _numProfileCurve, int _indexCtrlPoint);

    [[nodiscard]] ngl::Vec3 getPointOnPlane(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec,  ngl::Vec3 _normalVec);


    void createVAO() noexcept;
    void drawBezier() const noexcept;
    void drawControlPoints() const noexcept;
    void drawTangents() const noexcept;

private:
    std::unique_ptr<ngl::AbstractVAO>  m_vaoPoints;
    std::unique_ptr<ngl::AbstractVAO> m_vaoCurve;

    [[nodiscard]] std::pair<bool,int> findPoint(ngl::Vec3 _point) const;
    [[nodiscard]] float getParameter(ngl::Vec3 _pointOnPlane, ngl::Vec3 _distanceVec, ngl::Vec3 _normalVec) ;
    void storeCtrlPoints(ngl::Vec3 _point, bool _startPoint);
    void calcInnerControlPoints(Mesh &_mesh);
    void setInnerControlPoints(std::array<ngl::Vec3,4> _curve, int _indexCurve,  int _mainPoint, int _secondaryPoint, Mesh &_mesh, bool _startPoint);
    ngl::Vec3 calcCtrlPoint(ngl::Vec3 _mainPoint, ngl::Vec3 _secondaryPoint, Mesh &_mesh);


};


#endif