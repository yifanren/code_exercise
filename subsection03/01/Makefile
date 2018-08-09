object  := counter.o

PROGRAM := counter
CFLAGS  := -g 

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ 

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@ 

clean :
	-rm -f $(object) $(PROGRAM)
