ACLOCAL_AMFLAGS  = -I config
AM_CPPFLAGS = -I${top_srcdir}

lib_LTLIBRARIES = libhmsbase.la

hmsbaseincludedir = ${includedir}/hms/base

hmsbaseinclude_HEADERS = Argument.h                                            \
                         Argument_t.cc                                         \
                         BatchQueueSettings.h                                  \
                         BatchQueueSettings_t.cc                               \
                         Command.h                                             \
                         CommandFactory.h                                      \
                         CommandStatus.h                                       \
                         CommandType.h                                         \
                         InputFilter.h                                         \
                         InputFilter_t.cc                                      \
                         LocalhostResourceListGenerator.h                      \
                         Logger.h                                              \
                         MPICommand.h                                          \
                         MPIMachinefileGenerator.h                             \
                         MPIResourceListGenerator.h                            \
                         MachinefileGenerator.h                                \
                         Model.h                                               \
                         Model_t.cc                                            \
                         ModelPackage.h                                        \
                         ModelPackage_t.cc                                     \
			 NumberProcessorsPerNode.h                             \
                         OutputFilter.h                                        \
                         OutputFilter_t.cc                                     \
                         PBSCommand.h                                          \
                         Priority.h                                            \
                         Queue.h                                               \
                         Queue_t.cc                                            \
                         ResourceList.h                                        \
                         ResourceListGenerator.h                               \
                         ResourceManager.h                                     \
                         ResourceType.h                                        \
                         ResourceType.I                                        \
                         StateDB.h                                             \
                         TimeUtils.h                                           \
                         Value.h                                               \
                         Value_t.cc

hmsbasepredicateincludedir = ${includedir}/hms/base/predicate

hmsbasepredicateinclude_HEADERS = predicate/EndPredicate.h                     \
                                  predicate/EndPredicate_t.cc                  \
                                  predicate/StatusPredicate.h                  \
                                  predicate/StatusPredicate_t.cc               \
                                  predicate/TimeoutStatusPredicate.h           \
                                  predicate/TimeoutStatusPredicate_t.cc

libhmsbase_la_SOURCES = Argument.cc                                            \
                        BatchQueueSettings.cc                                  \
                        Command.cc                                             \
                        CommandFactory.cc                                      \
                        InputFilter.cc                                         \
                        LocalhostResourceListGenerator.cc                      \
                        Logger.cc                                              \
                        MachinefileGenerator.cc                                \
                        Model.cc                                               \
                        ModelPackage.cc                                        \
                        MPIMachinefileGenerator.cc                             \
                        MPIResourceListGenerator.cc                            \
                        MPICommand.cc                                          \
			NumberProcessorsPerNode.cc                             \
                        OutputFilter.cc                                        \
                        PBSCommand.cc                                          \
                        ResourceList.cc                                        \
                        ResourceListGenerator.cc                               \
                        ResourceManager.cc                                     \
                        StateDB.cc                                             \
                        TimeUtils.cc                                           \
                        Value.cc                                               \
                        predicate/EndPredicate.cc                              \
                        predicate/StatusPredicate.cc                           \
                        predicate/TimeoutStatusPredicate.cc

libhmsbase_la_LDFLAGS = ${top_builddir}/exception/libhmsexception.la           \
                        ${BOOST_LDFLAGS}

if COMPILE_MPI

libhmsbase_la_LDFLAGS += ${MPILIBS}

endif
