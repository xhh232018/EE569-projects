#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <math.h>
#include<algorithm>
#include <stdio.h>
using namespace std;
bool compare(unsigned char win[][3], unsigned char s[][3]) {
	int count = 0;
	if ((win[0][0] == s[0][0]) && (win[0][1] == s[0][1]) && (win[0][2] == s[0][2]))
	{
		count++;
	}
	if ((win[1][0] == s[1][0]) && (win[1][1] == s[1][1]) && (win[1][2] == s[1][2]))
	{
		count++;
	}
	if ((win[2][0] == s[2][0]) && (win[2][1] == s[2][1]) && (win[2][2] == s[2][2]))
	{
		count++;
	}
	if (count == 3) {
		return true;
	}
	else
	{
		return false;
	}
}
bool dcompare(unsigned char win[3][3], unsigned char d[3][3]) {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if ((win[i][j] == d[i][j]) || (d[i][j] == 2))
			{
				count++;
			}

		}
	}
	if (count == 9) {
		return true;
	}
	else
	{
		return false;
	}
}
unsigned char maskhit(unsigned char s4[8][3][3],
	unsigned char s5[][3][3], unsigned char s6[10][3][3], unsigned char s7[4][3][3], unsigned char s8[4][3][3],
	unsigned char s9[8][3][3], unsigned char s10[4][3][3], int b, unsigned char m_window[][3]) {
	unsigned char temp1 = 0;
	switch (b)
	{
	
	case 4:
		if (compare(m_window, s4[1]) || compare(m_window, s4[2]) || compare(m_window, s4[3]) || compare(m_window, s4[0]) ||
			compare(m_window, s4[4]) || compare(m_window, s4[5]) || compare(m_window, s4[6]) || compare(m_window, s4[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 5:
		if (compare(m_window, s5[0]) || compare(m_window, s5[1]) || compare(m_window, s5[2]) || compare(m_window, s5[3]) ||
			compare(m_window, s5[4]) || compare(m_window, s5[5]) || compare(m_window, s5[6]) || compare(m_window, s5[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 6:
		if (compare(m_window, s6[0]) || compare(m_window, s6[1]) || compare(m_window, s6[2]) || compare(m_window, s6[3]) ||
			compare(m_window, s6[4]) || compare(m_window, s6[5]) || compare(m_window, s6[6]) || compare(m_window, s6[7]) ||
			compare(m_window, s6[8]) || compare(m_window, s6[9])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 7:
		if (compare(m_window, s7[0]) || compare(m_window, s7[1]) || compare(m_window, s7[2]) || compare(m_window, s7[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 8:
		if (compare(m_window, s8[0]) || compare(m_window, s8[1]) || compare(m_window, s8[2]) || compare(m_window, s8[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 9:
		if (compare(m_window, s9[0]) || compare(m_window, s9[1]) || compare(m_window, s9[2]) || compare(m_window, s9[3]) ||
			compare(m_window, s9[4]) || compare(m_window, s9[5]) || compare(m_window, s9[6]) || compare(m_window, s9[7])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	case 10:
		if (compare(m_window, s10[0]) || compare(m_window, s10[1]) || compare(m_window, s10[2]) || compare(m_window, s10[3])) {
			temp1 = 1;
			break;
		}
		else
		{
			temp1 = 0;
			break;
		}
	}
	return temp1;
}

int main(int argc, char* argv[]) {
	int BytesPerPixel;
	int imageWidth;
	int imageHeight;
	int N = 3;
	int bond;
	int tempi;
	vector<unsigned char> pix;
	vector<vector<unsigned char> > pix_cp;
	vector<vector<unsigned char> > intermid;
	vector<vector<unsigned char> > extimg;
	vector<unsigned char> pix_out;
	vector<vector<unsigned char> > pix_iter;
	vector<string> u_ptn;
	unsigned char window[3][3];
	unsigned char temp;
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [imageWidth = 256] [imageHeight = 256]" << endl;
		return 0;
	}
	// input and output file names
	string inputFileName = argv[1];
	string outputFileName1 = argv[2];
	string match;
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
	unsigned char t_4[8][3][3]={{{0,0,1},{0,1,1},{0,0,1}},
	{{1,1,1},{0,1,0},{0,0,0}},
	{{1,0,0},{1,1,0},{1,0,0}},
	{{0,0,0},{0,1,0},{1,1,1}},{ { 0,1,0 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 0,1,0 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,1,0 } } };
	

	
	unsigned char t_5[8][3][3]={{{1,1,0},{0,1,1},{0,0,0}},
	{{0,1,0},{0,1,1},{0,0,1}},
	{{0,1,1},{1,1,0},{0,0,0}},
	{{0,0,1},{0,1,1},{0,1,0}},{ { 0,1,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,1,1 } } };
	
	
	unsigned char t_6[10][3][3]={{{1,1,1},{0,1,1},{0,0,0}},
	
	{{0,1,1},{0,1,1},{0,0,1}},
	
	{{1,1,1},{1,1,0},{0,0,0}},
	
	{{1,1,0},{1,1,0},{1,0,0}},
	
	{{1,0,0},{1,1,0},{1,1,0}},
	
	{{0,0,0},{1,1,0},{1,1,1}},
	
	{{0,0,0},{0,1,1},{1,1,1}},
	
	{{0,0,1},{0,1,1},{0,1,1}},{ { 1,1,0 },{ 0,1,1 },{ 0,0,1 } },{ { 0,1,1 },{ 1,1,0 },{ 1,0,0 } } };
	

	unsigned char t_7[4][3][3]={{{1,1,1},{0,1,1},{0,0,1}},
	{{1,1,1},{1,1,0},{1,0,0}},
	{{1,0,0},{1,1,0},{1,1,1}},
	{{0,0,1},{0,1,1},{1,1,1}}};
	
	unsigned char t_8[4][3][3]={{{0,1,1},{0,1,1},{0,1,1}},
	{{1,1,1},{1,1,1},{0,0,0}},
	{{1,1,0},{1,1,0},{1,1,0}},
	{{0,0,0},{1,1,1},{1,1,1}}};
	
	unsigned char t_9[8][3][3]=
	{{{1,1,1},{0,1,1},{0,1,1}},
	{{0,1,1},{0,1,1},{1,1,1}},
	{{1,1,1},{1,1,1},{1,0,0}},
	{{1,1,1},{1,1,1},{0,0,1}},
	{{1,1,1},{1,1,0},{1,1,0}},
	{{1,1,0},{1,1,0},{1,1,1}},
	{{1,0,0},{1,1,1},{1,1,1}},
	{{0,0,1},{1,1,1},{1,1,1}}};
	
	
	unsigned char t_10[4][3][3]=
	{{{1,1,1},{0,1,1},{1,1,1}},
	{{1,1,1},{1,1,1},{1,0,1}},
	{{1,1,1},{1,1,0},{1,1,1}},
	{{1,0,1},{1,1,1},{1,1,1}}};

	unsigned char unc_nd[28][3][3] = {
		//without D
		// Spur
		{ { 0,0,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,0 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,0,0 } },

	//L cluster
	{ { 0,0,1 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,1 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 0,1,0 },{ 0,0,0 } },
	{ { 1,0,0 },{ 1,1,0 },{ 0,0,0 } },
	{ { 0,0,0 },{ 1,1,0 },{ 1,0,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 1,1,0 } },
	{ { 0,0,0 },{ 0,1,0 },{ 0,1,1 } },
	{ { 0,0,0 },{ 0,1,1 },{ 0,0,1 } },

	//4-Connected offest
	{ { 0,1,1 },{ 1,1,0 },{ 0,0,0 } },
	{ { 1,1,0 },{ 0,1,1 },{ 0,0,0 } },
	{ { 0,1,0 },{ 0,1,1 },{ 0,0,1 } },
	{ { 0,0,1 },{ 0,1,1 },{ 0,1,0 } },


	//Spur corner cluster
	{ { 0,1,1 },{ 0,1,1 },{ 1,0,0 } },{ { 0,0,1 },{ 0,1,1 },{ 1,0,0 } },{ { 0,1,1 },{ 0,1,0 },{ 1,0,0 } },
	{ { 1,1,0 },{ 1,1,0 },{ 0,0,1 } },{ { 1,0,0 },{ 1,1,0 },{ 0,0,1 } },{ { 1,1,0 },{ 0,1,0 },{ 0,0,1 } },
	{ { 0,0,1 },{ 1,1,0 },{ 1,1,0 } },{ { 0,0,1 },{ 1,1,0 },{ 1,0,0 } },{ { 0,0,1 },{ 0,1,0 },{ 1,1,0 } },
	{ { 1,0,0 },{ 0,1,1 },{ 0,1,1 } },{ { 1,0,0 },{ 0,1,0 },{ 0,1,1 } },{ { 1,0,0 },{ 0,1,1 },{ 0,0,1 } },


	};

	unsigned char unc_d[41][3][3] = {
		//With D
		//corner cluster
		{ { 1,1,2 },{ 1,1,2 },{ 2,2,2 } },

		//Tree branch
	{ { 2,1,0 },{ 1,1,1 },{ 2,0,0 } },
	{ { 0,1,2 },{ 1,1,1 },{ 0,0,2 } },
	{ { 0,0,2 },{ 1,1,1 },{ 0,1,2 } },
	{ { 2,0,0 },{ 1,1,1 },{ 2,1,0 } },
	{ { 2,1,2 },{ 1,1,0 },{ 0,1,0 } },
	{ { 0,1,0 },{ 1,1,0 },{ 2,1,2 } },
	{ { 0,1,0 },{ 0,1,1 },{ 2,1,2 } },
	{ { 2,1,2 },{ 0,1,1 },{ 0,1,0 } },

	//Vee branch
	{ { 1,2,1 },{ 2,1,2 },{ 0,0,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 0,1,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 0,1,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,0,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,0,1 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,1,0 } },
	{ { 1,2,1 },{ 2,1,2 },{ 1,1,1 } },

	{ { 1,2,1 },{ 2,1,0 },{ 1,2,0 } },
	{ { 1,2,0 },{ 2,1,1 },{ 1,2,0 } },
	{ { 1,2,0 },{ 2,1,0 },{ 1,2,1 } },
	{ { 1,2,1 },{ 2,1,1 },{ 1,2,0 } },
	{ { 1,2,1 },{ 2,1,0 },{ 1,2,1 } },
	{ { 1,2,0 },{ 2,1,1 },{ 1,2,1 } },
	{ { 1,2,1 },{ 2,1,1 },{ 1,2,1 } },

	{ { 0,0,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 0,1,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,0,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 0,1,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,1,0 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,0,1 },{ 2,1,2 },{ 1,2,1 } },
	{ { 1,1,1 },{ 2,1,2 },{ 1,2,1 } },

	{ { 1,2,1 },{ 0,1,2 },{ 0,2,1 } },
	{ { 0,2,1 },{ 1,1,2 },{ 0,2,1 } },
	{ { 0,2,1 },{ 0,1,2 },{ 1,2,1 } },
	{ { 1,2,1 },{ 1,1,2 },{ 0,2,1 } },
	{ { 1,2,1 },{ 0,1,2 },{ 1,2,1 } },
	{ { 0,2,1 },{ 1,1,2 },{ 1,2,1 } },
	{ { 1,2,1 },{ 1,1,2 },{ 1,2,1 } },

	//Diagonal branch
	{ { 2,1,0 },{ 0,1,1 },{ 1,0,2 } },
	{ { 0,1,2 },{ 1,1,0 },{ 2,0,1 } },
	{ { 2,0,1 },{ 1,1,0 },{ 0,1,2 } },
	{ { 1,0,2 },{ 0,1,1 },{ 2,1,0 } }

	};


	pix.resize(imageWidth*imageHeight*BytesPerPixel, 0);
	intermid.resize(imageHeight);
	pix_cp.resize(imageHeight);
	pix_iter.resize(imageHeight);
	pix_out.resize(imageHeight*imageWidth*BytesPerPixel, 0);
	extimg.resize(imageHeight + 2 * floor(N / 2));
	FILE* inputFile;
	if (!(inputFile = fopen(inputFileName.c_str(), "rb"))) {
		cout << "Cannot open inputfile: " << inputFile << endl;
		exit(1);
	}
	fread(&pix[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, inputFile);
	fclose(inputFile);

	cout << "loading data success!" << endl;
	for (int i = 0; i < imageHeight; i++) {
		intermid[i].resize(imageWidth);
		pix_cp[i].resize(imageWidth);
		pix_iter[i].resize(imageWidth);
	}
	for (int i = 0; i < imageHeight + 2 * floor(N / 2); i++) {
		extimg[i].resize(imageWidth + 2 * floor(N / 2));
	}
	//二值化
	for (int r = 0; r < imageHeight; r++) {
		for (int c = 0; c < imageWidth; c++)
		{
			if (pix[r*imageWidth + c] <= 127) {
				temp = 255;
			}
			else
			{
				temp = 0;
			}
			pix_cp[r][c] = temp;
		}
	}

	//image extension
	for (int iter_num = 0; iter_num < 30; iter_num++) {
		
		//判断 Stage I
		unsigned char pixel;
		for (int r = 1; r < imageHeight - 1; r++) {
			for (int c = 1; c < imageWidth - 1; c++) {
				for (int r1 = 0; r1 <= 2; r1++) {
					for (int c1 = 0; c1 <= 2; c1++) {
						window[r1][c1] = pix_cp[r + r1 - 1][c + c1 - 1] / 255;
					}
				}

				if (window[1][1] == 0) {
					intermid[r][c] = 0;
					continue;
				}
				else
				{
					bond = 2 * (window[0][1] + window[1][2] + window[1][0] + window[2][1]) + 1 * (window[0][0] + window[0][2] + window[2][0] + window[2][2]);
				}
				if (bond == 0 || bond ==1 || bond ==2|| bond==3|| bond == 11) {
					intermid[r][c] = 0;
					continue;
				}

				intermid[r][c] = maskhit(t_4, t_5, t_6, t_7, t_8, t_9, t_10, bond, window);

			}
		}
		//STAGE II 
		int flag1 = 0;
		int flag2 = 0;
		for (int i = 1; i < imageHeight - 1; i++) {
			for (int j = 1; j < imageWidth - 1; j++) {
				if (intermid[i][j] == 1) {
					for (int r = 0; r < N; r++) {
						for (int c = 0; c < N; c++) {
							window[r][c] = intermid[i + r - 1][j + c - 1];
						}
					}
				}
				else
				{
					pix_iter[i][j] = pix_cp[i][j];
					continue;
				}
				for (int i = 0; i < 27; i++) {
					if (compare(window, unc_nd[i])) {
						flag1 = 1;
						break;
					}
				}
				for (int i = 0; i < 41; i++) {
					if (dcompare(window, unc_d[i])) {
						flag2 = 1;
						break;
					}
				}
				if ((flag1 || flag2)) {
					pix_iter[i][j] = pix_cp[i][j];
					flag1 = 0;
					flag2 = 0;
					continue;
				}
				else
				{
					pix_iter[i][j] = 0;

				}
			}
		}
		pix_cp = pix_iter;
		cout << iter_num << "iteration successful!" << endl;
	}
	
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			pix_out[i*imageWidth + j] = 255 - pix_iter[i][j];
		}
	}
	
	FILE* outputFile;
	if (!(outputFile = fopen(outputFileName1.c_str(), "wb"))) {
		cout << "Cannot open file: ";
		exit(1);
	}
	//    cout<< (int)pixelData[0] << endl;
	fwrite(&pix_out[0], sizeof(unsigned char), imageWidth*imageHeight*BytesPerPixel, outputFile);
	fclose(outputFile);

	return 0;
}
