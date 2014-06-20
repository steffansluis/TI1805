#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>

#include "Image.h"

bool Image::writeImage(const char * filename)
{
	FILE* file;
	file = fopen(filename, "wb");
	if (!file)
	{
		printf("dump file problem... file\n");
		return false;
	}

	fprintf(file, "P6\n%i %i\n255\n", _width, _height);


	std::vector<unsigned char> imageC(_image.size());

	for (unsigned int i = 0; i<_image.size(); ++i)
		imageC[i] = (unsigned char)(_image[i] * 255.0f);

	int t = fwrite(&(imageC[0]), _width * _height * 3, 1, file);
	if (t != 1)
	{
		printf("Dump file problem... fwrite\n");
		return false;
	}

	fclose(file);
	return true;
}