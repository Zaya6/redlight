CC := clang
cflags := -Wall -std=c99 -Iext/includes
ldflags := -fsanitize=address 

files := $(wildcard src/*.c)
output := bin/redlight.exe

ifeq ($(OS),Windows_NT)
	ldflags += -L$(WINLIBPATH) -Lext/win64 -lSDL2main -lSDL2 -lShell32 
#else
#	UNAME_S := $(shell uname -s)
#	ifeq ($(UNAME_S),Linux)
#		#stuff for linux
#	endif
#	ifeq ($(UNAME_S),Darwin)
#		#do nothing right now
#	endif
endif



debug: $(output)
	clang -g  -DDebug=1 $(cflags) -o $(output) $(ldflags) -static -Wl,-subsystem:console $(files) 
	$(info ------files------)
	dir /b bin


release: $(output)
	clang -O3 -DDebug=0 $(cflags) -o $(output) $(ldflags) -static -Wl,-subsystem:windows $(files) 
	$(info ------files------)
	dir /b bin

$(output):
	$(info ------building------)
	$(info ---platform Win64---)

#for release -Xlinker /subsystem:console
