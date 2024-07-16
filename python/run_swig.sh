swig -v -c++ -python -o ../src/python/HMSPython.cc -I../src ../src/python/hms.i

# Add default constructors for SwigDirectors created by swig to allow
# for boost serialization of these classes 

sed -i.bak '/SwigDirector_Argument(PyObject*/i \ 
SwigDirector_Argument() : Swig::Director(NULL) {};
' ../src/python/HMSPython.h

sed -i.bak '/SwigDirector_InputFilter(PyObject*/i \ 
SwigDirector_InputFilter() : Swig::Director(NULL) {};
' ../src/python/HMSPython.h

sed -i.bak '/SwigDirector_OutputFilter(PyObject*/i \ 
SwigDirector_OutputFilter() : Swig::Director(NULL) {};
' ../src/python/HMSPython.h

sed -i.bak '/SwigDirector_Value(PyObject*/i \ 
SwigDirector_Value() : Swig::Director(NULL) {};
' ../src/python/HMSPython.h

# Change swig_self stored in the Director base class to public so that
# it can be assigned when deserializing objects

sed -i.bak '/PyObject \*swig_self;/i \ 
public:
' ../src/python/HMSPython.cc

# Update DirectorException to output Python errors to stderr to allow
# debugging in user-implemented InputFilter and OutputFilter
sed -i.bak '/throw DirectorMethodException(msg);/i \
PyErr_Print();
' ../src/python/HMSPython.cc

clang++ -shared ../src/python/HMSPython.cc -I/usr/include/python2.7 -I../src -DHAVE_CONFIG_H -I. -I/opt/local/include/ -L/usr/lib -lpython -lhmsbroker -lhmsexception -lhmscomm -lhmsbase -L/opt/local/lib -lboost_serialization-mt -o _hms.so
cp ../src/python/hms.py .
