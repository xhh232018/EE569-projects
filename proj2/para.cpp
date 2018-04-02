#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>

using namespace std;
bool judgeOut(double x, double y, int imageWidth, int imageHeight) {
	if (x< 0.5 || y < 0.5 || x>(imageWidth - 0.5) || (y > imageHeight - 0.5)) {
		return true;
	}
	else
	{
		return false;
	}
}
bool judgeInori(double x, double y, int imageWidth, int imageHeight) {
	if ((x>0 && x<imageWidth) && (y>0) && (y<imageHeight)) {
		return true;
	}
	else
	{
		return false;
	}
}
unsigned char bilinearInterpolation(vector<unsigned char> pix, int ht_flr, int wd_flr, double delta_h, double delta_w, int imageWidth, int imageHeight, int BytesPerPixel, int depth) {
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
	int newHeight, newWidth, imageWidth, imageWidth1;
	int imageHeight1, imageHeight, r_index, c_index;
	int ht_flr, wd_flr;
	unsigned char tempr, tempg, tempb;
	vector <unsigned char> pix_md, pix_rt, pix_lf, pix_out, pix_out1;
	double xup_prm, yup_prm, xdn_prm, ydn_prm, x_prm, y_prm;
	double cart_xup, cart_yup, cart_xdn, cart_ydn;
	double w_prime, w_up, w_dn, x_up, x_dn, y_up, y_dn, x_max, x_min, y_max, y_min;
	double x_map, y_map, r_map, c_map, delta_h, delta_w;
	double H[3][3] = { { 0.8289,0.0091,204.0417 },{ -0.1621,0.9431,21.2156 },{ -0.00034,0.000006,1 } };
	double H_inv[3][3] = { { 1.1116,-0.0092,-226.6124 },{ 0.1825,1.0590,-59.7150 },{ 0.00037746,-0.0000099345,0.9232 } };
	double H1[3][3] = { { 1.41563566482423 , -0.0175884359008771 ,-368.262180342288 },{ 0.344955687771382,	1.33583458936253, -32.5514130837301 },{ 0.000858449842567264,	4.01966263460834e-05,	1 } };
	double H1_inv[3][3] = { { 0.577184885664370,	0.00120239252335920, 212.594504031092 },{ -0.160964021822413,0.747527821518444 ,-34.9438747231576 },{ -0.000489014063590424, -3.10802901972604e-05,0.818902907359175 } };
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string inputFileName1 = argv[2];
	string inputFileName2 = argv[3];
	string outputFileName1 = argv[7];
	// Check if image is grayscale or color
	if (argc < 4) {
		BytesPerPixel = 1;// default is grey image
		imageHeight = 256;
		imageWidth = 256;
	}
	else {
		BytesPerPixel = atoi(argv[4]);
		// Check if size is specified
		if (argc >= 5) {
			imageWidth = atoi(argv[5]);
			imageHeight = atoi(argv[6]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix_md.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_rt.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_lf.resize(imageWidth*imageHeight*BytesPerPixel, 0);

	//input
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix_md[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);

	if (!(inputFile = fopen(inputFileName1.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix_rt[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);

	if (!(inputFile = fopen(inputFileName2.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix_lf[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	cout << "loading data success!";

	//中间和右边配准
	cart_xup = imageWidth - 0.5;
	cart_yup = imageHeight - 0.5;
	cart_xdn = imageWidth - 0.5;
	cart_ydn = 0.5;
	xup_prm = H[0][0] * cart_xup + H[0][1] * cart_yup + H[0][2] * 1;
	yup_prm = H[1][0] * cart_xup + H[1][1] * cart_yup + H[1][2] * 1;
	w_up = H[2][0] * cart_xup + H[2][1] * cart_yup + H[2][2] * 1;
	xdn_prm = H[0][0] * cart_xdn + H[0][1] * cart_ydn + H[0][2] * 1;
	ydn_prm = H[1][0] * cart_xdn + H[1][1] * cart_ydn + H[1][2] * 1;
	w_dn = H[2][0] * cart_xdn + H[2][1] * cart_ydn + H[2][2] * 1;
	x_up = xup_prm / w_up;
	y_up = yup_prm / w_up;
	x_dn = xdn_prm / w_dn;
	y_dn = ydn_prm / w_dn;
	x_max = round(x_up > x_dn ? x_up : x_dn) + 0.5;
	x_min = 0.5;
	y_max = round(y_up) + 0.5;
	y_min = floor(y_dn) - 0.5;
	newHeight = y_max - y_min + 1;
	newWidth = x_max - x_min + 1;
	pix_out.resize(newHeight*newWidth*BytesPerPixel, 0);
	//根据得到的范围x_min x_max y_min y_max 和反变换矩阵h_inv算出对应在右侧的图的坐标 如果出界 则赋值为0（黑色） 如果在界内 进行双线性插值
	for (double x = x_min; x <= x_max; x++) {
		for (double y = y_min; y <= y_max; y++) {
			r_index = (newHeight - 0.5) - (y - y_min + 0.5);
			c_index = x - 0.5;
			w_prime = H_inv[2][0] * x + H_inv[2][1] * y + H_inv[2][2];
			x_prm = H_inv[0][0] * x + H_inv[0][1] * y + H_inv[0][2];
			y_prm = H_inv[1][0] * x + H_inv[1][1] * y + H_inv[1][2];
			x_map = x_prm / w_prime;
			y_map = y_prm / w_prime;
			//在右图之外 但在中图之内---直接用中图的值
			if (judgeOut(x_map, y_map, imageWidth, imageHeight) && judgeInori(x, y, imageWidth, imageHeight)) {
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = pix_md[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + c_index * BytesPerPixel];
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = pix_md[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + c_index * BytesPerPixel + 1];
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = pix_md[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + c_index * BytesPerPixel + 2];
				continue;
			}
			//在右图之外 在中图之外 全赋0
			if (judgeOut(x_map, y_map, imageWidth, imageHeight) && (!judgeInori(x, y, imageWidth, imageHeight))) {
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = 0;
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = 0;
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = 0;
				continue;
			}
			//在右图之内 双线性插值
			if (!judgeOut(x_map, y_map, imageWidth, imageHeight)) {
				r_map = imageHeight - y_map - 0.5;
				c_map = x_map - 0.5;
				ht_flr = floor(r_map);
				wd_flr = floor(c_map);
				delta_h = r_map - ht_flr;
				delta_w = c_map - wd_flr;
				tempr = bilinearInterpolation(pix_rt, ht_flr, wd_flr, delta_h, delta_w, 480, 640, 3, 0);
				tempg = bilinearInterpolation(pix_rt, ht_flr, wd_flr, delta_h, delta_w, 480, 640, 3, 1);
				tempb = bilinearInterpolation(pix_rt, ht_flr, wd_flr, delta_h, delta_w, 480, 640, 3, 2);
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = tempr;
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = tempg;
				pix_out[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = tempb;
			}
		}

	}
	imageHeight1 = imageHeight;
	imageWidth1 = imageWidth;
	imageHeight = newHeight;
	imageWidth = newWidth;


	// output后的图像 和左边配准
	cart_xup = 0.5;
	cart_yup = imageHeight - 0.5;
	cart_xdn = 0.5;
	cart_ydn = 0.5;
	xup_prm = H1[0][0] * cart_xup + H1[0][1] * cart_yup + H1[0][2] * 1;
	yup_prm = H1[1][0] * cart_xup + H1[1][1] * cart_yup + H1[1][2] * 1;
	w_up = H1[2][0] * cart_xup + H1[2][1] * cart_yup + H1[2][2] * 1;
	xdn_prm = H1[0][0] * cart_xdn + H1[0][1] * cart_ydn + H1[0][2] * 1;
	ydn_prm = H1[1][0] * cart_xdn + H1[1][1] * cart_ydn + H1[1][2] * 1;
	w_dn = H1[2][0] * cart_xdn + H1[2][1] * cart_ydn + H1[2][2] * 1;
	x_up = xup_prm / w_up;
	y_up = yup_prm / w_up;
	x_dn = xdn_prm / w_dn;
	y_dn = ydn_prm / w_dn;
	x_max = newWidth - 0.5;
	x_min = round(x_up < x_dn ? x_up : x_dn) - 0.5;
	y_max = round(y_up) + 0.5;
	y_min = round(y_dn) - 0.5;
	newHeight = y_max - y_min + 1;
	newWidth = x_max - x_min + 1;
	pix_out1.resize(newHeight*newWidth*BytesPerPixel, 0);
        cout<< newHeight<<endl;
        cout<< newWidth<<endl;
	//配准
	for (double x = x_min; x <= x_max; x++) {
		for (double y = y_min; y <= y_max; y++) {
			r_index = (newHeight - 0.5) - (y - y_min + 0.5);
			c_index = x - x_min;
			w_prime = H1_inv[2][0] * x + H1_inv[2][1] * y + H1_inv[2][2];
			x_prm = H1_inv[0][0] * x + H1_inv[0][1] * y + H1_inv[0][2];
			y_prm = H1_inv[1][0] * x + H1_inv[1][1] * y + H1_inv[1][2];
			x_map = x_prm / w_prime;
			y_map = y_prm / w_prime;
			//在左图之外 但在中图之内---直接用中图的值
			if (judgeOut(x_map, y_map, imageWidth1, imageHeight1) && judgeInori(x, y, imageWidth, imageHeight)) {
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = pix_out[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + (x - 0.5) * BytesPerPixel];
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = pix_out[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + (x - 0.5) * BytesPerPixel + 1];
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = pix_out[(imageHeight - y - 0.5)*imageWidth*BytesPerPixel + (x - 0.5) * BytesPerPixel + 2];
				continue;
			}
			//在左图之外 在中图之外 全赋0
			if (judgeOut(x_map, y_map, imageWidth1, imageHeight1) && (!judgeInori(x, y, imageWidth, imageHeight))) {
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = 0;
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = 0;
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = 0;
				continue;
			}
			//在左图之内 双线性插值
			if (!judgeOut(x_map, y_map, imageWidth1, imageHeight1)) {
				r_map = imageHeight1 - y_map - 0.5;
				c_map = x_map - 0.5;
				ht_flr = floor(r_map);
				wd_flr = floor(c_map);
				delta_h = r_map - ht_flr;
				delta_w = c_map - wd_flr;
				tempr = bilinearInterpolation(pix_lf, ht_flr, wd_flr, delta_h, delta_w, imageWidth1, imageHeight1, 3, 0);
				tempg = bilinearInterpolation(pix_lf, ht_flr, wd_flr, delta_h, delta_w, imageWidth1, imageHeight1, 3, 1);
				tempb = bilinearInterpolation(pix_lf, ht_flr, wd_flr, delta_h, delta_w, imageWidth1, imageHeight1, 3, 2);
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel] = tempr;
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 1] = tempg;
				pix_out1[r_index*newWidth*BytesPerPixel + c_index * BytesPerPixel + 2] = tempb;
			}
		}
		cout << c_index << "列完毕" << endl;
	}
       cout<<"new width is " << newWidth<<endl;
       cout<<"new height is " << newHeight<<endl;



	//output 
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_out1[0], sizeof(unsigned char), newWidth*newHeight*BytesPerPixel, outputFile);
	fclose(outputFile);
	return 0;
}

