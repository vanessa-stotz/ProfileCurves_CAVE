The project is split into three different classes: NGL Scene, ControlPoints and Mesh.
A class diagram for the classes can be found in Design.md in the git repository.


The **NGL Scene** class involves everything that is connected to displaying the mesh, curves, and points. 
The calculation for displaying the Bézier curve happens in the Geometry Shader. 
An array is inputted, and after 4 points, a restart call is made. 
With the collection of the four points, the geometry shader calculates the points of the Bézier curve.
The algorithm can be found in the geometry shader file.

The **Mesh class** takes a path to a .obj file as input and saves a list of the faces and vertices as global variables. 
Inside that class, all calculations that are connected to the mesh happen. 
The calculation for the closest point on the mesh with the constructed box and the barycentric coordinates can be found in **lines 45 to 186** of the Mesh class.


The **ControlPoints** class takes the given points and the path for the mesh as inputs. 
The input of the mesh is needed, as it calls functions from Mesh within its functions. 
The global variables of the ControlPoints class are the point list, which is a std::vector and a std::vector of arrays with a size of four, which contains the different Bézier curves.
This contrasts with the research paper, as it asks for the Bézier curves to be stored “as a tuple of four indices” (De Goes et al. 2022, p. 139:5) 
An array can have a fixed size, like a tuple, but it is easier to access and save data, therefore the change was made.

For the calculation of the inner control points, a plane with an outer control point and the normal vector of the surface is constructed. 
An intersection point is calculated, and its values are used to calculate the inner control point. This calculation with the Hesse normal form can be found in **lines 122 to 184** of the ControlPoints class.

The research paper did not define the calculation of the inner control points; it was personally decided to take the Hesse normal form as a possible method. However, depending on the size of the mesh the tangent handles tend to get too big and deform the curve in an undesired way. 
Therefore, this method needs further improvement. 

References

De Goes, F., Sheffler, W. and Fleischer, K., 2022. Character articulation through profile curves. ACM transactions on graphics [online], 41 (4), 1–14. Available from: http://dx.doi.org/10.1145/3528223.3530060 [Accessed 22 January 2024]


