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
	int newHeight;
	int newWidth;
	unsigned char tempr, tempg, tempb;
	vector<unsigned char> pix;
	vector<unsigned char> pix_out;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
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
			newHeight = atoi(argv[6]);
			newWidth = atoi(argv[7]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_out.resize(newHeight*newWidth*BytesPerPixel, 0);
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";
	for (int height = 0; height < newHeight; height++) {
		double ht_mapping = double(1.0*(imageHeight - 1) / (newHeight - 1))*height;
		int ht_flr = floor(ht_mapping);
		double delta_h = ht_mapping - ht_flr;
		for (int width = 0; width < newWidth;width++) {
			double wd_mapping = double(1.0*(imageWidth - 1) / (newWidth - 1))*width;
			int wd_flr = floor(wd_mapping);
			double delta_w = wd_mapping - wd_flr;
			
				if (ht_flr == (imageHeight - 1) && wd_flr == (imageWidth - 1))
				{
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel] = pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel];
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel + 1] = pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + 1];
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel+2] = pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + 1];
					continue;
				}
				//最后一行
				if (ht_flr == (imageHeight - 1) && wd_flr != (imageWidth - 1))
				{
	
					tempr =  delta_h * (1 - delta_w) * pix[ht_flr*imageWidth*BytesPerPixel+wd_flr*BytesPerPixel] +
					         delta_h * delta_w*pix[ht_flr*imageWidth*BytesPerPixel+(wd_flr+1)*BytesPerPixel];
					tempg =  delta_h * (1 - delta_w) * pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+1] +
						     delta_h * delta_w*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel+1];
					tempb =  delta_h * (1 - delta_w) * pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+2] +
						     delta_h * delta_w*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel+2];

				    pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel] = tempr;
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel+1] = tempg;
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel+2] = tempb;

					continue;
				}//最后一列
				if (ht_flr != (imageHeight - 1) && wd_flr == (imageWidth - 1))
				{
					
					tempr =  delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel] +
					         delta_h * delta_w*pix[(ht_flr+1)*imageWidth*BytesPerPixel + wd_flr*BytesPerPixel];
					tempg =  delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+1] +
						     delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+1];
					tempb =  delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+2] +
						     delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+2];
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel] = tempr;
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel + 1] = tempg;
					pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel + 2] = tempb;
					continue;
				}
				tempr = (1 - delta_h)*(1 - delta_w)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel] +
					delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel] +
					delta_h * (1 - delta_w)*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel] +
					delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + (wd_flr + 1) * BytesPerPixel];
				tempg = (1 - delta_h)*(1 - delta_w)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+1] +
					delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel+1] +
					delta_h * (1 - delta_w)*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+1] +
					delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + (wd_flr + 1) * BytesPerPixel+1];
				tempb = (1 - delta_h)*(1 - delta_w)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+2] +
					delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel+2] +
					delta_h * (1 - delta_w)*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel+2] +
					delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + (wd_flr + 1) * BytesPerPixel+2];
				pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel] = tempr;
				pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel + 1] = tempg;
				pix_out[height*newWidth*BytesPerPixel + width * BytesPerPixel + 2] = tempb;
		}
	}

	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	fwrite(&pix_out[0], sizeof(unsigned char), newWidth*newHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	return 0;
}
