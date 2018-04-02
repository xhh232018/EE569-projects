#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
using namespace std;
struct chosen
{
	unsigned char value;
	int count;
};

unsigned char FindMostFreq(vector<unsigned char>& neigh) {
	sort(neigh.begin(), neigh.end());
	chosen c1, c2;
	c1.count = 0;
	c1.value = 0;
	c2.count = 1;
	c2.value = neigh.front();
	for (int i = 0; i<neigh.size() - 1; i++) {
		if (neigh.at(i) == neigh.at(i + 1))//若相等则次数加一
			c2.count++;
		else {//如不等则更新c2，而且当前次数大于保存的次数时更新c1
			if (c2.count>c1.count)
				c1 = c2;
			c2.count = 1;
			c2.value = neigh.at(i + 1);
		}
	}
	if (c2.count>c1.count)//最后还需更新一次
		c1 = c2;
	return c1.value;
}
int main(int argc, char* argv[])
{
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int parts;
	//int N=5;
	int N;
	vector<unsigned char> pix;
	vector<unsigned char> pix_r;
	vector<unsigned char> pix_g;
	vector<unsigned char> pix_b;
	vector<pair<unsigned char, int> > pix_rp;
	vector<pair<unsigned char, int> > pix_gp;
	vector<pair<unsigned char, int> > pix_bp;
	vector<unsigned char> r_box;
	vector<unsigned char> g_box;
	vector<unsigned char> b_box;
	vector<vector<vector<unsigned char> > > extimg;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName = argv[2];
	string outputFileName1 = argv[8];
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
			parts = atoi(argv[7]);
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
	extimg.resize(imageHeight + 2 * floor(N / 2));
	r_box.resize(N*N);
	g_box.resize(N*N);
	b_box.resize(N*N);

	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);
	for (int r = 0; r <imageHeight; r++)
	{
		for (int c = 0; c < imageWidth; c++) {
			pix_rp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel], r*imageWidth + c));
			pix_gp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1], r*imageWidth + c));
			pix_bp.push_back(make_pair(pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2], r*imageWidth + c));


		}
	}
	cout << "loading data success!";
	sort(pix_rp.begin(), pix_rp.end());
	sort(pix_gp.begin(), pix_gp.end());
	sort(pix_bp.begin(), pix_bp.end());
	if (parts == 4) {
		double r_sum1 = 0, g_sum1 = 0, b_sum1 = 0;
		double r_sum2 = 0, g_sum2 = 0, b_sum2 = 0;
		double r_sum3 = 0, g_sum3 = 0, b_sum3 = 0;
		double r_sum4 = 0, g_sum4 = 0, b_sum4 = 0;
		int r1, r2, r3, r4;
		int g1, g2, g3, g4;
		int b1, b2, b3, b4;
		for (int index = 0; index < imageHeight*imageWidth; index++) {
			if (index < imageHeight*imageWidth / parts) {
				r_sum1 += (int)pix_rp[index].first;
				g_sum1 += (int)pix_gp[index].first;
				b_sum1 += (int)pix_bp[index].first;

			}
			if ((index >= imageHeight * imageWidth / parts) && (index<2 * imageWidth*imageHeight / parts)) {
				r_sum2 += (int)pix_rp[index].first;
				g_sum2 += (int)pix_gp[index].first;
				b_sum2 += (int)pix_bp[index].first;

			}
			if ((index >= 2 * imageHeight * imageWidth / parts) && (index< 3 * imageWidth*imageHeight / parts)) {
				r_sum3 += (int)pix_rp[index].first;
				g_sum3 += (int)pix_gp[index].first;
				b_sum3 += (int)pix_bp[index].first;
			}
			if ((index >= 3 * imageHeight * imageWidth / parts) && (index<imageWidth*imageHeight)) {
				r_sum4 += (int)pix_rp[index].first;
				g_sum4 += (int)pix_gp[index].first;
				b_sum4 += (int)pix_bp[index].first;

			}
		}
		r1 = round(r_sum1 / (imageHeight*imageWidth / parts));
		r2 = round(r_sum2 / (imageHeight*imageWidth / parts));
		r3 = round(r_sum3 / (imageHeight*imageWidth / parts));
		r4 = round(r_sum4 / (imageHeight*imageWidth / parts));
		g1 = round(g_sum1 / (imageHeight*imageWidth / parts));
		g2 = round(g_sum2 / (imageHeight*imageWidth / parts));
		g3 = round(g_sum3 / (imageHeight*imageWidth / parts));
		g4 = round(g_sum4 / (imageHeight*imageWidth / parts));
		b1 = round(b_sum1 / (imageHeight*imageWidth / parts));
		b2 = round(b_sum2 / (imageHeight*imageWidth / parts));
		b3 = round(b_sum3 / (imageHeight*imageWidth / parts));
		b4 = round(b_sum4 / (imageHeight*imageWidth / parts));

		for (int index = 0; index < imageHeight*imageWidth; index = index + 1) {
			if (index < imageHeight*imageWidth / parts) {
				pix_rp[index].first = (unsigned char)r1;
				pix_gp[index].first = (unsigned char)g1;
				pix_bp[index].first = (unsigned char)b1;

			}
			if ((index >= imageHeight * imageWidth / parts) && (index<2 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r2;
				pix_gp[index].first = (unsigned char)g2;
				pix_bp[index].first = (unsigned char)b2;

			}
			if ((index >= 2 * imageHeight * imageWidth / parts) && (index< 3 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r3;
				pix_gp[index].first = (unsigned char)g3;
				pix_bp[index].first = (unsigned char)b3;
			}
			if ((index >= 3 * imageHeight * imageWidth / parts) && (index<imageWidth*imageHeight)) {
				pix_rp[index].first = (unsigned char)r4;
				pix_gp[index].first = (unsigned char)g4;
				pix_bp[index].first = (unsigned char)b4;

			}
		}
		for (int i = 0; i < imageHeight*imageWidth; i++) {
			pix_r[pix_rp[i].second] = pix_rp[i].first;
			pix_g[pix_gp[i].second] = pix_gp[i].first;
			pix_b[pix_bp[i].second] = pix_bp[i].first;

		}
		for (int r = 0; r < imageHeight; r++) {
			for (int c = 0; c < imageWidth; c++) {
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_r[r*imageWidth + c];
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_g[r*imageWidth + c];
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_b[r*imageWidth + c];
			}
		}
	}
	if (parts == 8) {
		double r_sum1 = 0, g_sum1 = 0, b_sum1 = 0;
		double r_sum2 = 0, g_sum2 = 0, b_sum2 = 0;
		double r_sum3 = 0, g_sum3 = 0, b_sum3 = 0;
		double r_sum4 = 0, g_sum4 = 0, b_sum4 = 0;
		double r_sum5 = 0, g_sum5 = 0, b_sum5 = 0;
		double r_sum6 = 0, g_sum6 = 0, b_sum6 = 0;
		double r_sum7 = 0, g_sum7 = 0, b_sum7 = 0;
		double r_sum8 = 0, g_sum8 = 0, b_sum8 = 0;
		int r1, r2, r3, r4, r5, r6, r7, r8;
		int g1, g2, g3, g4, g5, g6, g7, g8;
		int b1, b2, b3, b4, b5, b6, b7, b8;
		for (int index = 0; index < imageHeight*imageWidth; index++) {
			if (index < imageHeight*imageWidth / parts) {
				r_sum1 += (int)pix_rp[index].first;
				g_sum1 += (int)pix_gp[index].first;
				b_sum1 += (int)pix_bp[index].first;

			}
			if ((index >= imageHeight * imageWidth / parts) && (index < 2 * imageWidth*imageHeight / parts)) {
				r_sum2 += (int)pix_rp[index].first;
				g_sum2 += (int)pix_gp[index].first;
				b_sum2 += (int)pix_bp[index].first;

			}
			if ((index >= 2 * imageHeight * imageWidth / parts) && (index < 3 * imageWidth*imageHeight / parts)) {
				r_sum3 += (int)pix_rp[index].first;
				g_sum3 += (int)pix_gp[index].first;
				b_sum3 += (int)pix_bp[index].first;
			}
			if ((index >= 3 * imageHeight * imageWidth / parts) && (index < 4 * imageWidth*imageHeight/parts)) {
				r_sum4 += (int)pix_rp[index].first;
				g_sum4 += (int)pix_gp[index].first;
				b_sum4 += (int)pix_bp[index].first;

			}
			if ((index >=4* imageHeight * imageWidth / parts) && (index < 5 * imageWidth*imageHeight / parts)) {
				r_sum5 += (int)pix_rp[index].first;
				g_sum5 += (int)pix_gp[index].first;
				b_sum5 += (int)pix_bp[index].first;

			}
			if ((index >= 5 * imageHeight * imageWidth / parts) && (index < 6 * imageWidth*imageHeight / parts)) {
				r_sum6 += (int)pix_rp[index].first;
				g_sum6 += (int)pix_gp[index].first;
				b_sum6 += (int)pix_bp[index].first;
			}
			if ((index >= 6 * imageHeight * imageWidth / parts) && (index <7* imageWidth*imageHeight/parts)) {
				r_sum7 += (int)pix_rp[index].first;
				g_sum7 += (int)pix_gp[index].first;
				b_sum7 += (int)pix_bp[index].first;

			}
			if (index >=7 * imageWidth*imageHeight/parts){
				r_sum8 += (int)pix_rp[index].first;
				g_sum8 += (int)pix_gp[index].first;
				b_sum8 += (int)pix_bp[index].first;

			}
		}
		r1 = round(r_sum1 / (imageHeight*imageWidth / parts));
		r2 = round(r_sum2 / (imageHeight*imageWidth / parts));
		r3 = round(r_sum3 / (imageHeight*imageWidth / parts));
		r4 = round(r_sum4 / (imageHeight*imageWidth / parts));
		r5 = round(r_sum5 / (imageHeight*imageWidth / parts));
		r6 = round(r_sum6 / (imageHeight*imageWidth / parts));
		r7 = round(r_sum7 / (imageHeight*imageWidth / parts));
		r8 = round(r_sum8 / (imageHeight*imageWidth / parts));

		g1 = round(g_sum1 / (imageHeight*imageWidth / parts));
		g2 = round(g_sum2 / (imageHeight*imageWidth / parts));
		g3 = round(g_sum3 / (imageHeight*imageWidth / parts));
		g4 = round(g_sum4 / (imageHeight*imageWidth / parts));
		g5 = round(g_sum5 / (imageHeight*imageWidth / parts));
		g6 = round(g_sum6 / (imageHeight*imageWidth / parts));
		g7 = round(g_sum7 / (imageHeight*imageWidth / parts));
		g8 = round(g_sum8 / (imageHeight*imageWidth / parts));

		b1 = round(b_sum1 / (imageHeight*imageWidth / parts));
		b2 = round(b_sum2 / (imageHeight*imageWidth / parts));
		b3 = round(b_sum3 / (imageHeight*imageWidth / parts));
		b4 = round(b_sum4 / (imageHeight*imageWidth / parts));
		b5 = round(b_sum5 / (imageHeight*imageWidth / parts));
		b6 = round(b_sum6 / (imageHeight*imageWidth / parts));
		b7 = round(b_sum7 / (imageHeight*imageWidth / parts));
		b8 = round(b_sum8 /  (imageHeight*imageWidth / parts));

		for (int index = 0; index < imageHeight*imageWidth; index = index + 1) {
			if (index < imageHeight*imageWidth / parts) {
				pix_rp[index].first = (unsigned char)r1;
				pix_gp[index].first = (unsigned char)g1;
				pix_bp[index].first = (unsigned char)b1;

			}
			if ((index >= imageHeight * imageWidth / parts) && (index < 2 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r2;
				pix_gp[index].first = (unsigned char)g2;
				pix_bp[index].first = (unsigned char)b2;

			}
			if ((index >= 2 * imageHeight * imageWidth / parts) && (index < 3 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r3;
				pix_gp[index].first = (unsigned char)g3;
				pix_bp[index].first = (unsigned char)b3;
			}
			if ((index >= 3 * imageHeight * imageWidth / parts) && (index < 4* imageWidth*imageHeight/parts)) {
				pix_rp[index].first = (unsigned char)r4;
				pix_gp[index].first = (unsigned char)g4;
				pix_bp[index].first = (unsigned char)b4;

			}
			if ((index >= 4 * imageHeight * imageWidth / parts) && (index < 5 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r5;
				pix_gp[index].first = (unsigned char)g5;
				pix_bp[index].first = (unsigned char)b5;

			}
			if ((index >= 5 * imageHeight * imageWidth / parts) && (index < 6 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r6;
				pix_gp[index].first = (unsigned char)g6;
				pix_bp[index].first = (unsigned char)b6;
			}
			if ((index >= 6 * imageHeight * imageWidth / parts) && (index <7 * imageWidth*imageHeight / parts)) {
				pix_rp[index].first = (unsigned char)r7;
				pix_gp[index].first = (unsigned char)g7;
				pix_bp[index].first = (unsigned char)b7;

			}
			if (index >= 7 * imageWidth*imageHeight/parts) {
				pix_rp[index].first = (unsigned char)r8;
				pix_gp[index].first = (unsigned char)g8;
				pix_bp[index].first = (unsigned char)b8;

			}
		}
		for (int i = 0; i < imageHeight*imageWidth; i++) {
			pix_r[pix_rp[i].second] = pix_rp[i].first;
			pix_g[pix_gp[i].second] = pix_gp[i].first;
			pix_b[pix_bp[i].second] = pix_bp[i].first;

		}
		for (int r = 0; r < imageHeight; r++) {
			for (int c = 0; c < imageWidth; c++) {
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = pix_r[r*imageWidth + c];
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1] = pix_g[r*imageWidth + c];
				pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2] = pix_b[r*imageWidth + c];
			}
		}
	}	//step 2 
	FILE* outputFile2;
	if (!(outputFile2 = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile2);
	fclose(outputFile2);

	for (int i = 0; i < imageHeight+2*floor(N/2); i++) {
		extimg[i].resize(imageWidth+2*floor(N / 2));
		for (int j = 0; j < imageWidth + 2 * floor(N / 2); j++) {
			extimg[i][j].resize(BytesPerPixel);
		}
	}
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r + N / 2][c + N / 2][0] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg[r + N / 2][c + N / 2][1] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 1];
			extimg[r + N / 2][c + N / 2][2] = pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel + 2];
		}
	}
	for (int r = 0; r < N / 2; r++) {
		for (int c = 0; c < imageWidth; c++) {
			extimg[r][c + N / 2][0] = pix[(N / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg[r][c + N / 2][1] = pix[(N / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel + 1];
			extimg[r][c + N / 2][2] = pix[(N / 2 - r)*imageWidth*BytesPerPixel + c * BytesPerPixel + 2];
			extimg[imageHeight + 2 * floor(N / 2) - 1 - r][c + N / 2][0] = pix[(imageHeight-1-N/2+r)*imageWidth*BytesPerPixel + c * BytesPerPixel];
			extimg[imageHeight + 2 * floor(N / 2) - 1 - r][c + N / 2][1] = pix[(imageHeight - 1 - N / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel+1];
			extimg[imageHeight + 2 * floor(N / 2) - 1 - r][c + N / 2][2] = pix[(imageHeight - 1 - N / 2 + r)*imageWidth*BytesPerPixel + c * BytesPerPixel+2];

		}
	}
	for (int r = 0; r < imageHeight + 2 * floor(N / 2); r++) {
		for (int c = 0; c < N / 2; c++) {
			extimg[r][c][0] = extimg[r][N - 1 - c][0];
			extimg[r][c][1] = extimg[r][N - 1 - c][1];
			extimg[r][c][2] = extimg[r][N - 1 - c][2];
			extimg[r][imageWidth + 2 * floor(N / 2) - 1 - c][0] = extimg[r][imageWidth + 2 * floor(N / 2) - 1 - 2 * floor(N / 2) + c][0];
			extimg[r][imageWidth + 2 * floor(N / 2) - 1 - c][1] = extimg[r][imageWidth + 2 * floor(N / 2) - 1 - 2 * floor(N / 2) + c][1];
			extimg[r][imageWidth + 2 * floor(N / 2) - 1 - c][2] = extimg[r][imageWidth + 2 * floor(N / 2) - 1 - 2 * floor(N / 2) + c][2];

		}
	}//图像拓展完毕
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++) {
			for (int r1 = 0; r1 < N; r1++) {
				for (int c1 = 0; c1 < N; c1++) {
					r_box[r1*N + c1] = extimg[r + r1][c + c1][0];
					g_box[r1*N + c1] = extimg[r + r1][c + c1][1];
					b_box[r1*N + c1] = extimg[r + r1][c + c1][2];
				}
			}
			unsigned char temp_r = FindMostFreq(r_box);
			unsigned char temp_g = FindMostFreq(g_box);
			unsigned char temp_b = FindMostFreq(b_box);
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel] = temp_r;
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+1] = temp_g;
			pix[r*imageWidth*BytesPerPixel + c * BytesPerPixel+2] = temp_b;
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
