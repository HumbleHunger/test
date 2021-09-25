TAGET:=bin/ls.out
OBJ:=obj/display_attribute.o obj/display_dir.o obj/display_single.o obj/display.o obj/display_recur.o obj/display_one.o 
OBJ+=src/my_ls.c
F:=-Wall -g
CC:=gcc

$(TAGET):$(OBJ)
	$(CC) -o $@ -Iinclude $^ $F
obj/%.o:src/%.c
	$(CC) -o $@ -Iinclude -c $^ $F
.PHONEY:clean
clean:
	rm -rf $(TARGET)
	rm -rf obj/*.o
	rm -rf *~ *.*~
	rm -rf include/*.*~
	rm -rf src/*.*~
