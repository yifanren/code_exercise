object = CountFileSize.o

PROGRAM := CountFileSize

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ 

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
