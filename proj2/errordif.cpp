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
	int nbpp = 1;
	int N1 = 3;
	int N2 = 5;
	double  FS_e[3][3] = { {0,0,0},{0,0,7.0 / 16.0},{3.0 / 16,5.0 / 16,1.0 / 16} };
	double  FS_o[3][3] = { { 0,0,0 },{ 7.0 / 16,0,0 },{ 1.0 / 16,5.0 / 16,3.0 / 16 } };
	double  JJN_e[5][5] = { {0,0,0,0,0},{0,0,0,0,0},{0,0,0,7.0 / 48,5.0 / 48},{3.0 / 48,5.0 / 48,7.0 / 48,5.0 / 48,3.0 / 48},{1.0 / 48,3.0 / 48,5.0 / 48,3.0 / 48,1.0 / 48} };
	double  JJN_o[5][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 5.0 / 48,7.0 / 48,0,0,0 },{ 3.0 / 48,5.0 / 48,7.0 / 48,5.0 / 48,3.0 / 48 },{ 1.0 / 48,3.0 / 48,5.0 / 48,3.0 / 48,1.0 / 48 } };
	double  STU_e[5][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 0,0,0,8.0 / 42,4.0 / 42 },{ 2.0 / 42,4.0 / 42,8.0 / 42,4.0 / 42 ,2.0 / 42},{ 1.0 / 42,2.0 / 42,4.0 / 42,2.0 / 42 ,1.0 / 42 } };
	double  STU_o[5][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 4.0 / 42,8.0 / 42,0,0,0 },{ 2.0 / 42,4.0 / 42,8.0 / 42,4.0 / 42 ,2.0 / 42 },{ 1.0 / 42,2.0 / 42,4.0 / 42,2.0 / 42 ,1.0 / 42 } };
	double  Ski_e[5][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 0,0,0,8.0 / 32,4.0 / 32 },{ 2.0 / 32,4.0 / 32,8.0 / 32,4.0 / 32 ,2.0 / 32 },{ 0,0,0,0,0 } };
	double  Ski_o[5][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 4.0 / 32,8.0 / 32,0,0,0 },{ 2.0 / 32,4.0 / 32,8.0 / 32,4.0 / 32 ,2.0 / 32 }, {0,0,0,0,0 } };
	vector<unsigned char> pix;
	vector<unsigned char> pix_gray1;
	vector<unsigned char> pix_gray2;
	vector<unsigned char> pix_gray3;
	vector<unsigned char> pix_gray4;
	vector<vector<unsigned char> > extimg1;
	vector<vector<unsigned char> > extimg2;
	vector<vector<unsigned char> > extimg3;
	vector<vector<unsigned char> > extimg4;
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
	string outputFileName4 = argv[8];
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
	pix_gray1.resize(imageHeight*imageHeight*BytesPerPixel, 0);
	pix_gray2.resize(imageHeight*imageHeight*BytesPerPixel, 0);
	pix_gray3.resize(imageHeight*imageHeight*BytesPerPixel, 0);
	pix_gray4.resize(imageHeight*imageHeight*BytesPerPixel, 0);
	extimg1.resize(imageHeight + 2 * floor(N1 / 2));
	extimg2.resize(imageHeight + 2 * floor(N2 / 2));
	extimg3.resize(imageHeight + 2 * floor(N2 / 2));
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";
	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg1[i].resize(imageWidth + 2 * floor(N1 / 2));
	}
	for (int i = 0; i < imageHeight + 2 * floor(N2 / 2); i++) {
		extimg2[i].resize(imageWidth + 2 * floor(N2 / 2));
		extimg3[i].resize(imageWidth + 2 * floor(N2 / 2));
	}
	//拓展图片（3*3）

	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r + N1 / 2][c + N1 / 2] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r][c + N1 / 2] = pix[(N1 / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg1[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = pix[(imageHeight - 1 - N1 / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N1 / 2); r++) {
		for (int c = 0; c < N1 / 2; c++) {
			extimg1[r][c] = extimg1[r][N1 - 1 - c];
			extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];
		}
	}//图像拓展完毕

	//拓展图片（5*5）
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg2[r + N2 / 2][c + N2 / 2] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < N2 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg2[r][c + N2 / 2] = pix[(N2 / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg2[imageHeight + 2 * floor(N2 / 2) - 1 - r][c + N2 / 2] = pix[(imageHeight - 1 - N2 / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N2 / 2); r++) {
		for (int c = 0; c < N2 / 2; c++) {
			extimg2[r][c] = extimg2[r][N2 - 1 - c];
			extimg2[r][imageWidth + 2 * floor(N2 / 2) - 1 - c] = extimg2[r][imageWidth + 2 * floor(N2 / 2) - 1 - 2 * floor(N2 / 2) + c];
		}
	}//5*5拓展完毕
	extimg3 = extimg2;
	extimg4 = extimg2;
	 //3*3的diffusion开始
	int oldpixel, newpixel, error;
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N1 / 2 + 1) % 2 == 0)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg1[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg1[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg1[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg1[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg1[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg1[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_e[i + 1][j + 1];
					}
				}
			}
		}
		//奇数行
		if ((r + N1 / 2 + 1) % 2 == 1) {
			for (int c = imageWidth-1; c >=0; c--) {
				if (extimg1[r + N1 / 2][c + N1 / 2] > 127) {
					oldpixel = extimg1[r + N1 / 2][c + N1 / 2];
					newpixel = 255;
					extimg1[r + N1 / 2][c + N1 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg1[r + N1 / 2][c + N1 / 2];
					newpixel = 0;
					extimg1[r + N1 / 2][c + N1 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						extimg1[r + N1 / 2 + i][c + N1 / 2 + j] += error * FS_o[i + 1][j + 1];
					}
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_gray1[r*imageWidth + c] = extimg1[r + N1 / 2][c + N1 / 2];
		}
	}

	//5*5第一次
	for (int r = 0; r < imageHeight; r++) {
		//奇数行
		if ((r + N2 / 2 + 1) % 2 == 1)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg2[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg2[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg2[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg2[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg2[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg2[r + N2 / 2 + i][c + N2 / 2 + j] += error * JJN_e[i + 2][j + 2];
					}
				}
			}
		}
		//偶数行
		if ((r + N2 / 2 + 1) % 2 == 0) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg2[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg2[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg2[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg2[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg2[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg2[r + N2 / 2 + i][c + N2 / 2 + j] += error * JJN_o[i + 2][j + 2];
					}
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_gray2[r*imageWidth + c] = extimg2[r + N2 / 2][c + N2 / 2];
		}
	}

	//5*5第二次
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N2 / 2 + 1) % 2 == 1)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg3[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg3[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg3[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg3[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg3[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg3[r + N2 / 2 + i][c + N2 / 2 + j] += error * STU_e[i + 2][j + 2];
					}
				}
			}
		}
		//奇数行
		if ((r + N2 / 2 + 1) % 2 == 0) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg3[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg3[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg3[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg3[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg3[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg3[r + N2 / 2 + i][c + N2 / 2 + j] += error * STU_o[i + 2][j + 2];
					}
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_gray3[r*imageWidth + c] = extimg3[r + N2 / 2][c + N2 / 2];
		}
	}

	//5*5第三次
	for (int r = 0; r < imageHeight; r++) {
		//偶数行
		if ((r + N2 / 2 + 1) % 2 == 1)
		{
			for (int c = 0; c < imageWidth; c++) {
				if (extimg4[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg4[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg4[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg4[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg4[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg4[r + N2 / 2 + i][c + N2 / 2 + j] += error * Ski_e[i + 2][j + 2];
					}
				}
			}
		}
		//奇数行
		if ((r + N2 / 2 + 1) % 2 == 0) {
			for (int c = imageWidth - 1; c >= 0; c--) {
				if (extimg4[r + N2 / 2][c + N2 / 2] > 127) {
					oldpixel = extimg4[r + N2 / 2][c + N2 / 2];
					newpixel = 255;
					extimg4[r + N2 / 2][c + N2 / 2] = 255;
					error = oldpixel - newpixel;
				}
				else
				{
					oldpixel = extimg4[r + N2 / 2][c + N2 / 2];
					newpixel = 0;
					extimg4[r + N2 / 2][c + N2 / 2] = 0;
					error = oldpixel - newpixel;
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						extimg4[r + N2 / 2 + i][c + N2 / 2 + j] += error * Ski_o[i + 2][j + 2];
					}
				}
			}
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			pix_gray4[r*imageWidth + c] = extimg4[r + N2 / 2][c + N2 / 2];
		}
	}
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	fwrite(&pix_gray1[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
	fclose(outputFile);
	if (!(outputFile = fopen(outputFileName2.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	fwrite(&pix_gray2[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
	fclose(outputFile);
	if (!(outputFile = fopen(outputFileName3.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	fwrite(&pix_gray3[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
	if (!(outputFile = fopen(outputFileName4.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	fwrite(&pix_gray4[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
	fclose(outputFile);
	return 0;
}
