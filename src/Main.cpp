/* 3D Pool Game
* 
* Summary:
* 	This work aims to apply the main 3D programming concepts covered in class. Thus, students, organized into
* 	groups of 4, must develop a program in C++ that, through the use of the OpenGL, GLEW, GLM and GLFW
* 	libraries, performs the rendering of a 3D scene, applying the respective textures, lighting, and performs an
* 	animation in response to a keyboard event.This work will result in the delivery, via Moodle, of the files with the
* 	source code, as well as an oral presentation / defense of the work carried out.
* 
* 	Objective:
* 	This work aims to develop a program in C++ (respecting the Object-Oriented Programming paradigm) that renders
* 	a 3D scene, applying the respective textures and lighting, and performs an animation (movement of one of the
* 	balls) in response to a keyboard event.
* 	The requirements are grouped here into 4 steps, in order to guide students in solving the work. Please note,
* 	however, that you should only submit a single version of your program (the final version), which meets all
* 	the requirements.
* 
* - Step 1 :
* 	• Implement window management and user interface through the GLFW library;
* 	• Implement the manipulation of matrices and vectors through the GLM library;
* 	• ( 1
* 	) Implement the rendering of a parallelepiped (to simulate the pool table) through the OpenGL library
* 	(including GLEW);
* 	• ( 2 ) The coloring of the parallelepiped fragments must be done so that each face has a distinct color (note
* 	that it is not expected that the lighting effects will be implemented at this stage);
* 	• The application should allow the user to zoom using the mouse scroll wheel ;
* 	• The application should allow navigation (rotation) around the center of the parallelepiped, through mouse
* 	movements.
* 	• Implement a minimap of the pool table, to be displayed in the top right corner of the application window.
* 	This minimap should show a top view perspective of the table. The minimap should not be affected by
* 	lighting.
* 
* - Step 2 :
* 	• Students must develop a C++ library (respecting the Object-Oriented Programming paradigm) capable of:
* 	o Load vertex position data, normals, and texture coordinates from .obj files;
* 	o Reorganize vertex information according to face information;
* 	o Send vertex data to GPU memory (VAO and respective VBO);
* 	o Identify the name of the .mtl file in the .obj file;
* 	o Load material properties (.mtl file), including texture image ;
* 	o Load the texture image into an OpenGL texture unit;
* 	o Provide function for rendering the object.
* 	Although the .mtl format supports multiple materials, for this work assume that a .mtl file will only have
* 	a single material to apply to the entire object.
* 	• The application must use this C++ library to load the models, compose and render a 3D scene that
* 	presents 15 balls arranged on a snooker table.
* 	Figure 1: Example of composition with billiard table and balls.
* 	• The camera should remain still in a position that allows the entire table to be viewed, oriented towards the
* 	center of the table.
* 	• The library must have its own namespace in which a class (and its respective member functions) will be
* 	defined to manage the rendering process. The following member functions must be implemented :
* 	o Load(const std::string obj_model_filepath)
* 	▪ This function aims to load the data from the .obj file, whose path is passed as an
* 	argument, into the CPU memory.
* 	▪ You must also load the materials and textures associated with the model into the CPU
* 	memory.
* 	o Install (void)
* 	ENGINEERING IN DIGITAL GAME DEVELOPMENT
* 	3D PROGRAMMING – 2ND YEAR Academic Year 2024/2025
* 	Polytechnic Institute of Cávado and Ave
* 	Higher School of Technology
* 	▪ Generates the necessary VAO and VBO and sends the model data (vertices, texture
* 	coordinates and normals) to GPU memory.
* 	o Render(glm::vec3 position, glm::vec3 orientation)
* 	▪ Function to render the model in the position defined by position and orientation defined
* 	by orientation .
* 	o Functions must also be created that allow the association between the model data (previously
* 	loaded to the GPU through the Install() function ) and the attributes of a shader program.
* 	o In addition to these mandatory functions, others may be created if deemed necessary.
* 
* - Step 3 :
* 	• It should be possible to apply any combination of 4 light sources that fall on the object:
* 	o Ambient light;
* 	o Directional light;
* 	o Spot light;
* 	o Conical light.
* 	• The parameters of each of the light sources are at the discretion of each group;
* 	• The application should allow the user to activate/deactivate each light source, using a key:
* 	o '1' – Enable/disable ambient light source;
* 	o '2' – Enable/disable directional light source;
* 	o '3' – Enable/disable point light source;
* 	o '4' – Enable/disable cone light source.
* 
* - Step 4 :
* 	• Implement the movement animation (translation and rotation) of one of the billiard balls. This animation
* 	should be triggered as soon as the user presses the "Space" key. The animation should stop when the ball
* 	hits another billiard ball or the table boundaries.
*/

#include "App.h"

using namespace std;
using namespace PoolGame3D;

int main() {
    App app;
    if (!app.initialize()) return -1;
    app.run();
    return 0;
}
