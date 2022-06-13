.PHONY: echo clean_d clean

# vars
app_name := ./CryptOne

# preprocessor definition
preprocess := _DEBUG
preprocess := $(addprefix -D , $(preprocess))

LIB_ROOT:=.

#  Project root directory
project_dir := ./
# Enum all subdirectories in 
project_subdirs := libs

source_dirs := $(addprefix ./, $(project_dir))
source_dirs += $(addprefix ./$(project_dir)/, $(project_subdirs))

source_files := $(addsuffix /*.cpp, $(source_dirs))
source_files := $(wildcard $(source_files))


# set object files
objects := $(patsubst %.cpp, %.o, $(source_files))
objects := $(notdir $(objects))
objects := $(addprefix ./build/, $(objects))
object_files := $(objects)

#includes
includes := ./
includes += $(LIB_ROOT)/libsodium/include 

includes += /usr/include 
includes += $(source_dirs)
includes := $(addprefix -I , $(includes))


# library
lib_files :=  -ldl -lpthread -lstdc++ -lrt -lsodium -lncurses

VPATH := $(source_dirs)

$(app_name):  $(object_files) 	
	g++ $^ $(lib_files) -g -o $@

	
./build/%.o: %.cpp 
	g++ -std=c++0x -g -c $(includes) $(preprocess) -MD $< -o $@ 		

echo:
	@echo $(source_files)
	
clean_d:
	rm build/*.d
clean:
	rm build/*.o build/*.d $(app_name) 
