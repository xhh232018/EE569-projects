#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
using namespace std;
void imageSave(const char* outputFileName, vector<unsigned char>pixelData, int width, int height, int BytePerPix) {
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName, "wb"))) {
		cout << "Cannot open file: " << outputFileName << endl;
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pixelData[0], sizeof(unsigned char), width*height*BytePerPix, outputFile);
	fclose(outputFile);
}

int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int nbpp = 1;
	vector<unsigned char> pix;
	vector<unsigned char> pix_c;
	vector<unsigned char> pix_m;
	vector<unsigned char> pix_y;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	string outputFileName2 = argv[6];
	string outputFileName3 = argv[7];
	// Check if image is grayscale or color
	if (argc < 4) {
		BytesPerPixel = 1;// default is grey image
		imageHeight = 256;
		imageWidth = 256;
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5) {
			imageWidth = atoi(argv[4]);
			imageHeight = atoi(argv[5]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";
	for (int index = 0; index < imageWidth*imageHeight*BytesPerPixel; index = index + 3) {
		double temp_c, temp_m, temp_y;
		temp_c = (1 - (double)pix[index] / 255.0)*255.0;
		temp_m = (1 - (double)pix[index + 1] / 255.0)*255.0;
		temp_y = (1 - (double)pix[index + 2] / 255.0)*255.0;
		pix_c.push_back((unsigned char)temp_c);
		pix_m.push_back((unsigned char)temp_m);
		pix_y.push_back((unsigned char)temp_y);
	}

	FILE* outputFile1;
	if (!(outputFile1 = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_c[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile1);
	fclose(outputFile1);

	FILE* outputFile2;
	if (!(outputFile2 = fopen(outputFileName2.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_m[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile2);
	fclose(outputFile2);

	FILE* outputFile3;
	if (!(outputFile3 = fopen(outputFileName3.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_y[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile3);
	fclose(outputFile3);
	return 0;
}
