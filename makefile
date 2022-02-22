CC := clang
cflags := -Wall -Wno-missing-braces -std=c99 
ldflags := 

files := $(wildcard src/*.c)
output :=
makedir :=
listdir :=
extra :=
extraRelease := 

ifeq ($(OS),Windows_NT)
	cfags += -Iext/includes
	ldflags += -L$(WINLIBPATH) -Lext/win64 -lSDL2main -lSDL2 -lShell32
	output := build/win64/redlight.exe
	listdir := dir /b build\win64
	extra := -Wl,-subsystem:console
	extraRelease := -Wl,-subsystem:windows
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
	clang -g -O0 -fno-omit-frame-pointer -fsanitize=address -DDebug=1 $(cflags) -o $(output) $(ldflags)  $(extra) $(files) 
	$(info ------files------)
	$(listdir)


release: $(output)
	$(makedir)
	clang -O3 -DDebug=0 $(cflags) -o $(output) $(ldflags) -static $(extra)  $(files) 
	$(info ------files------)
	$(listdir)

$(output):
	$(info ------building------)

#for release -Xlinker /subsystem:console
