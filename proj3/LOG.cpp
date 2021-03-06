// LOG.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define pi 3.1415926
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
vector<vector<int>> extension_2D(vector<vector<int>> db, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<int>> extimg1;
	extimg1.resize(imageHeight + 2 * floor(N1 / 2));
	for (int i = 0; i < imageHeight + 2 * floor(N1 / 2); i++) {
		extimg1[i].resize(imageWidth + 2 * floor(N1 / 2));
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r + N1 / 2][c + N1 / 2] = db[r][c];
		}
	}
	for (int r = 0; r < N1 / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg1[r][c + N1 / 2] = db[(N1 / 2 - r)][c];
			extimg1[imageHeight + 2 * floor(N1 / 2) - 1 - r][c + N1 / 2] = db[(imageHeight - 1 - N1 / 2 + r)][c];
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
vector<vector<double>> LOG_generator(int N, double sigma) {
	vector<vector<double>> LOG;
	LOG.resize(N);
	for (int i = 0; i < N; i++) {
		LOG[i].resize(N);
	}
	for (int m = 0; m <N; m++) {
		for (int n = 0; n <N; n++) {
			LOG[m][n] = ((pow((m-N/2), 2) + pow((n-N/2), 2) - 2 * pow(sigma, 2)) / pow(sigma, 4))*exp(-(pow((m - N / 2), 2) + pow((n-N/2), 2)) / (2 * pow(sigma, 2)));
			/*LOG[m][n] = -1 / (pi*pow(sigma, 4)) * 
				      (1 - (pow(m - N / 2, 2) + pow(n - N / 2, 2)) / (2 * pow(sigma, 2)))
				      *exp(-(pow(m - N / 2, 2) + pow(n - N / 2, 2)) / (2 * pow(sigma, 2)));*/
		}
	}
	return LOG;
}
vector<vector<double>> Gaussian_generator(int N, double sigma) {
	vector<vector<double>> Gau;
	double sum = 0;
	Gau.resize(N);
	for (int i = 0; i < N; i++) {
		Gau[i].resize(N);
	}
	for (int m = 0; m <N; m++) {
		for (int n = 0; n <N; n++) {
			Gau[m][n] = (1 / (2 * pi*sigma*sigma))*exp((-(pow((m - N/ 2), 2) + pow((n - N / 2), 2))) / pow(sigma, 2));
			sum += Gau[m][n];
		}
	}
	for (int m = 0; m < N; m++) {
		for (int n = 0; n < N; n++)
		{
			Gau[m][n] = (1 / sum)*Gau[m][n];
			
		}
	}
	return Gau;
}
vector<vector<double>> convolution(vector<vector<double>> db_ext, vector<vector<double>> LOG, int imageHeight, int imageWidth, int BytesPerPixel, int N1) {
	vector<vector<double>> pixel_fl;
	pixel_fl.resize(imageHeight);
	for (int i = 0; i < imageHeight; i++) {
		pixel_fl[i].resize(imageWidth);
	}
	double temp = 0.0;
	double ts;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			for (int m = -N1 / 2; m <= N1 / 2; m++) {
				for (int n = -N1 / 2; n <= N1 / 2; n++) {
					ts = db_ext[i + N1 / 2 + m][j + N1 / 2 + n] * LOG[m + N1 / 2][n + N1 / 2];
					temp += db_ext[i + N1 / 2 + m][j + N1 / 2 + n] * LOG[m + N1 / 2][n + N1 / 2];
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
	int nbpp = 1;
	int imageWidth;
	int imageHeight;
	int N;
	int N_L = 3;
	double sigma;
	vector<vector<double>> LOG_kernel,Gaussian_kernel;
	vector<vector<double>> extimg,extimg2;
	vector<unsigned char>pix,pix_out,pix_cv;
	vector<pair<double, int>> pix_pair;
	vector<double> pix_gray, pix_conv1d;
	vector<vector<double>> pix_conv,pix_conv2;
	vector<vector<int>> pix_trival,exttri;
	vector<vector<double>> Lap;
	double max, min;  
	float pdf[256] = { 0 };
	double Laplace[3][3] = { { 0, -1.0 / 4, 0 },{ -1.0 / 4, 1, -1.0 / 4 },{ 0, -1.0 / 4, 0 } };
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	string outputFileName2 = argv[8];
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
			sigma = atof(argv[7]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_conv1d.resize(imageWidth*imageHeight);
	pix_trival.resize(imageHeight);
	pix_cv.resize(imageWidth*imageHeight);
	pix_out.resize(imageWidth*imageHeight*nbpp, 0);
	LOG_kernel.resize(N);
	Lap.resize(3);
	Gaussian_kernel.resize(N);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	extimg2.resize(imageHeight + 2 * floor(N_L / 2));
	exttri.resize(imageHeight + 2 * floor(N_L / 2));
	pix_conv.resize(imageHeight);
	pix_conv2.resize(imageHeight);
	for (int i = 0; i < 3;i++) {
		Lap[i].resize(3);
	}
	for (int i = 0; i < imageHeight; i++) {
		pix_conv[i].resize(imageWidth);
		pix_conv2[i].resize(imageWidth);
		pix_trival[i].resize(imageWidth);
	}
	for (int i = 0; i < N; i++) {
		LOG_kernel[i].resize(N);
		Gaussian_kernel[i].resize(N);
	}
	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
		
	}
	for (int i = 0; i < imageHeight + 2 * floor(N_L / 2); i++) {
		
		extimg2[i].resize(imageWidth + 2 * floor(N_L / 2));
		exttri[i].resize(imageWidth + 2 * floor(N_L / 2));
	
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
	extimg = extension(pix_gray, imageHeight, imageWidth, nbpp, N);
	LOG_kernel = LOG_generator(N, sigma);
	Gaussian_kernel = Gaussian_generator(N, sigma);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Lap[i][j] = Laplace[i][j];
		}
	}
	cout << "got kernel! ";

	//normalize kernel
	double sum = 0;
	double mean;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			sum += LOG_kernel[i][j];
		}
	}
	mean = sum / (N*N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			LOG_kernel[i][j]= LOG_kernel[i][j]-mean;
		}
	}

	//pix_conv = convolution(extimg, LOG_kernel, imageHeight, imageWidth, nbpp, N);
	
	//先高斯
	pix_conv = convolution(extimg, Gaussian_kernel, imageHeight, imageWidth, nbpp, N);
	//2d->1d
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_conv1d[i*imageWidth + j] = pix_conv[i][j];
		}
	}
    //拉普拉斯
	extimg2 = extension(pix_conv1d, imageHeight, imageWidth, nbpp, N_L);
	pix_conv = convolution(extimg2, Lap, imageHeight, imageWidth, nbpp, N_L);

	//2D->1D
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_conv1d[i*imageWidth + j] = pix_conv[i][j];
		}
	}

	sort(pix_conv1d.begin(), pix_conv1d.end());
	min = pix_conv1d.front();
	max = pix_conv1d.back();
	//shift
	if (min < 0) {
		for (int i = 0; i < imageHeight; i++) {
			for (int j = 0; j < imageWidth; j++) {
				pix_conv[i][j] = pix_conv[i][j]-min;
			}
		}
		max = max - min;
		min = 0;
	}
	//norm to 0-255
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {

			pix_cv[i*imageWidth + j] = (unsigned char) (255.0 * (pix_conv[i][j]/ max));
			pdf[pix_cv[i*imageWidth + j ]] += 1;
		}
	}
	FILE *fp = fopen("pdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", pdf[i]);
	}
	fclose(fp);
	cout << "record success!";
	//mapping into tri-values image
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			if (pix_cv[i*imageWidth + j] < 105 ) {
				pix_trival[i][j] = -1;
			}
			if (pix_cv[i*imageWidth + j] >= 105 && pix_cv[i*imageWidth + j] <= 112) {
				pix_trival[i][j] = 0;
			}
			if (pix_cv[i*imageWidth + j] > 112) {
				pix_trival[i][j] = 1;
			}
		}
	}
	exttri = extension_2D(pix_trival, imageHeight, imageWidth, nbpp, N_L);
	//zero crossing detection
	int count = 0;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
		
				for (int r1 = 0; r1 <= 2; r1++) {
					for (int c1 = 0; c1 <= 2; c1++) {
						if ((exttri[i + r1][j + c1] * exttri[N_L + i - r1 - 1][N_L + j - c1 -1]) == -1) {
							count++;
						}
					}
				}
			
			if (count > 0) {
				pix_out[i*imageWidth + j] = 0;
			}
			else
			{
				pix_out[i*imageWidth + j] = 255;
			}
			count = 0;
			/*if (pix_trival[i][j] == 0) {
				pix_out[i*imageWidth + j] = 255;
			}
			else
			{
				pix_out[i*imageWidth + j] = 0;
			}*/
		}
	}


	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}

	//    cout<< (int)pixelData[0] << endl;

	fwrite(&pix_out[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);
	if (!(outputFile = fopen(outputFileName2.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}

	//    cout<< (int)pixelData[0] << endl;

	fwrite(&pix_cv[0], sizeof(unsigned char), imageWidth*imageHeight*nbpp, outputFile);

	fclose(outputFile);

	return 0;
}
