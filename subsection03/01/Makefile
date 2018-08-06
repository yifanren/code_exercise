object = 01_CountFileSize.o

PROGRAM := 01_CountFileSize

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ 

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
