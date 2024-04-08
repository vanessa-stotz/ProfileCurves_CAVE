#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <iostream>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>

// Modified from the NCCA Graphics Library:-
// Macey (2022) Blank NGL
// Accessed [22.01.2024]
// Available from // see https://github.com/NCCA/BlankNGL
// commit ccc8824

NGLScene::NGLScene()
{
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    setTitle("ProfileCurves");
    m_objFileName = "models/bunny.obj";
    m_inputPoints = m_inputPointsBunny;
}


NGLScene::~NGLScene()
{
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
    m_project = ngl::perspective(45.0, static_cast<float>(_w) / static_cast<float>(_h), 0.1f, 20.0f);
    m_win.width  = static_cast<int>( _w * devicePixelRatio() );
    m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

// shaders for scene
constexpr char *ColourShader = "ColourShader";
constexpr char *CurveShader = "CurveShader";

void NGLScene::initializeGL()
{
    // we must call that first before any other GL commands to load and link the
    // gl commands from the lib, if that is not done program will crash
    ngl::NGLInit::initialize();
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    ngl::ShaderLib::loadShader(ColourShader, "shaders/ColourVertex.glsl", "shaders/ColourFragment.glsl");

    ngl::ShaderLib::loadShader(CurveShader, "shaders/CurveVertex.glsl", "shaders/CurveFragment.glsl", "shaders/CurveGeometry.glsl");

    //create basic camera from graphics library
    //static camera which only needs to be set once

    //first create values for the camera position
    ngl::Vec3 from(6,5, 4);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    m_view = ngl::lookAt(from, to, up);


    initializeData();

}



void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_win.width,m_win.height);


    auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
    auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);

    m_mouseGlobalTX = rotY * rotX;

    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

    //toggle between display of input points and resulting Bézier curves
    if(m_output)
    {
        drawOutput();
    } else
    {
        drawInput();
    }


}

//creating VAOs for the different object
void NGLScene::initializeData()
{
    m_mesh.reset(new ngl::Obj(m_objFileName));
    m_mesh->createVAO();

    Mesh m(m_objFileName);
    m_profileCurves = std::make_unique<ControlPoints>(m_inputPoints,m);
    m_profileCurves->createVAO();

    m_vaoInputPoints=ngl::VAOFactory::createVAO("simpleVAO", GL_POINTS);

    m_text = std::make_unique<ngl::Text>("fonts/Arial.ttf", 15);
    m_text->setScreenSize(1024,720);

}

//draw resulting points, tangent handles and Bézier curves
void NGLScene::drawOutput()
{
    //drawText;
    drawText();
    ngl::ShaderLib::use(ColourShader);
    ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX); //default ngl::Mat4 is defaulting to uniform matrix
    ngl::ShaderLib::setUniform("colour", 0.5f, 0.5f, 0.5f);

    // draw OBJ;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_mesh->draw();

    //draw controlPoints;
    glPointSize(4);
    ngl::ShaderLib::setUniform("colour", 0.0f, 1.0f, 0.0f);
    m_profileCurves->drawControlPoints();

    //draw tangents;
    glPointSize(1);
    ngl::ShaderLib::setUniform("colour", 0.0f, 0.0f, 1.0f);
    m_profileCurves->drawTangents();

    //draw Bézier;
    ngl::ShaderLib::use(CurveShader);
    ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX); //default ngl::Mat4 is defaulting to uniform matrix
    m_profileCurves->drawBezier();
}


//draw input points
void NGLScene::drawInput()
{
    //draw Text
    drawText();

    ngl::ShaderLib::use(ColourShader);
    ngl::ShaderLib::setUniform("MVP", m_project * m_view * m_mouseGlobalTX); //default ngl::Mat4 is defaulting to uniform matrix
    ngl::ShaderLib::setUniform("colour", 0.5f, 0.5f, 0.5f);

    // draw OBJ;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_mesh->draw();

    //draw input points
    glPointSize(4);
    ngl::ShaderLib::setUniform("colour", 0.0f, 1.0f, 0.0f);
    m_vaoInputPoints->bind();
    m_vaoInputPoints->setData(ngl::SimpleVAO::VertexData(m_inputPoints.size()*sizeof(ngl::Vec3),m_inputPoints[0].m_x));
    m_vaoInputPoints->setNumIndices(m_inputPoints.size());
    m_vaoInputPoints->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vaoInputPoints->draw();
    m_vaoInputPoints->unbind();

}


void NGLScene::drawText()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_text->setColour(1.0,1.0,1.0);
    int pos = 700;
    m_text->renderText(10,700, " Toggle between geometry:");
    m_text->renderText(30,(pos -=23), " 1 = sphere");
    m_text->renderText(30,(pos -=23), " 2 = bunny");
    m_text->renderText(10,(pos -=23), "ARROW UP: input Points ");
    m_text->renderText(10,(pos -=23), "ARROW DOWN: resulted ProfileCurves ");
    m_text->renderText(10,(pos -=23), "R: random points");
    m_text->renderText(10,(pos -=23), "E: random vertex point");
    m_text->renderText(10,(pos -=23), "SPACE: reset Camera");
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
    // this method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    switch (_event->key())
    {
        // escape key to quite
        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
        case Qt::Key_Space :
        {
            m_win.spinXFace = 0;
            m_win.spinYFace = 0;
            m_modelPos.set(ngl::Vec3::zero());

            break;
        }

        case Qt::Key_Up:
        {
            m_output = false;
            update();
            break;
        }
        case Qt::Key_Down:
        {
            m_output = true;
            update();
            break;
        }

        //display sphere
        case Qt::Key_1 :
        {
            m_objFileName = "models/sphere.obj";
            m_inputPoints = m_inputPointsSphere;
            m_selectedObject = 0;
            initializeData();
            update();
            break;
        }
        //display bunny
        case Qt::Key_2 :
        {
            m_objFileName = "models/bunny.obj";
            m_inputPoints = m_inputPointsBunny;
            m_selectedObject = 1;
            initializeData();
            update();
            break;
        }
        //random points in scale range of geometry
        case Qt::Key_R :
        {
            std::vector<ngl::Vec3> randomPoints;
            for (int i = 0; i < 21; ++i)
            {
                if(m_selectedObject == 0)
                {
                    randomPoints.push_back(ngl::Random::getRandomPoint(1, 1, 1));
                } else if(m_selectedObject == 1)
                {

                    randomPoints.push_back(ngl::Random::getRandomPoint(1, 2, 1));
                }

            }
            m_inputPoints = randomPoints;
            initializeData();
            update();
            break;
        }
        //random point from existing vertexList
        case Qt::Key_E :
        {
            std::vector<ngl::Vec3> randomPoints;
            auto vertexLst = m_mesh->getVertexList();

            for (int i = 0; i < 21; ++i)
            {
                int randomIndex = rand() % vertexLst.size();
                randomPoints.push_back(vertexLst[randomIndex]);
            }
            m_inputPoints = randomPoints;
            initializeData();
            update();
            break;
        }
        default : break;

    }
    // finally update the GLWindow and re-draw
    update();
}

//endOfCitation

