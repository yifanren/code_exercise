rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= tcpServerClient
INCLUDES=  -I.
LIBS=


DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= -g $(DEFINES)
LIBS+= -lpthread

SRCS = 05_thread.cpp 

OBJS = 05_thread.o 

.cpp.o:
    $(rm) $@
    $(CC) $(CFLAGS) -c $*.cpp

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
    $(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
    $(rm) $(OBJS) $(PROGNAME) core *~














