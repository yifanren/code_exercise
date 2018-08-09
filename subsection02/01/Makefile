OBJS     = AsciiSort.o
CFLAGS  := -g 
LIBS    :=
PROGRAM := AsciiSort

all: $(PROGRAM)

$(PROGRAM) : $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

%.o:%.c
	$(CXX) $(CFLAGS) -c $< -o $@ 

clean:
	-rm -f $(OBJS) $(PROGRAM)
