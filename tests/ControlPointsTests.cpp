#include <gtest/gtest.h>
#include "ControlPoints.h"

TEST(ControlPoints,appendPoint)
{
    // append a point to points vec
    ControlPoints b;
    b.appendPoint( ngl::Vec3(2.0f,  3.0f, 4.0f));

    EXPECT_EQ(2.0f, b.m_pointList[0][0]);
    EXPECT_EQ(3.0f, b.m_pointList[0][1]);
    EXPECT_EQ(4.0f, b.m_pointList[0][2]);


}


//get Index of given VECTOR
TEST(ControlPoints, getIndex)
{
    ControlPoints b;

    for(int i = 0; i < 5; ++i)
    {
        b.appendPoint(ngl::Vec3(static_cast<float>(i+1), static_cast<float>(i+2), static_cast<float>(i+3)));
    }

    auto vec = ngl::Vec3(5.0f, 6.0f, 7.0f);

    EXPECT_EQ(4, b.getIndex(vec));


}

TEST(ControlPoints, pointExists)
{
    ControlPoints b;

    for(int i = 0; i < 5; ++i)
    {
        b.appendPoint(ngl::Vec3(static_cast<float>(i + 1), static_cast<float>(i + 2), static_cast<float>(i + 3)));
    }

    EXPECT_TRUE(b.pointExists(ngl::Vec3(2.0f, 3.0f, 4.0f)));
    EXPECT_FALSE(b.pointExists(ngl::Vec3(8.0f, 8.0f, 8.0f)));
}

TEST(ControlPoints, PointOnPlane)
{
    ControlPoints b;
    ngl::Vec3 closestPoint = b.getPointOnPlane({0,2,0}, {-0.707,2.239,-0.707}, {0,1,0} );


    EXPECT_FLOAT_EQ(-0.707, closestPoint.m_x);
    EXPECT_FLOAT_EQ(2, closestPoint.m_y);
    EXPECT_FLOAT_EQ(-0.707, closestPoint.m_z);
}

TEST(ControlPoints, findPoint)
{
    ControlPoints b;

    for(int i = 0; i < 5; ++i)
    {
        b.appendPoint(ngl::Vec3(static_cast<float>(i+1), static_cast<float>(i+2), static_cast<float>(i+3)));
    }

    bool found = b.findPoint({3,4,5}).first;

    EXPECT_TRUE(found);

}

TEST(ControlPoints, calcControlPoint)
{
    ControlPoints b;
    Mesh m("models/zylinder_for_tests.obj");
    ngl::Vec3 resVec = b.calcCtrlPoint(  {0, 2, -1}, {0, 0, -1}, m);

    EXPECT_FLOAT_EQ(0, resVec.m_x);
    EXPECT_FLOAT_EQ(1.4136899, resVec.m_y);
    EXPECT_FLOAT_EQ(-1.3173087, resVec.m_z);
}

