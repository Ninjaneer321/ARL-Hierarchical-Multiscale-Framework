//
// File:      HMSMacros.h
// Package    base
//
// HMS.
//
#if !defined(HMSMacros_h)
#define HMSMacros_h


#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

#include <boost/shared_ptr.hpp>

/**
 * Macro to export hms objects for serialization (to enable
 * polymorphic serialization)
 */
#define HMS_SERIALIZATION_EXPORT(Class)                                        \
BOOST_CLASS_EXPORT(Class)

/**
 * Macro to define hms shared pointer objects.
 */
#define HMS_SHARED_PTR(Class)                                                  \
typedef boost::shared_ptr<Class> Class##Pointer;                               \
typedef boost::shared_ptr<const Class> Const##Class##Pointer;

#endif // HMSMacros.h
