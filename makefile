CC = mos-cx16-clang
CARGS = -Os -flto -fnonreentrant -Wno-int-conversion 
# -O1 -ffast-math

ZSMKIT = zsmkit.o
WAVKIT = wavkit.o


ifeq ($(OS),Windows_NT)
# windows
define del
del $(subst /,\\,$(1))
endef

define mkdir
mkdir $(subst /,\\,$(1))
endef

else
# linux
define del
rm -rf $(1)
endef

define mkdir
mkdir -p $(1)
endef

endif



default: make


make:
	$(call del, ./OUT)
	$(call mkdir, ./OUT)
	$(call mkdir, ./OUT/ASSETS)

	./compile_everything.sh


	cp ./tilemaps/*.RLE ./OUT/ASSETS
#	cp ./metatiles/*.BIN ./OUT/ASSETS
	cp ./music/*.ZSM ./OUT/ASSETS
#	cp ./levels/*.LVL ./OUT/ASSETS
#	# cp ./music/*.ZCM ./OUT/ASSETS
	cp ./graphics/*.DAT ./OUT/ASSETS
	cp ./src/lib/*.BIN ./OUT/ASSETS
	cp ./wav/*.WAV ./OUT/ASSETS
	
	# cp './startup files/run.sh' ./OUT

	# zsmkit
	$(CC) -c ./src/lib/zsmkit.c -o $(ZSMKIT) 
	# wavkit
	$(CC) -c ./src/lib/wavkit.c -o $(WAVKIT)
	# main
	$(CC) $(CARGS) ./src/main.c $(ZSMKIT) $(WAVKIT) -o ./OUT/FIVENIGHTS.PRG -T link.ld

	rm -f src/main.o
	rm -f *.o
	cd ./OUT

run: 
	./x16emu/x16emu -midline-effects -debug -zeroram -c816
# 
	rm -rf ./OUT

run-debug:
	./x16emu/box16

do-all: make run
do-all-debug: make run-debug
