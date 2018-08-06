object = 01_ReadFile.o

PROGRAM := 01_ReadFile

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
