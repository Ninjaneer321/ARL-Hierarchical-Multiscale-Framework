#
#
#
ACLOCAL_AMFLAGS  = -I config
SUBDIRS          = exception base monitor scheduler comm broker                \
                   adaptive_sampling python

hmsutilsincludedir = ${includedir}/hms/utils
hmsutilsinclude_HEADERS = utils/HMSMacros.h                                    \
                          utils/HMSPaths.h

hmswarehouseincludedir = ${includedir}/hms/warehouse
hmswarehouseinclude_HEADERS = warehouse/ModelPackageWarehouse.h                \
                              warehouse/Warehouse.h                            \
                              warehouse/Warehouse_t.cc

hmsconfigincludedir = ${includedir}/hms
hmsconfiginclude_HEADERS = arlhms_config.h

bin_SCRIPTS = config/hms-config
