CC := clang
cflags := -Wall -std=c99 
ldflags := -fsanitize=address 

files := $(wildcard src/*.c)
output :=
makedir :=
listdir :=

ifeq ($(OS),Windows_NT)
	cfags += -Iext/includes
	ldflags += -L$(WINLIBPATH) -Lext/win64 -lSDL2main -lSDL2 -lShell32
	output := build/win64/redlight.exe
	listdir := dir /b build\win64
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		cflags += -lm `sdl2-config --cflags` -Iext/includes
		ldflags += `sdl2-config --libs`
		output := build/linux/redlight
		makedir := mkdir -p build/linux
		listdir := ls -l build/linux
	endif
#	ifeq ($(UNAME_S),Darwin)
#		#do nothing right now
#	endif
endif

#-static
#-Wl,-subsystem:console
debug: $(output)
	$(makedir)
	clang -g  -DDebug=1 $(cflags) -o $(output) $(ldflags) $(files) 
	$(info ------files------)
	$(listdir)


release: $(output)
	clang -O3 -DDebug=0 $(cflags) -o $(output) $(ldflags) -static -Wl,-subsystem:windows $(files) 
	$(info ------files------)
	dir /b bin

$(output):
	$(info ------building------)
	$(info ---platform Win64---)

#for release -Xlinker /subsystem:console
