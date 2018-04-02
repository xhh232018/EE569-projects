#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
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
	int imageWidth_rf;
	int imageHeight_rf;
	vector<unsigned char> pix;
	vector<unsigned char> pix_rf;
	vector<unsigned char> pix_r;
	vector<unsigned char> pix_g;
	vector<unsigned char> pix_b;
	vector<pair<unsigned char, int> > pix_rp;
	vector<pair<unsigned char, int> > pix_gp;
	vector<pair<unsigned char, int> > pix_bp;
	//float red_pdf[256] = { 0.0 };
	//float green_pdf[256] = { 0.0 };
	//float blue_pdf[256] = { 0.0 };
	float rfred_pdf[256] = { 0.0 };
	float rfgreen_pdf[256] = { 0.0 };
	float rfblue_pdf[256] = { 0.0 };
	int rbin_size[256] = { 0 };
	int gbin_size[256] = { 0 };
	int bbin_size[256] = { 0 };
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string inputRfname = argv[6];
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
			imageWidth_rf = atoi(argv[7]);
			imageHeight_rf = atoi(argv[8]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_rf.resize(imageWidth_rf*imageHeight_rf*BytesPerPixel, 0);
	pix_r.resize(imageHeight*imageWidth, 0);
	pix_g.resize(imageHeight*imageWidth, 0);
	pix_b.resize(imageHeight*imageWidth, 0);
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	if (!(inputFile = fopen(inputRfname.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix_rf[0], sizeof(unsigned char), imageWidth_rf*imageHeight_rf*BytesPerPixel, inputFile);
	fclose(inputFile);
	cout << "loading data success!";
	//mirror
	for (int r = 0; r < imageHeight; r++)
	{
		for (int c = 0; c < (imageWidth / 2); c++)
		{
			unsigned char temp_r, temp_g, temp_b;
			temp_r = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 3) - c * BytesPerPixel];
			pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 3) - c * BytesPerPixel] = temp_r;
			temp_g = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 2) - c * BytesPerPixel];
			pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 2) - c * BytesPerPixel] = temp_g;
			temp_b = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 1) - c * BytesPerPixel];
			pix[r*imageWidth*BytesPerPixel + (imageWidth * BytesPerPixel - 1) - c * BytesPerPixel] = temp_b;

		}
	}
	//minus
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel] = 255 - pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel];
			pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel + 1] = 255 - pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel + 1];
			pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel + 2] = 255 - pix[i*imageWidth*BytesPerPixel + j * BytesPerPixel + 2];

		}
	}

	for (int r = 0; r <imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++) {
			pix_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
			pix_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
			pix_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));


		}
	}
	for (int r = 0; r< imageHeight_rf; r++) {
		for (int c = 0; c<imageWidth_rf; c++) {
			rfred_pdf[(int)pix_rf[r*imageWidth_rf*BytesPerPixel + c * BytesPerPixel]] += 1.0 / (imageHeight_rf * imageWidth_rf);
			rfgreen_pdf[(int)pix_rf[r*imageWidth_rf*BytesPerPixel + c * BytesPerPixel + 1]] += 1.0 / (imageHeight_rf * imageWidth_rf);
			rfblue_pdf[(int)pix_rf[r*imageWidth_rf*BytesPerPixel + c * BytesPerPixel + 2]] += 1.0 / (imageHeight_rf * imageWidth_rf);
		}
	}
	for (int i = 0; i < 256; i++) {
		rbin_size[i] = round(rfred_pdf[i] * imageHeight* imageWidth);
		gbin_size[i] = round(rfgreen_pdf[i] * imageHeight* imageWidth);
		bbin_size[i] = round(rfblue_pdf[i] * imageHeight* imageWidth);

	}
	sort(pix_rp.begin(), pix_rp.end());
	sort(pix_gp.begin(), pix_gp.end());
	sort(pix_bp.begin(), pix_bp.end());
	//FILE* outputFile2;
	//if (!(outputFile2 = fopen(outputFileName.c_str(), "wb"))) {
	//	cout << "Cannot open file: ";
	//	exit(1);
	//}
	////    cout<< (int)pixelData[0] << endl;
	//fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile2);
	//fclose(outputFile2);
	//FILE *fp = fopen("redpdf_film.txt", "w");
	//for (int i = 0; i<256; i++)
	//{
	//	fprintf(fp, "%f\n", red_pdf[i]);
	//}
	//fclose(fp);

	//fp = fopen("greenpdf_film.txt", "w");
	//for (int i = 0; i<256; i++)
	//{
	//	fprintf(fp, "%f\n", green_pdf[i]);
	//}
	//fclose(fp);

	//fp = fopen("bluepdf_film.txt", "w");
	//for (int i = 0; i<256; i++)
	//{
	//	fprintf(fp, "%f\n", blue_pdf[i]);
	//}
	//fclose(fp);
	int count_r = 0;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < rbin_size[i]; j++) {
			if ((count_r + j) >= imageHeight * imageWidth) {
				break;
			}
			pix_rp[count_r + j].first = (unsigned char)i;
		}
		count_r += rbin_size[i];
	}
	if (count_r < pix_rp.size()) {
		for (int i = 0; i < (pix_rp.size() - count_r); i++) {
			pix_rp[count_r + i].first = (unsigned char)255;
		}
	}
	int count_g = 0;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < gbin_size[i]; j++) {
			if ((count_g + j) >= imageHeight * imageWidth) {
				break;
			}
			pix_gp[count_g + j].first = (unsigned char)i;
		}
		count_g += gbin_size[i];
	}
	int count_b = 0;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < bbin_size[i]; j++) {
			if ((count_b + j) >= imageHeight * imageWidth) {
				break;
			}
			pix_bp[count_b + j].first = (unsigned char)i;
		}
		count_b += bbin_size[i];
	}
	for (int i = 0; i < imageHeight*imageWidth; i++) {
		pix_r[pix_rp[i].second] = pix_rp[i].first;
		pix_g[pix_gp[i].second] = pix_gp[i].first;
		pix_b[pix_bp[i].second] = pix_bp[i].first;

	}
	for (int r = 0; r < imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++)
		{
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_r[r*imageWidth + c];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_g[r*imageWidth + c];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_b[r*imageWidth + c];

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
