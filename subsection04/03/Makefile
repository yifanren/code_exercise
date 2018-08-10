object = TcpConnect.o  

PROGRAM := TcpConnect

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^ -lpthread

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm -f $(object) $(PROGRAM)
