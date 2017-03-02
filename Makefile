bmpedit : bmpedit.c
	gcc -Wall -std=gnu99 -o bmpedit bmpedit.c data/bitmap.c extensions/threshold.c extensions/invert.c extensions/crop.c extensions/monochrome.c extensions/blur.c
