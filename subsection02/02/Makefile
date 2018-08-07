object = PinyinSort.o

PROGRAM := PinyinSort

all: $(PROGRAM)

$(PROGRAM) : $(object)
	gcc -o $@  $^

%.o:%.c
	gcc -c $< -o $@ 

clean :
	-rm $(object) $(PROGRAM)
