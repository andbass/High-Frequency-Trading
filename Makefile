LANG:=C
OUTPUT:=hft
LIBS:=
FLAGS:= -g

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
OBJ:=$(SRC:src/%.${EXT}=obj/%.o)
DEP:=$(OBJ:%.o=%.d)

CFLAGS:= -std=$(STD) $(LIBS) $(FLAGS) 
SHELL := /bin/bash
INSTALL_DIR := /usr/local/bin

build : compile remove_unused_objects

rebuild : clean build

install : build
	@install ./$(OUTPUT) $(INSTALL_DIR)
	@echo Install complete!

uninstall :
	-@rm $(INSTALL_DIR)/$(OUTPUT)
	@echo Uninstall complete!

compile : $(OBJ) 
	@$(CC) $^ -o $(OUTPUT) $(CFLAGS)	
	@echo "Linking done. Compilation successful."

obj/%.o : src/%.$(EXT) 
	@mkdir -p $(@D) # $(@D) <- Gets directory part of target path
	@if $(CC) $< -o $@ $(CFLAGS) -c -MMD -MP; then\
		echo -e "Compiled `tput bold``tput setaf 3`$<`tput sgr0`.";\
	fi


-include $(DEP)

-FILES_IN_OBJ = $(shell find obj -name *.o)

remove_unused_objects :
ifneq '' '$(filter-out $(OBJ), $(FILES_IN_OBJ))' # finds out which object files no longer have an associated source file
	@rm -r $(filter-out $(OBJ), $(FILES_IN_OBJ))
	-@rm -r $(filter-out $(DEP), $(FILES_IN_OBJ:%.o=%.d))
	@echo "Cleaned out unused .o and .dep files"
else
	@echo "No object files require deletion."
endif
	
clean : 
	@rm -r obj/*
	@rm $(shell find . -name $(OUTPUT)*)
	@echo "Cleaned out object files and binaries."

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
