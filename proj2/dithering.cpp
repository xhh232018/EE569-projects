#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	double T2[2][2] = { { 1.5/4,2.5/4 },{ 3.5/4,0.5/4 } };
	double T4[4][4] = { {5.5/16,9.5/16,6.5/16,10.5/16},{13.5/16,1.5/16,14.5/16,2.5/16},{7.5/16,11.5/16,4.5/16,8.5/16},{15.5/16,3.5/16,12.5/16,0.5/16} };
	double T8[8][8] = { {21.5,37.5,25.5,41.5,22.5,38.5,26.5,42.5},
	                    {53.5,5.5,57.5,9.5,54.5,6.5,58.5,10.5},
	                    {29.5,45.5,17.5,33.5,30.5,46.5,18.5,34.5},
						{61.5,13.5,49.5,1.5,62.5,14.5,50.5,2.5},
	                    {23.5,39.5,27.5,43.5,20.5,36.5,24.5,40.5},
	                    {55.5,7.5,59.5,11.5,52.5,4.5,56.5,8.5},
	                    {31.5,47.5,19.5,35.5,28.5,44.5,16.5,32.5},
	                    {63.5,15.5,51.5,3.5,60.5,12.5,48.5,0}};
	
	vector<unsigned char> pix;
	vector<unsigned char> pix_gray1;
	vector<unsigned char> pix_gray2;
	vector<unsigned char> pix_gray3;
	vector<unsigned char> pix_gray4;
	vector<unsigned char> pix_gray5;
	vector<unsigned char> pix_gray6;

	int mod_iqr, mod_jqr;
	int mod_iduo, mod_jduo;
	int mod_io, mod_jo;
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
	string outputFileName4 = argv[8];
	string outputFileName5 = argv[9];
	string outputFileName6 = argv[10];
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
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			T8[i][j] = T8[i][j] / 64;
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			mod_iduo = r % 2;
			mod_jduo = c % 2;
			mod_iqr = r % 4;
			mod_jqr = c % 4;
			mod_io = r % 8;
			mod_jo = c % 8;

			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >= 255 * T2[mod_iduo][mod_jduo]) {

				pix_gray1.push_back((unsigned char)255);
			}
			else
			{
				pix_gray1.push_back((unsigned char)0);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >= 255 * T4[mod_iqr][mod_jqr]) {

				pix_gray2.push_back((unsigned char)255);
			}
			else
			{
				pix_gray2.push_back((unsigned char)0);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >= 255 * T8[mod_io][mod_jo]) {

				pix_gray3.push_back((unsigned char)255);
			}
			else
			{
				pix_gray3.push_back((unsigned char)0);
			}

		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			mod_iduo = r % 2;
			mod_jduo = c % 2;
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >=0 && pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel]<= 255 * T2[mod_iduo][mod_jduo]/2) {

				pix_gray4.push_back((unsigned char)0);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T2[mod_iduo][mod_jduo] / 2 && 
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T2[mod_iduo][mod_jduo]) {

				pix_gray4.push_back((unsigned char)85);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T2[mod_iduo][mod_jduo] &&
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T2[mod_iduo][mod_jduo] / 2+127) {

				pix_gray4.push_back((unsigned char)170);
			}
			if(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T2[mod_iduo][mod_jduo] / 2 + 127)
			{
				pix_gray4.push_back((unsigned char)255);
			}

		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			mod_iqr = r % 4;
			mod_jqr = c % 4;
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >= 0 && pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T4[mod_iqr][mod_jqr] / 2) {

				pix_gray5.push_back((unsigned char)0);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T4[mod_iqr][mod_jqr] / 2 &&
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T4[mod_iqr][mod_jqr]) {

				pix_gray5.push_back((unsigned char)85);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T4[mod_iqr][mod_jqr] &&
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T4[mod_iqr][mod_jqr] / 2 + 127) {

				pix_gray5.push_back((unsigned char)170);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T4[mod_iqr][mod_jqr] / 2 + 127)
			{
				pix_gray5.push_back((unsigned char)255);
			}

		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			mod_io = r % 8;
			mod_jo = c % 8;
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] >= 0 && pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T8[mod_io][mod_jo] / 2) {

				pix_gray6.push_back((unsigned char)0);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T8[mod_io][mod_jo] / 2 &&
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T8[mod_io][mod_jo]) {

				pix_gray6.push_back((unsigned char)85);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T8[mod_io][mod_jo] &&
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] <= 255 * T8[mod_io][mod_jo] / 2 + 127) {

				pix_gray6.push_back((unsigned char)170);
			}
			if (pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] > 255 * T8[mod_io][mod_jo] / 2 + 127)
			{
				pix_gray6.push_back((unsigned char)255);
			}

		}
	}
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray1[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	if (!(outputFile = fopen(outputFileName2.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray2[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	if (!(outputFile = fopen(outputFileName3.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray3[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	if (!(outputFile = fopen(outputFileName4.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray4[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	if (!(outputFile = fopen(outputFileName5.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray5[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	if (!(outputFile = fopen(outputFileName6.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_gray6[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	return 0;

}
