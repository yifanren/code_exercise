object = GetNetworkInfo.o  

PROGRAM := GetNetworkInfo

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm -f $(object) $(PROGRAM)
