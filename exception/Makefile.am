ACLOCAL_AMFLAGS  = -I config 

lib_LTLIBRARIES = libhmsexception.la

hmsexceptionincludedir = ${includedir}/hms/exception

hmsexceptioninclude_HEADERS = ArithmeticError.h                                \
                              CommandError.h                                   \
                              ConnectionTerminationException.h                 \
                              DomainError.h                                    \
                              Exception.h                                      \
                              Exception_t.cc                                   \
                              IOError.h                                        \
                              InvalidArgument.h                                \
                              InvalidPointer.h                                 \
                              LengthError.h                                    \
                              LogicError.h                                     \
                              MemoryError.h                                    \
                              NetworkError.h                                   \
                              NotImplemented.h                                 \
                              NullPointer.h                                    \
                              OSError.h                                        \
                              OutOfRangeError.h                                \
                              RangeError.h                                     \
                              RuntimeError.h

libhmsexception_la_SOURCES = ArithmeticError.cc                                \
                             CommandError.cc                                   \
                             ConnectionTerminationException.cc                 \
                             DomainError.cc                                    \
                             Exception.cc                                      \
                             IOError.cc                                        \
                             InvalidArgument.cc                                \
                             InvalidPointer.cc                                 \
                             LengthError.cc                                    \
                             LogicError.cc                                     \
                             MemoryError.cc                                    \
                             NetworkError.cc                                   \
                             NotImplemented.cc                                 \
                             NullPointer.cc                                    \
                             OSError.cc                                        \
                             OutOfRangeError.cc                                \
                             RangeError.cc                                     \
                             RuntimeError.cc

if COMPILE_MPI

libhmsexception_la_LDFLAGS = ${MPILIBS}

endif
