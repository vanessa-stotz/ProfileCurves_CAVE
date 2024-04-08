## How to operate the programme

The programme relies on the NCCA Graphics Library (NGL).
With the NGL library installed, it just needs to be compiled and run.

NOTE: If the code is run via Clion, the ProfileCurveCopyResources need to be made a CMakeTarget in ProfileCurves before building and running.

There are no user inputs. However, the user is able to toggle between different meshes and display modes.

1. With the arrow up and down keys, the display of the curves can be toggled between the points that are given to the algorithm
   and the resulted Bézier curves.
2. The numbers 1 and 2 toggle between different meshes (1 = sphere, 2 = bunny).
3. With the keys E and R, random points can be generated.
  4. R creates random points inside the mesh's size.
  5. E chooses random points from the existing vertex list of the mesh


##  How it works


The project takes two points and constructs a cubic Bezier spline. The two points are self-defined.
It first saves them in a point list, which contains all the points used and also stores them in a vector list of arrays with a size of 4.
Each array represents a Bézier curve with its associated control points.

Before the points are stored in the list and array, they get tested to see if they lie on the surface. If not, the closest point is defined and the point is moved to that location.

The inner control points are perpendicular to the surface normal. Therefore, a plane is created with an outer control point and the surface normal.
The closest point from the plane to the other control point is calculated. The distance between the first outer control point and the closest point is then defined and added to the outer control point.
This results in the inner control point.

As a final step the Bézier curves, individual points and tangent handles are displayed.


## Outlook

Further implementation for future updates:
1. change the points to user inputs via drag and drop.
2. better calculation of the length of the tangent handles
3. adding further features to manipulate the Bézier curves
  4. split splines
  5. merge splines
  6. weld and break control points
  8. flatten tangents
4. Make the points selectable and movable
