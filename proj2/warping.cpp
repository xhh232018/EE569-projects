#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
using namespace std;

unsigned char bilinearInterpolation(vector<unsigned char> pix, int ht_flr, int wd_flr, double delta_h, double delta_w, int imageWidth, int imageHeight, int BytesPerPixel,int depth) {
	unsigned char pix_bl;
	double temp;
	if ((ht_flr == -1) && (wd_flr == -1)) {
          pix_bl = 0;
	}
	if (ht_flr == (imageHeight - 1) && wd_flr == (imageWidth - 1))
	{
		pix_bl = pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth];

	}
	//最后一行
	if (ht_flr == (imageHeight - 1) && wd_flr != (imageWidth - 1))
	{

		temp = (1 - delta_h) * (1 - delta_w) * pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth] +
			(1 - delta_h) * delta_w*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel + depth];
		pix_bl = temp;
	}//最后一列
	if (ht_flr != (imageHeight - 1) && wd_flr == (imageWidth - 1))
	{

		temp = (1 - delta_w) * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth] +
			delta_h * (1 - delta_w)*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth];
		pix_bl = temp;
	}
	if (ht_flr != (imageHeight - 1) && wd_flr != (imageWidth - 1) && (ht_flr>=0) && (wd_flr>=0)) {
		temp = (1 - delta_h)*(1 - delta_w)*pix[ht_flr*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth] +
			delta_w * (1 - delta_h)*pix[ht_flr*imageWidth*BytesPerPixel + (wd_flr + 1)*BytesPerPixel + depth] +
			delta_h * (1 - delta_w)*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + wd_flr * BytesPerPixel + depth] +
			delta_h * delta_w*pix[(ht_flr + 1)*imageWidth*BytesPerPixel + (wd_flr + 1) * BytesPerPixel + depth];
		pix_bl = temp;
	}
	return pix_bl;
}
int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	double arct,theta,radius, scale;
	unsigned char tempr, tempg, tempb;
	vector<unsigned char> pix;
	vector<unsigned char> pix_polar;
	double cart_x, cart_y;
	double x_map, y_map, delta_h, delta_w, ratio, k;
	int ht_flr, wd_flr;
	int part_imageHeight, part_imageWidth;
	vector<pair<unsigned char, int> > pix1_rp;
	vector<pair<unsigned char, int> > pix1_gp;
	vector<pair<unsigned char, int> > pix1_bp;
	vector<pair<unsigned char, int> > pix2_rp;
	vector<pair<unsigned char, int> > pix2_gp;
	vector<pair<unsigned char, int> > pix2_bp;
	vector<pair<unsigned char, int> > pix3_rp;
	vector<pair<unsigned char, int> > pix3_gp;
	vector<pair<unsigned char, int> > pix3_bp;
	vector<pair<unsigned char, int> > pix4_rp;
	vector<pair<unsigned char, int> > pix4_gp;
	vector<pair<unsigned char, int> > pix4_bp;
	vector<unsigned char>pix_out1, pix_out2, pix_out3, pix_out4;
	//vector<pair<float, float>> cate2_index;
	//vector<pair<float, float>> cate3_index;
	//vector<pair<float, float>> cate4_index;
	vector<unsigned char>pix1,pix2,pix3,pix4,pix_out;
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
	part_imageHeight = imageHeight / 2;
	part_imageWidth = imageWidth / 2;
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_out1.resize(part_imageWidth*part_imageHeight*BytesPerPixel, 0);
	pix_out2.resize(part_imageWidth*part_imageHeight*BytesPerPixel, 0);
	pix_out3.resize(part_imageWidth*part_imageHeight*BytesPerPixel, 0);
	pix_out4.resize(part_imageWidth*part_imageHeight*BytesPerPixel, 0);
	pix_out.resize(imageWidth*imageHeight*BytesPerPixel, 0);

	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";
	for (int r = 0; r < imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++) {
			if ((r < 256) && (c < 256)) {
				pix1_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
				pix1_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
				pix1_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));
			}
			if ((r < 256) && (c >= 256)) {
				pix2_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
				pix2_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
				pix2_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));
				
			}
			if ((r >= 256) && (c < 256)) {
				pix3_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
				pix3_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
				pix3_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));
				
			}
			if ((r >= 256) && (c >= 256)) {
				pix4_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
				pix4_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
				pix4_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));
				

			}
		}
	}
	for (int r = 0; r < 256; r++) {
		for (int c = 0; c < 256; c++) {

			pix1.push_back(pix1_rp[r * 256 + c].first);
			pix1.push_back(pix1_gp[r * 256 + c].first);
			pix1.push_back(pix1_bp[r * 256 + c].first);	 
			

			pix2.push_back(pix2_rp[r * 256 + c].first);
			pix2.push_back(pix2_gp[r * 256 + c].first);
			pix2.push_back(pix2_bp[r * 256 + c].first);

			pix3.push_back(pix3_rp[r * 256 + c].first);
			pix3.push_back(pix3_gp[r * 256 + c].first);
			pix3.push_back(pix3_bp[r * 256 + c].first);

			pix4.push_back(pix4_rp[r * 256 + c].first);
			pix4.push_back(pix4_gp[r * 256 + c].first);
			pix4.push_back(pix4_bp[r * 256 + c].first);

		}
	}

	radius = 255;
	double d, x_edge, y_edge, y;
	// part1
	for (int r = 0; r < part_imageHeight; r++) {
		for (int c = 0; c <part_imageWidth; c++) {
			cart_y = 255 - r;
			cart_x = 255 - c;
			x_edge = sqrt(pow(255, 2) - pow(cart_y, 2));
			ratio = x_edge / radius;
			if (cart_x == 0) {
				x_map = 255;
			}
			else
			{
				x_map = 255 - cart_x / ratio;
			}
			y_map = r;
			if (x_map < 0) {
				pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = 0;
				pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = 0;
				pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = 0;
				continue;
			}
			ht_flr = floor(y_map);
			wd_flr = floor(x_map);
			delta_h = y_map - ht_flr;
			delta_w = x_map - wd_flr;
			tempr = bilinearInterpolation(pix1, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 0);
			tempg = bilinearInterpolation(pix1, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 1);
			tempb = bilinearInterpolation(pix1, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 2);
			pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = tempr;
			pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+1] = tempg;
			pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+2] = tempb;
		}
	}
	//part2
	for (int r = 0; r < part_imageHeight; r++) {
		for (int c = 0; c <part_imageWidth; c++) {
			cart_y = 255 - r;
			cart_x = c;
			x_edge = sqrt(pow(255, 2) - pow(cart_y, 2));
			ratio = x_edge / radius;
			if (cart_x == 0) {
				x_map = 0;
			}
			else
			{
				x_map = cart_x / ratio;
			}
			y_map = r;
			if (x_map > 255) {
				pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = 0;
				pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = 0;
				pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = 0;
				continue;
			}
			ht_flr = floor(y_map);
			wd_flr = floor(x_map);
			delta_h = y_map - ht_flr;
			delta_w = x_map - wd_flr;
			tempr = bilinearInterpolation(pix2, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 0);
			tempg = bilinearInterpolation(pix2, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 1);
			tempb = bilinearInterpolation(pix2, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 2);
			pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = tempr;
			pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+1] = tempg;
			pix_out2[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+2] = tempb;
		}
	}
	//part3
	for (int r = 0; r < part_imageHeight; r++) {
		for (int c = 0; c <part_imageWidth; c++) {
			cart_y =  r;
			cart_x = 255 - c;
			x_edge = sqrt(pow(255, 2) - pow(cart_y, 2));
			ratio = x_edge / radius;
			if (cart_x == 0) {
				x_map = 255;
			}
			else
			{
				x_map = 255 - cart_x / ratio;
			}
			y_map = r;
			if (x_map <0) {
				pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = 0;
				pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = 0;
				pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = 0;
				continue;
			}
			ht_flr = floor(y_map);
			wd_flr = floor(x_map);
			delta_h = y_map - ht_flr;
			delta_w = x_map - wd_flr;
			tempr = bilinearInterpolation(pix3, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 0);
			tempg = bilinearInterpolation(pix3, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 1);
			tempb = bilinearInterpolation(pix3, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 2);
			pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = tempr;
			pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+1] = tempg;
			pix_out3[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+2] = tempb;
		}
	}
	//part4
	for (int r = 0; r < part_imageHeight; r++) {
		for (int c = 0; c <part_imageWidth; c++) {
			cart_y = r;
			cart_x = c;
			x_edge = sqrt(pow(255, 2) - pow(cart_y, 2));
			ratio = x_edge / radius;
			if (cart_x == 0) {
				x_map = 0;
			}
			else
			{
				x_map = cart_x / ratio;
			}
			y_map = r;
			if (x_map > 255) {
				pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = 0;
				pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = 0;
				pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = 0;
				continue;
			}
			ht_flr = floor(y_map);
			wd_flr = floor(x_map);
			delta_h = y_map - ht_flr;
			delta_w = x_map - wd_flr;
			tempr = bilinearInterpolation(pix4, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 0);
			tempg = bilinearInterpolation(pix4, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 1);
			tempb = bilinearInterpolation(pix4, ht_flr, wd_flr, delta_h, delta_w, 256, 256, 3, 2);
			pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel] = tempr;
			pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+1] = tempg;
			pix_out4[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+2] = tempb;
		}
	}
	// combine together:

	for (int r = 0; r < imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++) {
			if ((r < 256) && (c < 256)) {
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel+1] = pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+1];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel+2] = pix_out1[r*part_imageWidth*BytesPerPixel + c * BytesPerPixel+2];

			}
			if ((r < 256) && (c >= 256)) {
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel]     = pix_out2[r*part_imageWidth*BytesPerPixel + (c-256) * BytesPerPixel];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_out2[r*part_imageWidth*BytesPerPixel + (c-256) * BytesPerPixel + 1];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_out2[r*part_imageWidth*BytesPerPixel + (c-256) * BytesPerPixel + 2];

			}
			if ((r >= 256) && (c < 256)) {
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_out3[(r-256)*part_imageWidth*BytesPerPixel + c * BytesPerPixel];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_out3[(r-256)*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 1];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_out3[(r-256)*part_imageWidth*BytesPerPixel + c * BytesPerPixel + 2];
			}
			if ((r >= 256) && (c >= 256)) {
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel]  =    pix_out4[(r-256)*part_imageWidth*BytesPerPixel + (c-256) * BytesPerPixel];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_out4[(r-256)*part_imageWidth*BytesPerPixel + (c - 256) * BytesPerPixel + 1];
				pix_out[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_out4[(r-256)*part_imageWidth*BytesPerPixel + (c - 256) * BytesPerPixel + 2];
			}
		}
	}

	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_out[0], sizeof(unsigned char), imageHeight*imageWidth*BytesPerPixel, outputFile);
	fclose(outputFile);
	
	return 0;
}
