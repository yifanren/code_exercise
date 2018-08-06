object = 02_ImitateServerClient.o  

PROGRAM := 02_ImitateServerClient

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ -lpthread

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
