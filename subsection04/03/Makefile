object = 03_MulConnect.o  

PROGRAM := 03_MulConnect

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ -lpthread

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
