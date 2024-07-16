%module(directors="1") hms
%{
    #include <base/Argument.h>
    #include <base/InputFilter.h>
    #include <base/Model.h>
    #include <base/OutputFilter.h>
    #include <base/ResourceType.h>
    #include <base/Value.h>

    #include <base/ModelPackage.h>

    #include <comm/Communicator.h>
    #include <broker/BrokerLauncher.h>

    #include <exception/NetworkError.h>
  
    using namespace arl::hms;
%}

// Macro to check if class can be downcast to a SwigDirector. If so,
// it pulls the PyObject stored in the director and returns it
// directly to Python
#define HMS_OUTPUT_TYPEMAP(Class)                                              \
namespace arl {                                                                \
  namespace hms {                                                              \
    %typemap(out) const Class##Pointer & {                                     \
      if(boost::shared_ptr<SwigDirector_##Class> director =                    \
        boost::dynamic_pointer_cast<SwigDirector_##Class>(*$1)) {              \
        $result = director->swig_get_self();                                   \
      }                                                                        \
      else {                                                                   \
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_boost__shared_ptrT_arl__hms__##Class##_t, SWIG_POINTER_OWN);                              \
      }                                                                        \
    }                                                                          \
  }                                                                            \
}

HMS_OUTPUT_TYPEMAP(Argument);
HMS_OUTPUT_TYPEMAP(InputFilter);
HMS_OUTPUT_TYPEMAP(OutputFilter);
HMS_OUTPUT_TYPEMAP(Value);

//
// The Argument is passed as an input to InputFilter and OutputFilter
// apply(). This typemap extracts the Argument PyObject from the
// director so that it can be used directly inside the apply() methods
// (rather than passing the hms.Argument base class)
//
%typemap(directorin) arl::hms::ArgumentPointer const & {
  if(boost::shared_ptr<SwigDirector_Argument> director =
     boost::dynamic_pointer_cast<SwigDirector_Argument>($1)) {
    $input = director->swig_get_self();
    Py_INCREF($input);
  }
  else {
    $input = SWIG_NewPointerObj(SWIG_as_voidptr(&$1), SWIGTYPE_p_boost__shared_ptrT_arl__hms__Argument_t, 0);
  }
}

//
// The OutputFilter will create a Value to return to Python. Rather
// than returning the hms.Value base class we stuff the Value PyObject
// inside a director so that the specific subclass can be accessed
// later.
//
%typemap(directorout) ValuePointer {
  Py_INCREF($1);
  $result = ValuePointer(new SwigDirector_Value($1));
}

%include boost_shared_ptr.i

%include std_string.i
%include std_vector.i

// Shared Pointer Templates
%shared_ptr(arl::hms::Argument);
%shared_ptr(arl::hms::InputFilter);
%shared_ptr(arl::hms::OutputFilter);
%shared_ptr(arl::hms::Value);

%shared_ptr(arl::hms::Model);

%shared_ptr(arl::hms::ModelPackage);

%shared_ptr(arl::hms::Communicator);

// Enable cross-language polymorphism
// (allow subclasses to be created in Python)
%feature("director") arl::hms::Argument;
%feature("director") arl::hms::Value;
%feature("director") arl::hms::InputFilter;
%feature("director") arl::hms::OutputFilter;

%include arlhms_config.h
%include utils/HMSMacros.h

%include base/ResourceType.h

%include base/Argument.h
%include base/InputFilter.h
%include base/Model.h
%include base/OutputFilter.h
%include base/Value.h

%include base/ModelPackage.h
  
%include comm/Communicator.h
%include comm/Launcher.h
  
%include broker/BrokerLauncher.h

%template(CommunicatorVector) std::vector<arl::hms::CommunicatorPointer>;
%template(StringVector) std::vector<std::string>;
%template(ModelPackageVector) std::vector<arl::hms::ModelPackagePointer>;

%template(send) arl::hms::Communicator::send<ModelPackagePointer>;
%template(receive) arl::hms::Communicator::receive<ModelPackagePointer>;

%{
  //
  // Allow serialization of director classes that sit at the bottom of
  // the inheritance chain to allow for cross language polymorphism
  //
  #include <utils/HMSMacros.h>
  #include "HMSPython.h"
  HMS_SERIALIZATION_EXPORT(SwigDirector_InputFilter);
  HMS_SERIALIZATION_EXPORT(SwigDirector_OutputFilter);
  HMS_SERIALIZATION_EXPORT(SwigDirector_Argument);
  HMS_SERIALIZATION_EXPORT(SwigDirector_Value);
  
%}

%header %{
namespace {

  //
  // generic save / load to handle serialization of Swig::Director classes
  // 
  template <class Archive>
  void saveDirector(const Swig::Director * object,
                    Archive & archive,
                    const unsigned int version)
  {

    static PyObject *module = NULL;
    
    if (module == NULL &&
        (module = PyImport_ImportModuleNoBlock("pickle")) == NULL) {
      // throw exception here
      PyErr_Print();
      return;
    }
  
    //
    // serialize the PyObject held by director using pickle
    //
    PyObject * swig_self = object->swig_get_self();

    //std::cerr << std::endl;
    //PyObject_Print(swig_self, stderr, Py_PRINT_RAW);

    PyObject *pickle;
    pickle = PyObject_CallMethodObjArgs(module,
                                        PyString_FromString("dumps"),
                                        swig_self,
                                        NULL);
    if(pickle == NULL) {
      PyErr_Print();
      throw arl::hms::NetworkError("Error calling pickle.dumps() when "
				   "serializing Python-implemented HMS class");
      return;
    }

    Py_ssize_t length = PyBytes_Size(pickle);
    std::vector<char> pickleString(length);
    char * pickleChar = PyBytes_AsString(pickle);
    std::copy(pickleChar,
              pickleChar + length,
              pickleString.begin());
    archive & pickleString;

    Py_DECREF(pickle);
    
  }

  template <class Archive>
  void loadDirector(Swig::Director * object,
                    Archive & archive,
                    const unsigned int version)
  {
      
    std::vector<char> pickledObject;
    archive & pickledObject;
      
    static PyObject *module = NULL;
    
    if(!Py_IsInitialized()) {
      Py_Initialize();
    }
    
    if (module == NULL &&
        (module = PyImport_ImportModuleNoBlock("pickle")) == NULL) {
      // throw exception here
      PyErr_Print();
      exit(-1);
    }
    
    PyObject * swig_self =
      PyObject_CallMethodObjArgs(module,
                                 PyString_FromString("loads"),
                                 PyBytes_FromStringAndSize(&(pickledObject[0]),
							   pickledObject.size()),
                                 NULL);
    
    if(swig_self == NULL) {
      PyErr_Print();
      throw arl::hms::NetworkError("Error calling pickle.loads() when "
				   "deserializing a Python-implemented HMS "
				   "class");
      return;
    }

    object->swig_self = swig_self;
    
  }
  
}

#define HMS_PYTHON_SERIALIZATION_EXPORT(Class)                                \
BOOST_SERIALIZATION_SPLIT_FREE(SwigDirector_##Class)                          \
namespace boost {                                                             \
  namespace serialization {                                                   \
    template <class Archive>                                                  \
    void save(Archive & archive,                                              \
              const SwigDirector_##Class & a,                                 \
              const unsigned int version)                                     \
    {                                                                         \
      archive & boost::serialization::base_object<Class>(a);                  \
      saveDirector(&a, archive, version);                                     \
      return;                                                                 \
    }                                                                         \
    template <class Archive>                                                  \
    void load(Archive & archive,                                              \
              SwigDirector_##Class & a,                                       \
              const unsigned int version)                                     \
    {                                                                         \
      archive & boost::serialization::base_object<Class>(a);                  \
      loadDirector(&a, archive, version);                                     \
      return;                                                                 \
    }                                                                         \
  }                                                                           \
} 

HMS_PYTHON_SERIALIZATION_EXPORT(Argument);
HMS_PYTHON_SERIALIZATION_EXPORT(InputFilter);
HMS_PYTHON_SERIALIZATION_EXPORT(OutputFilter);
HMS_PYTHON_SERIALIZATION_EXPORT(Value);
 
%}
