# graphics
cd ./graphics
    ./X16PngConverter ./tiles/menu4bpp.png -tiles -w 8 -h 8
	./X16PngConverter ./tiles/static4bpp.png -tiles -w 8 -h 8
	#./X16PngConverter ./tiles/game2bpp.png -tiles -w 8 -h 8
	#./X16PngConverter ./tiles/menu4bpp.png -tiles -w 8 -h 8
	cp ./tiles/*.BIN ../OUT/ASSETS
	
	./X16PngConverter ./sprites/fan.png -sprites -w 32 -h 64
	./X16PngConverter ./sprites/fnaf.png -sprites -w 64 -h 64
	./X16PngConverter ./sprites/fnaf2.png -sprites -w 16 -h 64
	./X16PngConverter ./sprites/fnaf3.png -sprites -w 8 -h 8
	cp ./sprites/*.BIN ../OUT/ASSETS

	./convert.sh  # this is for images like the office
cd ..

# tilemaps
cd ./tilemaps/csv
    ./compile-backgrounds.sh
cd ../..



# levels
#cd ./levels/csv
#	./compile-levels.sh
