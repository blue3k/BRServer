

#-----------------------------------------------------------------
# inputs

INCLUDES?=.
DEFINES?=

# Linking order affects symbol search result. funny....
LIBS=libServerSystem.a libNet.a libProtocol.a libTable.a libDB.a libCommon.a


export LIBS
