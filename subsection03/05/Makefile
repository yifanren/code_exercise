PROGNAME = SelfAdd
OBJS     = SelfAdd.o
LIBS     = -lpthread
CFLAGS   = -std=c++11 
CC       = g++

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJS) $(PROGNAME) 



