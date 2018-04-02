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
	int N;
	unsigned char rtemp, gtemp, btemp;
	vector<unsigned char> pix;
	vector<unsigned char> pix_rf;
	vector<unsigned char> pixr_wd;
	vector<unsigned char> pixg_wd;
	vector<unsigned char> pixb_wd;
	vector<vector<vector<unsigned char> > > extimg;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string inpurFileName1 = argv[7];
	string outputFileName = argv[2];
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
			N = atoi(argv[6]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_rf.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pixr_wd.resize(N*N);
	pixg_wd.resize(N*N);
	pixb_wd.resize(N*N);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	if (!(inputFile = fopen(inpurFileName1.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix_rf[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	cout << "loading data success!";
	//step1:拓展图片
	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
		for (int j = 0; j < imageWidth + 2 * floor(N / 2); j++) {
			extimg[i][j].resize(BytesPerPixel);
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r + N / 2][c + N / 2][0] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];

		}
	}
	for (int r = 0; r < N / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r][c + N / 2][0] = pix[(N / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg[imageHeight + 2 * floor(N / 2) - 1 - r][c + N / 2][0] = pix[(imageHeight - 1 - N / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];

		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N / 2); r++) {
		for (int c = 0; c < N / 2; c++) {
			extimg[r][c][0] = extimg[r][N - 1 - c][0];
			extimg[r][imageWidth + 2 * floor(N / 2) - 1 - c][0] = extimg[r][imageWidth + 2 * floor(N / 2) - 1 - 2 * floor(N / 2) + c][0];
		}
	}//图像拓展完毕
	for (int index_rori = 0; index_rori < imageHeight; index_rori++) {
		for (int index_cori = 0; index_cori < imageWidth; index_cori++) {
                    for(int depth = 0; depth<BytesPerPixel;depth++){
			if (((int)pix[index_rori*imageWidth*BytesPerPixel + index_cori * BytesPerPixel+depth] == 255) || ((int)pix[index_rori*imageWidth*BytesPerPixel + index_cori * BytesPerPixel+depth] == 0)) {
				for (int index_rwd = 0; index_rwd < N; index_rwd++)
				{
					for (int index_cwd = 0; index_cwd < N; index_cwd++)
					{
						pixr_wd[index_rwd*N + index_cwd] = extimg[index_rori + index_rwd][index_cori + index_cwd][0];
					}
				}
				sort(pixr_wd.begin(), pixr_wd.end());
				rtemp = pixr_wd[N*N / 2];
				pix[index_rori*imageWidth*BytesPerPixel + index_cori * BytesPerPixel+depth] = rtemp;
			}

		    }	 
		}
	}
	double mse_r, psnr_r;
	int val_r;
	mse_r = 0.0;
		for (int i = 0; i<imageHeight; i++)
		{
			for (int j = 0; j<imageWidth; j++)
			{

				val_r = abs((int)pix_rf[i*imageWidth*BytesPerPixel + j * BytesPerPixel] - (int)pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel]);
				mse_r += pow(val_r,2);


			}
		}


	mse_r = mse_r / (imageWidth*imageHeight);
	psnr_r = 10 * log10(255 * 255 / mse_r);
	cout << psnr_r << endl;
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	return 0;
}
