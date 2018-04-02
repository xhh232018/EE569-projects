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
unsigned char findmax(int x, int y, int z) {
	int max;
	if (x >= y) 
		max = x;
	else
		max = y;
	if (z > max)
		max = z;
	return (unsigned char)max;
		
}
unsigned char findmin(int x, int y, int z) {
	int min;
	if (x <= y)
		min = x;
	else
		min = y;
	if (z < min)
		min = z;
	return (unsigned char)min;
}
int main(int argc,char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int nbpp=1;
	vector<unsigned char> pix;
	vector<unsigned char> pix_gray1;
	vector<unsigned char> pix_gray2;
	vector<unsigned char> pix_gray3;
	unsigned char temp, temp1, temp2, temp3, temp4;
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
		fread(&pix[0], sizeof(unsigned char),imageWidth*imageHeight*BytesPerPixel, inputFile);
		fclose(inputFile);
	
	cout << "loading data success!";
		for (int index = 0; index < imageWidth*imageHeight*BytesPerPixel; index=index+3) {
			
			temp1 = findmin((int)pix[index], (int)pix[index + 1], (int)pix[index + 2]);
			temp2 = findmax((int)pix[index], (int)pix[index + 1], (int)pix[index + 2]);
			temp = (temp1 + temp2) / 2;
			temp3 = (pix[index] + pix[index + 1] + pix[index + 2]) / 3;
			temp4 = 0.21*pix[index] + 0.72*pix[index + 1] + 0.07*pix[index + 2];
			pix_gray1.push_back(temp);
			pix_gray2.push_back(temp3);
			pix_gray3.push_back(temp4);

   }

		FILE* outputFile;
		if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
			cout << "Cannot open file: ";
			exit(1);
		}
		//    cout<< (int)pixelData[0] << endl;
		fwrite(&pix_gray1[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
		fclose(outputFile);
		if (!(outputFile = fopen(outputFileName2.c_str(), "wb"))) {
			cout << "Cannot open file: ";
			exit(1);
		}
		//    cout<< (int)pixelData[0] << endl;
		fwrite(&pix_gray2[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
		fclose(outputFile);
		if (!(outputFile = fopen(outputFileName3.c_str(), "wb"))) {
			cout << "Cannot open file: ";
			exit(1);
		}
		//    cout<< (int)pixelData[0] << endl;
		fwrite(&pix_gray3[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
		fclose(outputFile);
    return 0;
}
