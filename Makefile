
LANG:=C
OUTPUT:=run
LIBS:=


ifeq "$(LANG)" "C++"
	EXT:=cpp
	STD:=c++11
	CC:=$(CXX)
else ifeq "$(LANG)" "C"
	EXT:=c
	STD:=c99
	CC:=$(CC)
else
	$(error Invalid language specified)	
endif

SRC:=$(shell find src -name *.${EXT})
OBJ:=$(SRC:src/%.c=obj/%.o)
DEP:=$(OBJ:%.o=%.d)

CFLAGS:= -std=$(STD) $(LIBS)

-include $(DEP)

build : compile remove_unused_objects

rebuild : clean build
	
compile : $(OBJ) 
	$(CC) $^ -o $(OUTPUT) $(CFLAGS)	

obj/%.o : src/%.$(EXT) 
	@mkdir -p $(@D) # $(@D) <- Gets directory part of target path
	$(CC) $< -o $@ $(CFLAGS) -c -MMD -MP

FILES_IN_OBJ = $(shell find obj -name *.o)

remove_unused_objects :
ifneq '' '$(filter-out $(OBJ), $(FILES_IN_OBJ))' # finds out which object files no longer have an associated cpp file
	rm -r $(filter-out $(OBJ), $(FILES_IN_OBJ))
else
	@echo "No object files require deletion."
endif
	
clean : 
	rm -r obj
	rm $(shell find . -name $(OUTPUT)*)
	@echo "Cleaned out object files and binaries"

debug :
	@echo Language Selected: $(LANG)
	@echo Compiler: $(CC)
	@echo Standard Library: $(STD)
	@echo Libraries: $(LIBS)
	@echo
	@echo Binary Name: $(OUTPUT)
	@echo Source Files: $(SRC) 
	@echo Object Files: $(OBJ)
	@echo Dependencies: $(DEP)
	@echo All files in Object folder: $(FILES_IN_OBJ)
	@echo
