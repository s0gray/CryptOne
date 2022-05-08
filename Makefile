.PHONY: echo clean_d clean

# vars
app_name := ./CryptOne

# preprocessor definition
preprocess := _DEBUG
preprocess := $(addprefix -D , $(preprocess))

LIB_ROOT:=./

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
objects := $(addprefix ./bin/, $(objects))
object_files := $(objects)

#includes
includes := $(LIB_ROOT)/boost/include 
#ifdef mysqli
#  includes += /usr/include/mysql
#else
#  includes += ../mysqlclient/include
#endif
includes += $(LIB_ROOT)/libsodium/include 

#includes += ../../cpp-driver-1.0/include 
includes += /usr/include 
includes += $(source_dirs)
includes := $(addprefix -I , $(includes))


# library
lib_files :=  -ldl -lpthread -lstdc++ -lrt -lsodium -lncurses
# -lssl -lcrypto
#-lrt -leXosip2 -lortp -losip2 -losipparser2 -lamr_codec -lcurl
#lib_files += $(wildcard $(LIB_ROOT)/libsodium/lib/libsodium.a)

#ifdef mysqli
#  lib_files += -lmysqlclient
#else
#  lib_files += $(wildcard ../mysqlclient/lib/*.a)
#endif

VPATH := $(source_dirs)

$(app_name):  $(object_files) 	
	g++ $^ $(lib_files) -g -o $@

	
./bin/%.o: %.cpp 
	g++ -std=c++0x -g -c $(includes) $(preprocess) -MD $< -o $@ 
		
#include $(wildcard *.d)

echo:
	@echo $(source_files)
	
clean_d:
	rm bin/*.d
clean:
	rm bin/*.o bin/*.d $(app_name) 
