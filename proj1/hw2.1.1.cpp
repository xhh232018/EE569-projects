// HW2.1.1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	vector<unsigned char> pix;
	vector<unsigned char> pix_r;
	vector<unsigned char> pix_g;
	vector<unsigned char> pix_b;
	vector<pair<unsigned char, int>> pix_rp;
	vector<pair<unsigned char, int>> pix_gp;
	vector<pair<unsigned char, int>> pix_bp;
	vector<unsigned char> pix_out;
	vector<int> r_index;
	vector<int> g_index;
	vector<int> b_index;
	float red_pdf[256] = { 0.0 };
	float green_pdf[256] = { 0.0 };
	float blue_pdf[256] = { 0.0 };
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
			imageHeight = atoi(argv[4]);
			imageWidth = atoi(argv[5]);
		}
		else {
			imageHeight = 256;
			imageWidth = 256;
		}
	}
	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	pix_r.resize(imageWidth*imageHeight, 0);
	pix_g.resize(imageWidth*imageHeight, 0);
	pix_b.resize(imageWidth*imageHeight, 0);
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!";

	int mod = (imageHeight*imageWidth) % 256;
	int avg = (imageHeight*imageWidth) / 256;
	for (int r = 0; r <imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++) {
			pix_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
			pix_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
			pix_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));


		}
	}
	
	sort(pix_rp.begin(), pix_rp.end());
	sort(pix_gp.begin(), pix_gp.end());
	sort(pix_bp.begin(), pix_bp.end());
	unsigned char pixv1 = 0;
	for (int index = 0; index < avg*(256-mod); index = index+avg) {
		for (int i = 0; i < avg; i++) {
			pix_rp[index + i].first = pixv1;
			pix_gp[index + i].first = pixv1;
			pix_bp[index + i].first = pixv1;
		}
		pixv1++;
	}
	unsigned char pixv = 256 - mod;
	for (int index = avg * (256 - mod); index < imageHeight*imageWidth; index = index+(avg+1)){
		for (int i = 0; i < avg + 1; i++) {
			pix_rp[index + i].first = pixv;
			pix_gp[index + i].first = pixv;
			pix_bp[index + i].first = pixv;
		}
		pixv++;
	}
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			red_pdf[(int)pix_rp[i*imageWidth + j ].first] += 1;
			green_pdf[(int)pix_gp[i*imageWidth + j].first] += 1;
			blue_pdf[(int)pix_bp[i*imageWidth + j ].first] += 1;
		}
	}
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
	/*for (int k = 0; k < 256; k++) {
		red_pdf[k] = red_pdf[k] / (imageHeight*imageWidth);
		green_pdf[k] = green_pdf[k] / (imageHeight*imageWidth);
		blue_pdf[k] = blue_pdf[k] / (imageHeight*imageWidth);
	}*/
	for (int i = 0; i < imageHeight*imageWidth; i++) {
		pix_r[pix_rp[i].second] = pix_rp[i].first;
		pix_g[pix_gp[i].second] = pix_gp[i].first;
		pix_b[pix_bp[i].second] = pix_bp[i].first;

	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++)
		{
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_r[r*imageWidth + c];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_g[r*imageWidth + c];
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_b[r*imageWidth + c];

		}
	}
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

	
	
