// sobel.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;
vector<vector<double>> extension(vector<double> db, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> extimg1;
	extimg1.resize(imageHeight + 2 * floor(N1 / 2));
	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg1[i].resize(imageWidth + 2 * floor(N1 / 2));
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r + N1 / 2][c + N1 / 2] = db[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r][c + N1 / 2] = db[(N1 / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg1[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = db[(imageHeight - 1 - N1 / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N1 / 2); r++) {
		for (int c = 0; c < N1 / 2; c++) {
			extimg1[r][c] = extimg1[r][N1 - 1 - c];
			extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - c] = extimg1[r][imageWidth + 2 * floor(N1 / 2) - 1 - 2 * floor(N1 / 2) + c];
		}
	}
	return extimg1;
}
vector<vector<double>> convolution(vector<vector<double>> db_ext, double L_F[3][3] , int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> pixel_fl;
	pixel_fl.resize(imageHeight);
	for (int i = 0; i < imageHeight; i++) {
		pixel_fl[i].resize(imageWidth);
	}
	double temp = 0.0;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			for (int m = -N1 / 2; m <=N1 / 2; m++) {
				for (int n = -N1 / 2; n <= N1 / 2; n++) {
					temp += db_ext[i + N1 / 2 + m][j + N1 / 2+n] * L_F[m + N1 / 2][n + N1 / 2];
				}
			}
			pixel_fl[i][j] = temp;
			temp = 0;
		}
	}
	return pixel_fl;
}
int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int nbpp=1;
	int imageWidth;
	int imageHeight;
	int N = 3;
	double sobel_x[3][3] = { {1.0 / 4, 0, -1.0 / 4 },{ 2.0 / 4, 0, -2.0 / 4 },{ 1.0 / 4, 0, -1.0 / 4 } };
	double sobel_y[3][3] = { {-1.0 / 4, -2.0 / 4, -1.0 / 4},{0,0,0},{1.0/4, 2.0/4, 1.0/4} };
	vector<vector<double>> pix_dx2d;
	vector<vector<double>> pix_dy2d;
	vector<vector<double>> pix_mag2d;
	vector<vector<double>> extimg;
	vector<unsigned char>pix,pix_dxout,pix_dyout,pix_nm;
	vector<pair<double, int>> pix_pair, pix_pairx, pix_pairy;
	vector<double> pix_gray;
	vector<double> pix_dx;
	vector<double> pix_dy;
	vector<double> pix_mag;
	double max_dx,max_dy,min_dx,min_dy,max, min,thrs;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	string outputFileName2 = argv[7];
	string outputFileName3 = argv[8];
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
			thrs = atof(argv[6]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_dx.resize(imageWidth*imageHeight, 0);
	pix_dy.resize(imageWidth*imageHeight, 0);
	pix_dxout.resize(imageWidth*imageHeight*nbpp, 0);
	pix_dyout.resize(imageWidth*imageHeight*nbpp, 0);
	pix_mag.resize(imageWidth*imageHeight*nbpp, 0);
	pix_nm.resize(imageWidth*imageHeight*nbpp, 0);
	pix_dx2d.resize(imageHeight);
	pix_dy2d.resize(imageHeight);
	pix_mag2d.resize(imageHeight);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	
	for (int i = 0; i < imageHeight; i++) {
		pix_dx2d[i].resize(imageWidth);
		pix_dy2d[i].resize(imageWidth);
		pix_mag2d[i].resize(imageWidth);
	}

	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
	}	

	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	cout << "loading data success!";
	//RGB TO GRAY
	for (int index = 0; index < imageWidth*imageHeight*BytesPerPixel; index = index + 3) {

		pix_gray.push_back((double)(0.21*pix[index] + 0.72*pix[index + 1] + 0.07*pix[index + 2]));
		
	}
	//img extension
	extimg = extension(pix_gray, imageHeight, imageWidth, nbpp, N);
	//sobel
	pix_dx2d = convolution(extimg, sobel_x, imageHeight, imageWidth, nbpp, N);
	pix_dy2d = convolution(extimg, sobel_y, imageHeight, imageWidth, nbpp, N);
	//输出dx dy
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_dx[i*imageWidth + j] = pix_dx2d[i][j];
			pix_dy[i*imageWidth + j] = pix_dy2d[i][j];
			pix_pairx.push_back(make_pair(pix_dx[i*imageWidth + j], i*imageWidth + j));
			pix_pairy.push_back(make_pair(pix_dy[i*imageWidth + j], i*imageWidth + j));
		}
	}
	sort(pix_pairx.begin(), pix_pairx.end());
	sort(pix_pairy.begin(), pix_pairy.end());
	min_dx = pix_pairx.front().first;
	max_dx = pix_pairx.back().first;
	min_dy = pix_pairy.front().first;
	max_dy = pix_pairy.back().first;
	
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_dxout[i*imageWidth + j] = (unsigned char) 255.0*((pix_dx[i*imageWidth + j] - min_dx) / (max_dx - min_dx));
			pix_dyout[i*imageWidth + j] = (unsigned char) 255.0*((pix_dy[i*imageWidth + j] - min_dy) / (max_dy - min_dy));
		}
	}

	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_mag[i*imageWidth + j] = sqrt(pix_dx2d[i][j] * pix_dx2d[i][j] + pix_dy2d[i][j] * pix_dy2d[i][j]);
			pix_pair.push_back(make_pair(pix_mag[i*imageWidth + j], i*imageWidth + j));
		}
	}
	
	
	sort(pix_pair.begin(), pix_pair.end());

	for (int i = 0; i < pix_pair.size(); i++) {
		if (i <= pix_pair.size()*thrs) {
			pix[pix_pair[i].second] = 255;
		}
		else
		{
			pix[pix_pair[i].second] = 0;
		}
	}

	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);

	if (!(outputFile = fopen(outputFileName2.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_dxout[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);

	if (!(outputFile = fopen(outputFileName3.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_dyout[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);

	fclose(outputFile);

	return 0;
}
