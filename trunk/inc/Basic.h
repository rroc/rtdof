#ifndef BASIC1_H
#define BASIC1_H

//#include <windows.h>

#include <stdio.h>    // for fopen, FILE & NULL
#include <cstdlib>    // for exit and atoi
#include <iostream>   // for cout and cerr
#include <sstream>    // for istrstream and ostringstream
#include <fstream>    // for fprintf
#include <string>     // for string and strcpy()
#include <iomanip>    // for setw()
//#include <unistd.h>   // for sleep
#include <assert.h>   // for assert (bypass with  -DNDEBUG)
#include <math.h>     // for basic math
#include <ctype.h>
#include <cassert>
#include <vector>
#include <algorithm>
#include <time.h>


// ***** LINUX INCLUDE
//#include <GL/glut.h>    // Header File For The GLUT Library for linux
//#include <GL/gl.h>	// Header File For The OpenGL32 Library for linux
//#include <GL/glu.h>	// Header File For The GLu32 Library for linux

#ifdef __APPLE__
#include <OpenGL/gl.h>		// Header File For The OpenGL32 Library for Mac os x
#include <OpenGL/glu.h>		// Header File For The GLu32 Library for Mac os x
#include <GLUT/glut.h>		// Header File For The GLUT Library for Mac os x

#elif defined _WIN32
#include <GL/glew.h>
#include <GL/glut.h>    // Header File For The GLUT Library
#endif

#include "myLab.h"		//Global defines

/* Elementary 3D graphic programming.
*
* \ref intro_sec
*              - \ref labdep
*              - \ref labcomp
*              - \ref labdoc
*              - \ref labbefor
*              - \ref labpre
*
* 1 \ref first_a
*              - \ref first_1
*              - \ref first_2
*              - \ref extra_1
*
* 2 \ref second
*              - \ref second_1
*              - \ref second_2
*              - \ref second_3
*
* 3 \ref third
*              - \ref third_1
*              - \ref third_2
*              - \ref extra_3
*
* 4 \ref fourth
*              - \ref fourth1
*              - \ref fourth2
*              - \ref fourth3
*              - \ref extra4
*
* 5 \ref Fifth
*			   - \ref fifht1
*			   - \ref fifht2
*			   - \ref fifht3
*			   - \ref fifht4
*
* <br>
* <p>
* <br>
*
* \section intro_sec Introduction
*
* This lab course in elementary 3D graphic programming using C++ is produced to give you all the basic concepts in 3D graphics,
* the aims that we have had while designing this lab course were:
*      -       Learn the basic knowledge in 3D graphics programming hands on
*      -       Put your programming skills to a good and relevant use.
*      -       To see one of the many uses of linear algebra in action and learn to appreciate it as a useful tool.
*
*      When you have completed the five lab exercises you will have created a collection of c++ classes,
* which implement some of the basic algorithms. This can be hard to do from scratch if you never have done any computer
* graphics programming therefore a lab skeleton has been implemented,
* you are then asked to implement the actual algorithms inside the classes and methods, and apply them to different 3D graphic objects.
* Feel free to create/change these classes, as long you solve the lab exercises. Feel free to use the labs as a
* basis for any project in the future, but please do not supply them to students that have not taken the course yet.
*
* When you have completed the lab course we hope that you will be rightfully proud of the results of your efforts,
* but it will not be something that amazes the general public, or even you friends and family. Rather, everything will be
* re-implementations of things that are already available in professional graphics packages, and unfortunately
* it will be a rather limited tool. It might even be so that the algorithm you implement is already implemented in
* your graphics card and you can replace you algorithm with one line of code but remember that this lab is not to
* produce something that is cutting edge in computer graphics but is designed for you to learn the details well here. After the labs you will have a
* thorough understanding of 3D graphics to build upon and do wonderful, amazing and advanced computer graphics in later courses.
* When designing these labs we have not searched for the most efficient way to solve the problems, but
* instead of a class structure for simple understanding and that can be successively improved upon during
* the progress of the lab series. The classes are aimed at solving exactly the problems presented
* in the labs course, not solving the general problems of 3D graphics.
*
* We hope that these labs should give you many hours of fun, interesting programming and
* not just become a programming assignment, remember that after each assignment there are some extra assignments.
* These are not compulsory, but if you find computer graphics fun, or you finish an assignment quickly, feel free to implemnt,
* one or as many you wish. If you come up with you own ideas feel free to implement that, but remember to always
* complete the compulsory part first. If you encounter any hard problems, we are always open for questions, but try first to solve the problem.
*
*
* \subsection labdep Lab dependencies
*
* The school computer lab that you have been assigned to has everything installed for you to complete the labs,
* but you want to work with them on you home computer you may do so. You need first openGL installed, you
* can find more information on www.opengl.org or on the homepage of manufacturer of you graphic card. You also need glut
* installed, this is a window handling toolkit, that can draw openGL, but also handles mouse and keyboard
* events and more, for information look at http://www.opengl.org/resources/libraries/glut.html, you also
* need the make tool and c++ library with the stl installed <br>
*
* \subsection labcomp Compiling
* With this lab there is a makefile that you may use to compile your code. If you create any of you own classes or
* executables, you have to make changes in the makefile. Makefiles are very powerful to use, and can even be a
* powerful tool to use when developing, debugging and optimizing code. The official homepage for gnumake
* can be found at http://www.gnu.org/software/make/ but if you want to learn more about make files
* I recommend that you do some googling (http://www.google.com)<br>
*
* Here under are the commands that have been implemented, use you favorite command tool (example bash, but use the one you prefer)
*
*      - <i>make clean</i> this will clean you build directory. It removes all executables and .o files (object files), it will not alter your source code.
 *      - <i>make superclean</i> this is the same as clean, but it also removes the doc directory (might be useful if you want to upload the code to your home computer, and don't wont to transfer the documentation, object and executable files.)
*      - <i>make labb1</i> this will build all your objects and executables from the labb1.cpp file
*      - <i>make doc</i> this will remove the /doc and then execute the doxygen command to build the documentation.
*
*
* \subsection labdoc Lab documentation
* This lab has been documented with doxygen, which is a powerful tool to document your code.
* Doxgen can output documentation in many formats such as html pages, man, xml, latex or pdf. <br>
* If you have found the html pages that is good, but if you are reading the pdf file, you might want to
* check out the html pages, here you can click around in the code, and very easily see how it works.<br>
* Take a look in the doc/html directory, open the index.html in you favorite web browser. If there is no documentation
* run either <i>make doc</i> to use the makefile and build the documentation, or run the doxygen
* directly with <i>doxygen Doxyfile</i><br>
* Feel free to implement your own comments/documentation in the .h and .cpp files, and use doxygen to
* extract it into these documentation files. <br>
* Don't think that documentation is something that is easy, your code might be efficient and superior other coders,
* but if no one can understand it except you, it is worthless. Learn how to document code during you lab sessions,
* it will be worth it.
*<br>
*<b>Remember that it might take weeks until you have a lab session where you
* will be able to show your results. Some well written
* comments can save you hours of work understanding your code. </b>
*
* You can learn more about doxygen at www.doxygen.com.
*
*\subsection labbefor Before the lab session
*
* <b>Each assignment is supposed to require considerable preparation and even some good thought afterwards,
* so please don't expect to be able to show up unprepared and have the assignment over and done in a four
* hour session. Your lab course is worth a certain number of credits, each credit resemble a week of work
* ( 40 hours) but your lab sessions are only 4 hours, this means that you have to work with the labs outside
* the scheduled lab sessions. The labs are not divided up in even workload, some labs will require more work then other.</b>
*
* Start by reading through the lab, it is also a good idea to read through the
* lecture notes to find information you need to solve the lab exercises. You might also want to read parts in your
* computer graphics book. Make sure that you know what you want to do and which methods or equations you will use and how they work before you start to implement them, this will save you a lot of time.<br>
* It is also a good idea to bring your computer graphics book to the lab session, it could also be a good idea to bring
* your linear algebra book, and if you want a c++ book. You don't need any special linear algebra book or c++ book,
* the one you have used before will be sufficient)
*
*
* \subsection labpre Lab presentation
* You are expected to present the labs to the lab assistant. You don't have to do this at every lab session, but don't
* expect to be able to present all the labs at the last session, you should do this under the lab series.<br>
* When you present the labs, make sure that you can answer questions on any part of the code, the lab assistant wants to know how you
* thought and how you solved the problem.<br>
* No screen dumps, movies (screen captures), print outs or log files will be accepted. You are expected to show working
* executables and you are expected to present every part of the lab when you have finished the whole lab.
* If you work on labs on your home computer, don't expect that the code to compile directly on the school computer. It should
* but you might have to make some changes, take this into account in your time plan.<br>
* If you use a laptop feel free to use that one to present the labs on, you don't have to use the machines in the computer lab.
*
* \section first_a First Assignment - 3D transformations
*
* \subsection first_1 Implement TMatrix4 (compulsory)
*      Read through the documentation on the code that has been written, make sure you understand the main part (labb1.cpp) of the program.
*      Start by implementing the following functions in the TMatrix4 object
*                      - TMatrix4::mult(TVertex4);
*                      - TMatrix4::mult(TMatrix4, TMatrix4);
*                      - TMatrix4::mult(Matrix4f);
*                      - TMatrix4::getRotateXInstance(float);
*                      - TMatrix4::getRotateYInstance(float);
*                      - TMatrix4::getRotateZInstance(float);
*                      - TMatrix4::getScaleInstance(float);
*                      - TMatrix4::getScaleInstance(float,float,float);
*
*
* When you are confident that you have implemented these methods the correct way (you may write your own small test programs if you wish) you
* want to compile the code. If you have not made any syntax errors an executable should have been
* built and linked. Execute it by typing <i>./labb1</i>. If you have implemented your methods the correct way you should see an icosahedron that is
* rotating. Now alter the Calculations() method in the labb1.cpp file so you try out all your methods. If all of them work then you
* have completed the first part.
*
*
* \subsection first_2 Implement Animation (compulsory)
*
*      Rewrite the Calculations() method in the labb1.cpp file, so that your object rotates in a circular path in the (x,y) plane.
*  around a centre that you specify (not the x=0, y=0 centre)
*  Try also to make the object bounce around in the window, while spinning, or implement some other animation of you own choice
*      (It doesn't have to follow the physics on earth, but if you like to you could try to implement a physical movement, like a pendulum)
*
* \subsection extra_1 Extra assignment, (not compulsory)
*
*      These assignments are not compulsory but could be fun to do, if you have time over, feel free to do any of them, or get
*  a idea from any of them and implement something of you own. The lab assistant might not be able to help you.
*
*    - Faster sine and cosine<br>
*              To calculate sine and cosine takes a long time, and in a complex scene there can be many objects that you want to move.
*              A way to do this faster is to calculate cosine or sine (or both) one time, store them in a nice datastructure and then
*              get the value from this datastructure (that should be fast to read, but could be slower to write to).
*              You could calculate this table when the program starts or, when you don't have the cosine/sine value that you are after.
*              When you think you have solved it, run the program with your code, and then with cosine/sine and see if yours is faster.
*    - Ply fileformat<br>
*              ply is a very nice file format to store computer graphic geometry in. It was developed at Stanford University and is
*              very commonly used. You can find more information about the file format at http://graphics.stanford.edu/data/3Dscanrep/
*              Here you also can find geometry in the ply format but don't forget to search, there are many ply files available on the internet.
*              -# implement a reader for triangle (you dont have to support other geometry if you don't want to) and vertices from
*                      the ply format.
*                      Feel free to create a new object that inherits from the CMesh object, look at the CIcosahedron.cpp and CIcosahedron.h
*                      to see how they are constructed.
*                      A nice way would be to give the ply file as an argument, when you start the application.<br>
*                      You dont have to reimplement a reader if you dont want to (though it might be useful), you can find many good implementations
*                      on the internet, http://www.cs.princeton.edu/~diego/professional/rply/ is one of them
*    - Path<br>
               Implement a way to assign a path to an object. The path can be specified by some points in the 3D world,
               p1(x,y,z), p2(x,y,z)....pN(x,y,z), the object should then pass through these points.
               One way is to use linearly interpolations between the points, or do a spline fitting,
               spline will give a smoother path but will require more work. You can find more information about
               splines and other interpolations in your 3D Computer Graphics book, or on the internet.

*    - Planetary system<br>
*              Implement a planetary system with several objects rotating in circular or elliptical paths around a common centre.
*              You might have to control the order in which the objects are rendered (do you want to render back to front or front to back?)
*
* \section second Second Assignment - Face normals, back face culling, face colours.
*
* \subsection second_1 Math Implementation (compulsory)
*      Start by implementing the following methods in TVector3
*              - TVector3::cross(TVector3);
*              - TVector3::cross(TVector3, TVector3);
*              - TVector3::dot(TVector3);
*              - TVector3::dot(TVector3, TVector3);
*              - TVector3::normalize();
*
*      Now you have the methods to calculate face normals and therefore also perform face culling
* Implement the following methods in CMesh
*              - CMesh::calculateFaceNormals();
*              - CMesh::calculateFaceVisibility(TVector3);
*
* \subsection second_2 Render Implementation (compulsory)
*
* Now you can do all the maths to speed up your function, therefore you want to implement this functionality into the diffrent drawing methods
* that exist.
*      Implement your face culling into
*      - RenderWireCulled()
*      - RenderFacedCulledSimpleShaded();
*
* Run both of the programs, in RenderFacedCulledSimpleShaded() the program will render triangles where the colour of the face
* depends directly on the face normal. You can then see whether your face normal is correct or not.
*
*  \subsection second_3 Colour Implementation (compulsory)
* Implement the following methods
*
*              - CMesh::randomColors();
*  This method should calculate a random face colour (RGB, use the TColorRGB object), when the animation runs you don't want to recalculate
*      the face colours for every frame.
*
*      In the labb1.cpp implement
*              - RenderFaceCulledColors();
*      Where you use the face colour and send it to the DrawTriangle() method.
*
*
* \section third Third Assignment - TLight reflection model
*
* \subsection third_1 Math Implementation (compulsory)
*
* Implement the following methods in CMesh
*              - CMesh::calculateFaceLighting(TLight);
*              - CMesh::calculateFaceLighting(TLight, TLight);
*
* and following method in TMatrix4
*              - TMatrix4::mult(TVector3);
*
*
* \subsection third_2 Render Implementation (compulsory)
*
* Use now the lighting methods in the Calculations() method.
*      Rewrite the RenderFaceShadedCulled() to both use the culling and the light you have calculated.
*
*
* \subsection extra_3 Extra assignment, (not compulsory)
*
*              -  Specular reflection<br>
*                      Read about specular reflection in your Computer Graphic book. Implement a ball-like object
*                      with considerably more faces than an icosahedron to test your code. If you need help on how
*                      to define the vertices and triangles for a ball, feel free to ask for guidance, but try first
*                      to use for loops and sine and cosine.
*              - Distance to light<br>
*                      Define lighting equations for a point light source with distance attenuation and animate a point
*                      light source near an object. (How would you define the distance from a point to a triangle?)
*              - Overlapping object<br>
*                      When you have two objects that are overlapping, one of them should be fully lit, but the other one have a shadow.
*                      Implement a sorting of the triangles in depth order ( z ) and then calculate the light of those triangles
*                      that are visible from the lightsource. You don't have to cut the triangles to create an accurate shadow, this
*                      is quite hard, but not impossible.
*              - Following light<br>
*                      Implement a way to attach a light source that is orthogonal to the face or vertix. There might be
*                      something usefull in the method TLight::transformDirection(TMatrix4) class already.
*
* \section fourth Fourth Assignment - Perspective
*      First, review the lecture notes about perspective, after that read in your computer graphics book about perspective.
*      Take the time to really understand what you want to to, and how you are going to calculate the perspective.
*
* First we need to find an object that is good to use for perspective calculation, the icosahedron is not one
* of these. You would like a long shaped object so that the perspective effect is clearly visible. Either implement your own
* object to calculate the perspective for or
* use the CTitanic class (this doesn't look like the titanic, it is vaguely similar to a boat). If you wish feel free to alter
* the model to look more like a boat.
*
*      If you have implemented the ply reader in extra assignements, feel free to use any object that is "long shaped" and
*      therefore a good one to do perspective calculations on.
*
* \subsection fourth1 Without perspective (compulsory)
*
*      Start by animating your object in a circular path around the z-axis. Why does the animation look strange?
*      What can be done about it? (To best see the effect you should use the RenderWire() method or renderWirekCulled() )
*
* \subsection fourth2 Implement perspective (compulsory)
*
* Implement first the following methods
*
*              - TVertex4::normalizeW();
*              - CMesh::normalizeTransformedVertices();
*
* Now make your animated wire frame appear in perspective instead. You might need to create a new TMatrix4
* in which to put your perspective matrix.<br>
* Experiment with the perspective parameters to give you a good view.
* What is the distance to your virtual camera? <br>
* What is the field of view? <br>
*
* You are probably now dealing with many diffrent transformations, a good design view could be to separate them
* into several matrices.
*
* When you have the wire frame perspective plot working, use the back face culled version instead to display
* your object. Think about where and how the normal calculation and the culling should be performed.
*
* \subsection fourth3 TLight the scene (compulsory)
*
* Last but not least, add light to your scene. Think about how the lighting, culling and perspective transformations
* should be made to work together. You will need to transform the vertex data in separate step. Where should
* the face normals be calculated?
*
* \subsection extra4 Extra assignment, (not compulsory)
*
*              - Shadows<br>
*                We have a nice scene, animations, light and perspective, but there is one major part missing: shadows.<br>
*                Start by reading about shadows in your computer grahics book, if you find a nice method you would like
*                to implement pick that one. Otherwise you can treat the shadows as black, flattened out versions of the object.
*                Start by implementing a plane in your scene (a square can be described as two triangles).
*              - Camera<br>
*                Implement a convenient method in CMesh to set up the perspective and view transformations by positioning
*                a virtual camera. Both transformations could be calculated by specifying the position of the camera, a point
*                to look at, a vector that defines "up" for the camera, and the desired field of view for the virtual camera.
*                Use your method to animate a camera along a straight line (or if you have implemented the extra assigments of the path
*                in part one, use that) past an object while keeping the object in view.
*
* \section Fifth Fifth Assignment - Scan-Line Conversion
*
* So far you have been working with the mathematics behind 3D graphics, such as vertex list,
* triangle list, and methods to alter them such as scaling, translation and perspective.
* You have also done some basic operation when calculating colors and lighting. But so far we have been using
* openGL to draw the result onto the screen. The next assignment can be divided into two parts, scan-line
* conversion and shaders.<br>
* The first part will be to implement a scan-line converter that convert you mathematical representation
* into an image that can be saved, send to a customer or used in post productions. <br>
* The second part will be to implement two different shaders that looks more realistic then the
* face shading that we have covered earlier. <br>
* <b>
* This exercise will be a little bit different then the other once. You are expected to find the information
* that you need and to implement it. You will find all information that you need in your lecture notes and in
* the course text book, but you may also get information from other sources, such as other text books or on the internet.</b>
*
* When you show your results to lab assistant, you will be expected to specify what you have done,
* explain methods and equations, but also from witch sources you got the information (keep a reference list).
*
* \subsection fifht1 Image format
*
* Before you can start rendering images using a scan-line conversion method you have to been able
* to save the result. The first part of this lab session is to learn/implement an image writer of
* you choice. Remember that you need to view/show your images later, so either you have to write
* your own viewer or use an standard image format (gif, jpeg, png…). In the lab there exist Gimp
* that can view more or less any image format, but there exist a lot of software that can show images.
* Here follows some information about image format that is common in the research community, but you
* may use any image format you like.
* <p>
* - Flexible Image Transport System (fit)<br>
*	This is a very excellent data format. It were developed to store any type
* of astronomic data.
* It is  endorsed by NASA and the
* International Astronomical Union. The format is really easy to use, you can
* find more information about it at
* http://fits.gsfc.nasa.gov/. If you don't want to implement one
* yourself there exist a huge number of implementations on the specified homepage.
*
*<P>
*	- Portable Network Graphics (PNG) <br>
* The portable network graphics is an excellent image format.
* It is quite easy to implement, but there exist
* many open source image libraries that support png, libpng is probably the
* most known and the most used. You can find more information about the
* png format at, http://www.libpng.org/ here you also can find
* the highly recommended libpng, http://www.libpng.org/pub/png/libpng.html <br>
* You don't have to use the libpng, you can implement you own png writer if you
* like to.<br>
*
* <p>
* - Other formats <br>
* You don't have to use the png or fit file format, feel free to us any image
* format that you like.
*
* You don't have to show this exercise to the lab assistance, just make sure
* that you understand the image format that you have chosen, and that you
* can create an image, set different pixel values ( with different
* colours) and save the image, make sure that you can view a stored image
*
* \subsection fifht2 Scan-Line Conversion (compulsory)
*
* There exist a many different scan-line convert methods, in this exercise you
* are expected to implement one.
* To complete this task all information you need exist in the lecture notes,
* but you might also want to read about scan-line converter in you text book.
* If you search information in other text books or on the internet remember that
* there exist some really advanced methods, you don't have to choose one of theses
* advanced methods, but feel free to do so if you find the topic interesting.
* Remember that if you do so, the lab assistance might not be able to help you.<br>
* Add a method in the RenderScene() method in the labb1.cpp file that using your scan-line method.
* It might be a good idée to first implement the scan-line converter so it only
* detects if you are inside or outside an face, draw a black pixel if you are
* outside and a white pixel if you are inside, this make it easy to debug your algorithm.
* When this work you can also start using the colours and face normal
* to render colours to the different faces.
* When you have a working scan-line converter that are using the colour and face
* normal you have finished the first part of this lab. Render some images and save them.
*
* \subsection fifht3 Phong shading (compulsory)
*
* You have now a working scan-line converter, so the next two exercises will deal
* with two type of shaders, the first shader will be a phong shader that will produce
* more realistic images then the face shading that have been used before.
* The phong shader uses the vertex normal, but interpolates them to get a
* normal for each pixel, and uses this pixel normal to calculate the shading.
* Do not use the surface normal, this will create an "flat" shading and will note
* create a phong shaded surface. Start by reading about phong shaders in you lecture
* notes and in your text book, and then implement the shader. <br>
* When your phong shader is working render some images and save.
*
* \subsection fifht4 Bidirectional Reflectance Distribution Function (compulsory)
*
* This is the final compulsory exercise, and you are supposed to implement a shading
* technology called bidirectional reflectance distribution function, or brdf. This
* method has the ability to create realistic results, and have been used in
* numerous movie productions, The Matrix is one of them.
* The shader is using a distribution function to simulate the real environment,
* but it is not difficult to implement and it don't give a huge memory or time footprint.
* Start by reading you lecture notes and your text book about brdf, but this might not
* be enough. You have to find the necessary data, or information that you need to have
* to complete the task. When you have implemented brdf and it creates nice result,
* tell the lab assistance what type of material that you try to render, and if you have
* used any information from another source, and where you found it.<br>
* <p>
* Congratulation you have now completed the lab course.
*
*
*/
using namespace std;  // no need for std::cout etc.

#endif
