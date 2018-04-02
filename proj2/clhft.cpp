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
	int N1=3;
	vector<unsigned char> pix;
	vector<unsigned char> pix_c;
	vector<unsigned char> pix_m;
	vector<unsigned char> pix_y;
	vector<vector<unsigned char> > extimg_c;
	vector<vector<unsigned char> > extimg_m;
	vector<vector<unsigned char> > extimg_y;
	double  FS_e[3][3] = { { 0,0,0 },{ 0,0,7.0 / 16.0 },{ 3.0 / 16,5.0 / 16,1.0 / 16 } };
	double  FS_o[3][3] = { { 0,0,0 },{ 7.0 / 16,0,0 },{ 1.0 / 16,5.0 / 16,3.0 / 16 } };
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
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
	extimg_c.resize(imageHeight + 2 * floor(N1 / 2));
	extimg_m.resize(imageHeight + 2 * floor(N1 / 2));
	extimg_y.resize(imageHeight + 2 * floor(N1 / 2));

	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg_c[i].resize(imageWidth + 2 * floor(N1 / 2));
		extimg_m[i].resize(imageWidth + 2 * floor(N1 / 2));
		extimg_y[i].resize(imageWidth + 2 * floor(N1 / 2));
	}

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
	//拓展图片
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg_c[r + N1 / 2][c + N1 / 2] = pix_c[r*imageWidth + c];
			extimg_m[r + N1 / 2][c + N1 / 2] = pix_m[r*imageWidth + c];
			extimg_y[r + N1 / 2][c + N1 / 2] = pix_y[r*imageWidth + c];

		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg_c[r][c + N1 / 2] = pix_c[(N1 / 2 - r)*imageWidth + c ];
			extimg_c[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = pix_c[(imageHeight - 1 - N1 / 2 + r)*imageWidth + c ];

			extimg_m[r][c + N1 / 2] = pix_m[(N1 / 2 - r)*imageWidth + c];
			extimg_m[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = pix_m[(imageHeight - 1 - N1 / 2 + r)*imageWidth + c];

			extimg_y[r][c + N1 / 2] = pix_y[(N1 / 2 - r)*imageWidth + c];
			extimg_y[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = pix_y[(imageHeight - 1 - N1 / 2 + r)*imageWidth + c];
		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N1 / 2); r++) {
		for (int c = 0; c < N1 / 2; c++) {
			extimg_c[r][c] = extimg_c[r][N1 - 1 - c];
			extimg_c[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg_c[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];

			extimg_m[r][c] = extimg_m[r][N1 - 1 - c];
			extimg_m[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg_m[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];

			extimg_y[r][c] = extimg_m[r][N1 - 1 - c];
			extimg_y[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg_m[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];
		}
	}

	//hft C
	int oldpixel, newpixel, error;
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N1 / 2 + 1) % 2 == 0)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg_c[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_c[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_c[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_c[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_c[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_c[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_e[i + 1][j + 1];
					}
				}
			}
		}
		//奇数行
		if ((r + N1 / 2 + 1) % 2 == 1) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg_c[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_c[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_c[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_c[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_c[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_c[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_o[i + 1][j + 1];
					}
				}
			}
		}
	}

	//hft M
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N1 / 2 + 1) % 2 == 0)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg_m[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_m[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_m[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_m[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_m[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_m[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_e[i + 1][j + 1];
					}
				}
			}
		}
		//奇数行
		if ((r + N1 / 2 + 1) % 2 == 1) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg_m[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_m[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_m[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_m[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_m[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_m[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_o[i + 1][j + 1];
					}
				}
			}
		}
	}

	//hft Y
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N1 / 2 + 1) % 2 == 0)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg_y[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_y[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_y[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_y[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_y[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_y[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_e[i + 1][j + 1];
					}
				}
			}
		}
		//奇数行
		if ((r + N1 / 2 + 1) % 2 == 1) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg_y[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg_y[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg_y[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg_y[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg_y[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg_y[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_o[i + 1][j + 1];
					}
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = (1-extimg_c[r + N1 / 2][c + N1 / 2]/255.0)*255;
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+1] = (1-extimg_m[r + N1 / 2][c + N1 / 2]/255.0)*255;
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+2] = (1-extimg_y[r + N1 / 2][c + N1 / 2]/255.0)*255;

		}
	}

	FILE* outputFile1;
	if (!(outputFile1 = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile1);
	fclose(outputFile1);
	return 0;
}
