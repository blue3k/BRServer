

#-----------------------------------------------------------------
# inputs

INCLUDES?=.
DEFINES?=

# Linking order affects symbol search result. funny....
LIBS=ServerSystem.a Net.a Protocol.a Table.a DB.a Common.a


export LIBS