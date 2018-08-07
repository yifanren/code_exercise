PROGNAME= SelfAdd
OBJS= SelfAdd.o
CC= g++
LIBS= -lpthread
CFLAGS= -std=c++11

$(PROGNAME) : $(OBJS)
	$(CC) $(OBJS) -o $(PROGNAME) $(LIBS)

$(OBJS) :
	$(CC) -c $*.cpp


clean:
	rm $(OBJS) $(PROGNAME) 



