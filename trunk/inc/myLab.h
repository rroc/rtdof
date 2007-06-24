#ifndef PROJECT_DOF_H
#define PROJECT_DOF_H

//ALL GLOBAL DEFINES
#define USE_SIN_TABLE

//#define USE_FPS_LIMIT
#define USE_OPENGL_RENDERER
//#define USE_VERTEX_ARRAYS
#define USE_SYMBIAN_STYLE_CONSTANTS
#define USE_FBO
#define USE_SHADER

//OPENGL uses degrees...
#ifdef USE_OPENGL_RENDERER
# define USE_DEGREES
#else
# define USE_RADIANS
#endif








/*! \mainpage 3d graphics engine for school project
* 3dEngine © 2006 Johannes Rajala
* johra470@student.liu.se
*
*  \ref hist_sec
*  \ref naming_sec
*		\ref naming_class_sec
*		\ref naming_struct_sec
*		\ref naming_var_sec
*		\ref naming_func_sec
*		\ref naming_macro_sec
*		\ref naming_enum_sec
*		\ref naming_const_sec
*
* <p>
* <br>
*
* \section hist_sec Version History
*
*
* The course started 20.3.2006 (the first lab 28.3.2006) <br>
* Here's the release history:
*
* - 1.71 - basic object depth ordering - 04.05.2006 - johannes rajala
*
* - 1.7 - Perspective for the software renderer - 02.05.2006 - johannes rajala
*
* - 1.6 - Simple 3DS-mesh loader - 30.04.2006 - johannes rajala
*
* - 1.52 - Seperate OPENGL renderer, also the degrees and radians are different - 29.04.2006 - johannes rajala
*
* - 1.5 - Added lightning calculations for ambient, diffuse and specular lights- 24.04.2006 - johannes rajala
*
* - 1.4 - Added normal's drawing, and OpenGL translations - 20.04.2006 - johannes rajala
*
* - 1.35 - Added a ball object - 01.04.2006 - johannes rajala
*
* - 1.3 - Flat shading and random colors(with intensity) - 30.03.2006 - johannes rajala
*
* - 1.2 - Rotation around a point - 28.03.2006 - johannes rajala
*	- User can give the point of rotation, and the location of the object by using mouse
*
* - 1.1 - Optimizing - 25.03.2006 - johannes rajala
*	- Added a possibility to use a sinus-table instead of calculating it in realtime
*
* - 1.0 - The first working version - 25.03.2006 - johannes rajala
*	- Added matrix calculations
*	- Wireframe rendering works
*
* - 0.7 - fixing the original - 24.03.2006 - johannes rajala
*	- Changed the thing to c++
*	- corrected functions
*	- applied coding conventions
*
*
* \section naming_sec Naming Conventions
*
* I have decided to use the naming conventions from Symbian to make the code more manageable.
* This also allows easy porting to other platforms as well. Here is a description from Symbian documentation.
*<br>
*
* \subsection naming_class_sec Class names
*
* Most class names are formed with a prefix letter C, T, R, or M. Briefly, the meaning of these is as follows:
* - C: heap-allocated classes, that are derived from a base class CBase
* - T: value classes, that do not own any external object
* - R: resource classes, that contain handles to a real resource which is maintained elsewhere
* - M: interface classes, that define abstract protocol definitions that are implemented by derived classes
*
* Classes that consist solely of static member functions have no prefix letter. Beyond the prefix, the class name is usually a noun that indicates the purpose of the class.
*
*<br>
* \subsection naming_struct_sec Struct names
*
* Structure types are considered as similar to T classes, as they should not own external objects, and are normally given names beginning with T (although some begin with S).
*
*<br>
* \subsection naming_var_sec Variable names
*
* Member variables’ names begin with i, e.g. iMember. This makes it easy to check that certain cleanup-related rules are being obeyed. Arguments’ names begin with a, e.g. aControl or aIndex. Local variables’ names have no initial letter. Global variables are usually avoided, but when used, their names begin with a capital letter.
*
* Symbian does not use Hungarian or any notation which attempts to include the variable type in its name: such notations are ugly, and become impossible to manage when there are several hundred classes in the system. They are irrelevant anyway: functions are usually so short that it is easy to see the types of variables defined in them, and class browsers provide a quick way to find the types of class members.
*
*<br>
* \subsection naming_func_sec Function names
*
* Functions’ names indicate what they do. They are usually verbs. One exception is “getter” functions: for a function which returns the value of a member variable, the function name is usually the name of the variable, without the leading i:
*
\code
inline RWindow& Window() const { return iWindow; };
\endcode
* A corresponding “setter” function would include the word Set, e.g. SetWindow().
*
*<br>
* \subsection naming_macro_sec Macro names
*
* Macro names are all capitalised, with underscores to separates words.
*
*<br>
* \subsection naming_enum_sec Enumeration names
*
* Enumerations are named as follows:
*
* - as enumerations are types, they have the T prefix
* - enumeration members have the prefix E
* - type and members should have a meaningful, unambiguous name
* - Enumerations should be scoped within the relevant class, so as not to pollute the global name space.
*
* An example of the declaration and use of an enumeration is as follows:
*
\code
class TDemo
	{
	public:
		enum TShape {EShapeRound, EShapeSquare};
	};

	TDemo::TShape shape=TDemo::EShapeSquare;
\endcode
*
*<br>
* \subsection naming_const_sec Constant names
*
* Names of constants have a prefix K. For example,
\code
const TInt KMaxNameLength=0x20;
\endcode
*
*
*
*/

#endif
