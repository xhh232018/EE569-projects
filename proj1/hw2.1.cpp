// HW2.1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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

int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	vector<unsigned char> pix;
	vector<pair<unsigned char, int>> pix_rp;
	vector<pair<unsigned char, int>> pix_gp;
	vector<pair<unsigned char, int>> pix_bp;
    float red_pdf[256] = { 0 };
	float green_pdf[256] = { 0 };
	float blue_pdf[256] = { 0 };
	float red_cdf[256] = { 0.0 };
	float green_cdf[256] = { 0.0 };
	float blue_cdf[256] = { 0.0 };
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
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			red_pdf[(int)pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel]] += 1;
			green_pdf[(int)pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+1]] += 1 ;
			blue_pdf[(int)pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+2]] += 1 ;
			/*pix_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
			pix_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
			pix_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));*/

		}
	}
	sort(pix_rp.begin(), pix_rp.end());
	/*for (int k = 0; k < 256; k++) {
		red_pdf[k] = red_pdf[k] / (imageHeight*imageWidth);
		green_pdf[k] = green_pdf[k] / (imageHeight*imageWidth);
		blue_pdf[k] = blue_pdf[k] / (imageHeight*imageWidth);
	}*/
	cout << "record success!";
	FILE *fp = fopen("redpdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", red_pdf[i]);
	}
	fclose(fp);

    fp = fopen("greenpdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", green_pdf[i]);
	}
	fclose(fp);

	fp = fopen("bluepdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", blue_pdf[i]);
	}
	fclose(fp);
	red_cdf[0] = red_pdf[0];
	green_cdf[0] = green_pdf[0];
	blue_cdf[0] = blue_pdf[0];
	for (int m = 1; m < 256; m++) {
		red_cdf[m] = red_cdf[m-1] + red_pdf[m];
		green_cdf[m] = green_cdf[m-1] + green_pdf[m];
		blue_cdf[m] = blue_cdf[m-1] + blue_pdf[m];
	}
	fp = fopen("redcdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", red_cdf[i]);
	}
	fclose(fp);

	fp = fopen("greencdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", green_cdf[i]);
	}
	fclose(fp);

	fp = fopen("bluecdf.txt", "w");
	for (int i = 0; i<256; i++)
	{
		fprintf(fp, "%f\n", blue_cdf[i]);
	}
	fclose(fp);
	for (int row = 0; row < imageHeight; row++)
	{
		for (int col = 0; col < imageWidth; col++)
		{
			pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel] = (unsigned char) round(red_cdf[(int)pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel]] * 255);
			pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel+1] = (unsigned char)round(green_cdf[(int)pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel+1]] * 255);
			pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel + 2] = (unsigned char)round(blue_cdf[(int)pix[row*imageWidth*BytesPerPixel + col * BytesPerPixel+2]] * 255);

		}
	}
	FILE* outputFile1;
	if (!(outputFile1 = fopen(outputFileName.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile1);
	fclose(outputFile1);
	return 0;
}
