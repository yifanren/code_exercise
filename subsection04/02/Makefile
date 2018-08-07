object = ImitateServerClient.o  

PROGRAM := ImitateServerClient

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ -lpthread

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
