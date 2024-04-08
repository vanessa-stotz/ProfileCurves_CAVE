/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <iostream>
#include <QtGui/QGuiApplication>
#include "NGLScene.h"



int main(int argc, char **argv)
{
    //std::string oName = "shaders/sphere01.obj";
    //Mesh m(oName);// can be a user defined input later on
    //std::vector<ngl::Vec3> inputPoints = {{-0.2f,1.5,1} , {0,0,1}, {1,2,0}, {1,0,0}, {0,2,-1}, {0,0,-1}, {0,2.2,-1}, {0,3,0} }; //later aswell from file or TOP would be as userInput -> mouseEvent
    //ControlPoints(inputPoints, m);

    QGuiApplication app(argc, argv);
    // create an OpenGL format specifier
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);
#if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
#else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
#endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile(QSurfaceFormat::CoreProfile);
    // now set the depth buffer to 24 bits
    format.setDepthBufferSize(24);
    // now we are going to create our scene window
    NGLScene window;
    // and set the OpenGL format
    window.setFormat(format);
    // we can now query the version to see if it worked
    std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
    // set the window size
    window.resize(1024, 720);
    // and finally show
    window.show();

    return app.exec();
}



