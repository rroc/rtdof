
CXX = gcc #c++
CXXFLAGS = -Iinc -O2
LDFLAGS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lX11 -lm 
SRCDIR = src/
INCDIR = inc/
OUTDIR = bin/unix/

GOVIEWOBJS = \
	myLab.o \
	CMyRenderer.o \
	CMyUiEvents.o \
	TTriangle.o \
	TVector3.o \
	TAngles.o \
	TMatrix4.o \
	TColorRGB.o \
	CIcosahedron.o \
	TLight.o \
	CTrigTable.o \
	CMesh.o \
	CTitanic.o \
	CBall.o \
	TMeshLoader.o \

PROGS = myLab

all:
	@echo "This make file support many diffrent functions."
	@echo " make myLab -> compiles the myLab executable"
	@echo " make clean -> remove all objects and executable"
	@echo " make superclean -> remove all objects, executable and the doc directory"
	@echo " make doc -> create the documentation in the doc directory"

clean:
	rm -rf $(PROGS) *.o *~

superclean:
	rm -rf $(PROGS) *.o *~
	rm -rf doc

doc:
	doxygen Doxyfile

TAngles.o: \
	$(SRCDIR)TAngles.cpp \
#	TAngles.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

TTriangle.o: \
	$(SRCDIR)TTriangle.cpp \
#	TTriangle.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

TVector3.o: \
	$(INCDIR)TVector3.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp	

TMatrix4.o: \
	$(SRCDIR)TMatrix4.cpp \
#	TMatrix4.h TTriangle.h TVector3.h CTrigTable.h 
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp
	
CTrigTable.o: \
	$(SRCDIR)CTrigTable.cpp \
#	CTrigTable.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

TColorRGB.o: \
	$(SRCDIR)TColorRGB.cpp \
#	TColorRGB.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

TLight.o: \
	$(SRCDIR)TLight.cpp \
#	TLight.h TVector3.h TColorRGB.h TMatrix4.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp
	
CMesh.o: \
	$(SRCDIR)CMesh.cpp \
#	CMesh.h TTriangle.h TVector3.h TMatrix4.h TAngles.h TColorRGB.h TLight.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp
		
CIcosahedron.o: \
	$(SRCDIR)CIcosahedron.cpp \
#	CIcosahedron.h  TTriangle.h TVector3.h TMatrix4.h CMesh.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

CTitanic.o: \
	$(SRCDIR)CTitanic.cpp \
#	CTitanic.h TTriangle.h TVector3.h TMatrix4.h CMesh.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

CBall.o: \
	$(SRCDIR)CBall.cpp \
#	CBall.h TTriangle.h TVector3.h TMatrix4.h CMesh.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp
		
CMyRenderer.o: \
	$(SRCDIR)CMyRenderer.cpp \
#	TAngles.h CTrigTable.h TTriangle.h TVector3.h TMatrix4.h TColorRGB.h CIcosahedron.h TLight.h CMesh.h CTitanic.h CBall.h 
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

CMyUiEvents.o: \
	$(SRCDIR)CMyUiEvents.cpp \
#	CMyUiEvents.h CMyRenderer.h TAngles.h
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

TMeshLoader.o: \
	$(SRCDIR)TMeshLoader.cpp 
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp
	
# goview
myLab: $(GOVIEWOBJS)
	$(CXX) -o $@ $(GOVIEWOBJS) $(LDFLAGS)

myLab.o: \
	$(SRCDIR)myLab.cpp
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

# Generic rules

.cc.o: $<
	$(CXX) -c $(CXXFLAGS) $(SRCDIR)$*.cpp

.cc: $<
	$(CXX) $(CXXFLAGS) -o $(OUTDIR)$@ $(SRCDIR)$*.cpp $(LDFLAGS)
