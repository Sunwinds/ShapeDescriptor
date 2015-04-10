#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <float.h>
#include <string.h>
#include <limits.h>

#include "ds.h"
#include "RWObj.h"
#include "Bitmap.h"
#include "TranslateScale.h"
#include "Rotate.h"
#include "RegionShape.h"
#include "RecovAffine.h"
#include "Refine.h"
#include "edge.h"
#include "convert.h"
#include "ColorDescriptor.h"
#include "Circularity.h"
#include "FourierDescriptor.h"
#include "Eccentricity.h"

#define abs(a) (a>0)?(a):-(a)

#define	QUANT8				256		// 2^8
#define FD_SCALE			2		// *2 first, and then quantization
#define CIR_SCALE			2.318181818		// the range of circularity is [0~110], so *2.318 to be [0~255]
#define ECC_SCALE			25.5			// the range of circularity is [0~10], so *25.5 to be [0~255]

unsigned char	CamMap[CAMNUM_2]={0,1,2,3,4,5,6,7,8,9,5,6,7,8,9,2,3,4,0,1};

char srcfn[100];
char destfn[100];

int			winw = WIDTH, winh = HEIGHT;

pVer		vertex=NULL;
pTri		triangle=NULL;
int			NumVer=0, NumTri=0;		// total number of vertex and triangle.

pVer		vertex1, vertex2;
pTri		triangle1, triangle2;
int			NumVer1, NumTri1, NumVer2, NumTri2;		// total number of vertex and triangle.

// translate and scale of model 1 and 2
Ver				Translate1, Translate2;
double			Scale1, Scale2;

void FindCenter(unsigned char *srcBuff, int width, int height, double *CenX, double *CenY)
{
	int					x, y, count;
	unsigned char		*pImage;
	int					maxX, minX, maxY, minY;
	int					MeanX, MeanY; 

	count = 0;
	pImage = srcBuff;

	// ***********************************************************************************************
	// if use "mean" to each 2D shape independnetly, the origin will be moved a lot in 3D
	// if ues "center" to each 2D shape independnetly, the origin will be moved only a little in 3D
	// if center can be defined in 3D, the origin will not be moved any more.
	// But this will not very robust in 3D similarity transformation
	// In addition, to make center of each 2D shape more close to user drawn 2D shapes,
	// it's better to define center for each 2D shape independently

	// uee center of max and min to be center
	maxX = maxY = -1;
	minX = minY = INT_MAX;
	for (y=0 ; y<height ; y++)
	for (x=0 ; x<width; x++)
	{
		if( *pImage < 255 )
		{
			if( x > maxX ) maxX = x;
			if( x < minX ) minX = x;
			if( y > maxY ) maxY = y;
			if( y < minY ) minY = y;
		}
		pImage++;
	}

	if( maxX > 0 )
	{
		*CenX = (maxX+minX) / 2.0;
		*CenY = (maxY+minY) / 2.0;
	}
	else
		*CenX = *CenY = -1;		// nothing to be rendered

	// use mean to be center
/*	count = 0;
	MeanX = MeanY = 0;
	for (y=0 ; y<height ; y++)
	for (x=0 ; x<width; x++)
	{
		if( *pImage < 255 )
		{
			MeanX += x;
			MeanY += y;
			count ++;
		}
		pImage++;
	}

	if( count > 0 )
	{
		*CenX = (double)MeanX / (double)count;
		*CenY = (double)MeanY / (double)count;
	}
	else
		*CenX = *CenY = -1;		// nothing to be rendered
*/
}

void display(void)
{
	int				i, j;
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();				
//		glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0);
		for(i = 0; i<NumTri; i++)
		{
			glColor3f((GLfloat)triangle[i].r, (GLfloat)triangle[i].g, (GLfloat)triangle[i].b);
			glBegin(GL_POLYGON);
				for(j=0; j<triangle[i].NodeName; j++)
					glVertex3d(vertex[triangle[i].v[j]].coor[0], vertex[triangle[i].v[j]].coor[1], vertex[triangle[i].v[j]].coor[2]);
			glEnd();
		}
	glPopMatrix();				

	glutSwapBuffers();
}

void RenderToMem(unsigned char *bmBits, unsigned char *bmColor, pVer CamVertex, pVer v, pTri t, int nv, int nt)
{
/*	
	// this is the same with reshape(), so this didn't do again
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(100.0, (GLfloat) winw/(GLfloat) winh, 0.1, 10.0);
//	glOrtho(-1, 1, -1, 1, 0.1, 100.0);
	glViewport (0, 0, (GLsizei) winw, (GLsizei) winh);
*/
	// test LookAt Up
/*	vector	v1, v2, v3;
	v1.x = CamVertex->coor[0];
	v1.y = CamVertex->coor[1];
	v1.z = CamVertex->coor[2];
	v2.x = 0;
	v2.y = 1;
	v2.z = 0;
	v3 = cross(v1, v2);*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	gluLookAt(CAMSCALE*CamVertex->coor[0], CAMSCALE*CamVertex->coor[1], CAMSCALE*CamVertex->coor[2],
	gluLookAt(CamVertex->coor[0], CamVertex->coor[1], CamVertex->coor[2],
				0, 0, 0,
				0, 1, 0);
//				v3.x, v3.y, v3.z);

	vertex = v;
	triangle = t;
	NumVer = nv;
	NumTri = nt;
	display();

	glReadBuffer(GL_BACK);
//	glReadPixels(0, 0, winw, winh, GL_RED, GL_UNSIGNED_BYTE, bmBits);
	glReadPixels(0, 0, winw, winh, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, bmBits);
	if( bmColor )
		glReadPixels(0, 0, winw, winh, GL_RGB, GL_UNSIGNED_BYTE, bmColor);
}

void keyboard (unsigned char key, int x, int y)
{
	unsigned char	*srcBuff[CAMNUM], *destBuff[CAMNUM], *EdgeBuff, *ColorBuff[CAMNUM], *YuvBuff;
	char			filename[400];
	pVer			CamVertex[ANGLE];
	pTri			CamTriangle[ANGLE];
	int				CamNumVer[ANGLE], CamNumTri[ANGLE];		// total number of vertex and triangle.
	FILE			*fpt, *fpt1, *fpt2, *fpt3, *fpt4, *fpt_art_q8, *fpt_art_q4, *fpt_fd_q8, *fpt_fd, *fpt_cir_q8, *fpt_ecc_q8;//, *fpt_ccd;
	int				i, j, k, srcCam, destCam, p, r, a, itmp;
	double			cost[ANGLE][ANGLE][CAMNUM_2][CAMNUM_2];
	double			**matrix;
	static int		UseCam = 2;
	clock_t			start, finish;
	double			Err;
	// for region shape descriptor
	double			src_ArtCoeff[ANGLE][CAMNUM][ART_ANGULAR][ART_RADIAL];
	double			dest_ArtCoeff[ANGLE][CAMNUM][ART_ANGULAR][ART_RADIAL];
	double			MinErr, err;
	int				align[60][CAMNUM_2];
	unsigned char	q8_ArtCoeff[ANGLE][CAMNUM][ART_COEF];
	unsigned char	q4_ArtCoeff[ANGLE][CAMNUM][ART_COEF_2];
	// for color decsriptor
	unsigned __int64 CompactColor[ANGLE][CAMNUM];	// 63 bits for each image
	unsigned __int64 dest_CompactColor[ANGLE][CAMNUM];	// 63 bits for each image
	// for circularity
	double			cir_Coeff[ANGLE][CAMNUM];
	unsigned char	q8_cirCoeff[ANGLE][CAMNUM], dest_cirCoeff[ANGLE][CAMNUM];
	// for fourier descriptor
	double			src_FdCoeff[ANGLE][CAMNUM][FD_COEFF_NO], dest_FdCoeff[ANGLE][CAMNUM][FD_COEFF_NO];
	unsigned char	q8_FdCoeff[ANGLE][CAMNUM][FD_COEFF_NO];
	sPOINT			*Contour;
	unsigned char	*ContourMask;
	// for eccentricity
	double			ecc_Coeff[ANGLE][CAMNUM];
	unsigned char	q8_eccCoeff[ANGLE][CAMNUM], dest_eccCoeff[ANGLE][CAMNUM];
	// for compare
	int				Count, TopNum;
	pMatRes			pSearch, pmr, pmrr, pTop;
	// quantization version
	char			fname[400];
//	char			fn[200];
 	int				high, low, middle;
	double			QuantTable[17] = {	0.000000000, 0.003585473, 0.007418411, 0.011535520, 
										0.015982337, 0.020816302, 0.026111312, 0.031964674, 
										0.038508176, 0.045926586, 0.054490513, 0.064619488, 
										0.077016351, 0.092998687, 0.115524524, 0.154032694, 1.000000000};
	double			CenX[CAMNUM], CenY[CAMNUM];
	int				total;

	switch (key) 
	{
	case 27:
		exit(0);
		break;

// *************************************************************************************************
/*	case 'e':
		// initialize ART
		GenerateBasisLUT();
		// initialize: read camera set
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			sprintf(filename, "12_%d", destCam);
			ReadObj(filename, CamVertex+destCam, CamTriangle+destCam, CamNumVer+destCam, CamNumTri+destCam);
		}

		if( (fpt1 = fopen("pair.txt", "r")) == NULL )
		{
			printf("pair.txt does not exist.\n");
			break;
		}

		while( fscanf(fpt1, "%s", srcfn) != EOF )
		{
			if( fscanf(fpt1, "%s", destfn) == EOF )
				break;

			// get the translatation and scale of the two model
			if( ReadObj(srcfn, &vertex1, &triangle1, &NumVer1, &NumTri1) == 0 )
				continue;

			if( ReadObj(destfn, &vertex2, &triangle2, &NumVer2, &NumTri2) == 0 )
				continue;

			// save original two models
			sprintf(filename, "%s_to_%s_org.obj", destfn, srcfn);
			SaveMergeObj(filename, vertex1, triangle1, NumVer1, NumTri1, vertex2, triangle2, NumVer2, NumTri2);

			// ****************************************************************
			// Corase alignment
			// ****************************************************************

			// record execute time --- start
			start = clock();

			// Translate and scale model 1 and 2
			TranslateScale(vertex2, NumVer2, triangle2, NumTri2, destfn, &Translate2, &Scale2);
			TranslateScale(vertex1, NumVer1, triangle1, NumTri1, srcfn, &Translate1, &Scale1);

			// read RED only, so size is winw*winh
			for(i=0; i<CAMNUM; i++)
				srcBuff[i] = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
			for(srcCam=0; srcCam<ANGLE; srcCam++)
			{
				// capture CAMNUM silhouette of srcfn to memory
				// 0.24 sec for an example (3012 triangles)
				for(i=0; i<CAMNUM; i++)
					RenderToMem(srcBuff[i], NULL, CamVertex[srcCam]+i, vertex1, triangle1, NumVer1, NumTri1);

				// 0.02 sec for an example
				FindRadius(srcBuff);

				// 0.16 sec for an example
				for(i=0; i<CAMNUM; i++)
					ExtractCoefficients(srcBuff[i], src_ArtCoeff[srcCam][i], NULL);
			}
			for(i=0; i<CAMNUM; i++)
				free(srcBuff[i]);

			// ********************************************************************************
			// capture CAMNUM silhouette of destfn to memory,
			// and get the cost between srcfn silhouette and destfn silhouette

			// read REB only, so size is winw*winh
			for(i=0; i<CAMNUM; i++)
				destBuff[i] = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
			for(destCam=0; destCam<ANGLE; destCam++)
			{
				// capture CAMNUM silhouette of dest to memory
				// 0.47 sec for an example (18110 triangles)
				for(i=0; i<CAMNUM; i++)
					RenderToMem(destBuff[i], NULL, CamVertex[destCam]+i, vertex2, triangle2, NumVer2, NumTri2);

				// 0.02 sec for an example
				FindRadius(destBuff);

				// 0.16 sec for an example
				for(i=0; i<CAMNUM; i++)
				{
					ExtractCoefficients(destBuff[i], dest_ArtCoeff[destCam][i], NULL);

					for(srcCam=0; srcCam<ANGLE; srcCam++)
						for(j=0; j<CAMNUM; j++)
							cost[srcCam][destCam][j][i] = GetDistance(dest_ArtCoeff[destCam][i], src_ArtCoeff[srcCam][j]);
				}
			}
			for(i=0; i<CAMNUM; i++)
				free(destBuff[i]);

			// ********************************************************************************
			// get the affine transformation from model 2 to model 1
			// initialize
			matrix = (double **) malloc (4 * sizeof(double *));
			for(i=0; i<4; i++)
			{
				matrix[i] = (double *) malloc(4 * sizeof(double));
				memset(matrix[i], 0, 4 * sizeof(double));
			}
			matrix[0][0] = matrix[1][1] = matrix[2][2] = matrix[3][3] = 1;

			// get the transformation matrix of model 2 based on model 1
			Err = RecoverAffine(matrix, cost, &UseCam);

			Rotate(vertex2, NumVer2, matrix);

			Scale1 = 1.0 / Scale1;
			Scale(vertex2, NumVer2, Scale1);

			Translate1.coor[0] = -Translate1.coor[0];
			Translate1.coor[1] = -Translate1.coor[1];
			Translate1.coor[2] = -Translate1.coor[2];
			Translate(vertex2, NumVer2, Translate1);

			sprintf(filename, "%s_to_%s.obj", destfn, srcfn);
			SaveObj(filename, vertex2, triangle2, NumVer2, NumTri2);

			// save two model
			// model 1 should translate and scale back to original
			Scale(vertex1, NumVer1, Scale1);
			Translate(vertex1, NumVer1, Translate1);
			sprintf(filename, "%s_to_%s_merge.obj", destfn, srcfn);
			SaveMergeObj(filename, vertex1, triangle1, NumVer1, NumTri1, vertex2, triangle2, NumVer2, NumTri2);

			for(i=0; i<4; i++)
				free(matrix[i]);
			free(matrix);

			// record execute time --- end
			finish = clock();
			fpt = fopen("time.txt", "a");
			fprintf(fpt, "%s to %s ( V: %d ; T: %d )\t: Corase: %.1f sec;", destfn, srcfn, NumVer1, NumTri1, (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);

			// write Err to file
			fpt2 = fopen("Coarse_Err.txt", "a");
			fprintf(fpt2, "Err of %s to %s : %f\n", destfn, srcfn, Err);
			fclose(fpt2);

			// ****************************************************************
			// Refine alignment
			// ****************************************************************

			// record execute time --- start
			start = clock();

			// refine Translate and scale between model 1 and 2
			TranslateScale(vertex2, NumVer2, destfn, &Translate2, &Scale2);
			TranslateScale(vertex1, NumVer1, srcfn, &Translate1, &Scale1);

			// refine rotate of model 2 to fit the model 1
			Err = Refine(src_ArtCoeff[UseCam], vertex2, triangle2, NumVer2, NumTri2, UseCam,
						 CamVertex[UseCam], CamTriangle[UseCam], CamNumVer[UseCam], CamNumTri[UseCam]);

			// refine Translate and scale of model 2 again
			TranslateScale(vertex2, NumVer2, destfn, &Translate2, &Scale2);

			// scale and translate back to the original state of model 1
			Scale1 = 1.0 / Scale1;
			Scale(vertex2, NumVer2, Scale1);

			Translate1.coor[0] = -Translate1.coor[0];
			Translate1.coor[1] = -Translate1.coor[1];
			Translate1.coor[2] = -Translate1.coor[2];
			Translate(vertex2, NumVer2, Translate1);

			// record execute time --- end
			finish = clock();
			fpt = fopen("time.txt", "a");
			fprintf(fpt, " Refine %.1f sec.\n", (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);

			sprintf(filename, "%s_to_%s_refine.obj", destfn, srcfn);
			SaveObj(filename, vertex2, triangle2, NumVer2, NumTri2);

			// save two model
			// model 1 should translate and scale back to original
			Scale(vertex1, NumVer1, Scale1);
			Translate(vertex1, NumVer1, Translate1);
			sprintf(filename, "%s_to_%s_merge_refine.obj", destfn, srcfn);
			SaveMergeObj(filename, vertex1, triangle1, NumVer1, NumTri1, vertex2, triangle2, NumVer2, NumTri2);

			// write Err to file
			fpt2 = fopen("Refine_Err.txt", "a");
			fprintf(fpt2, "Err of %s to %s : %f\n", destfn, srcfn, Err);
			fclose(fpt2);

			// free memory of 3D model
			free(vertex1);
			free(triangle1);
			free(vertex2);
			free(triangle2);
		}

		fclose(fpt1);
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			free(CamVertex[destCam]);
			free(CamTriangle[destCam]);
		}
		NumTri = 0;
		break;
*/
// *************************************************************************************************
	// calculate feature and save to file
	case 'n':
		// initialize ART
		GenerateBasisLUT();
		// initialize: read camera set
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			sprintf(filename, "12_%d", destCam);
			ReadObj(filename, CamVertex+destCam, CamTriangle+destCam, CamNumVer+destCam, CamNumTri+destCam);
		}

		for(i=0; i<CAMNUM; i++)
		{
			srcBuff[i] = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
			ColorBuff[i] = (unsigned char *) malloc (3 * winw * winh * sizeof(unsigned char));
		}
		YuvBuff = (unsigned char *) malloc (3 * winw * winh * sizeof(unsigned char));
		// add edge to test retrieval
		EdgeBuff = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));

		// for Fourier Descriptor
		total = winw * winh;
		Contour = (sPOINT *) malloc( total * sizeof(sPOINT));
		ContourMask = (unsigned char *) malloc( total * sizeof(unsigned char));

		fpt1 = fopen("list.txt", "r");
		fpt_art_q4 = fopen("all_q4_v1.8.art", "wb");
		fpt_art_q8 = fopen("all_q8_v1.8.art", "wb");
//		fpt_ccd = fopen("all_v1.7.ccd", "wb");
		fpt_cir_q8 = fopen("all_q8_v1.8.cir", "wb");
//		fpt_fd = fopen("all.fd", "wb");
		fpt_fd_q8 = fopen("all_q8_v1.8.fd", "wb");
		fpt_ecc_q8 = fopen("all_q8_v1.8.ecc", "wb");
		Count = 0;
		while( fgets(fname, 400, fpt1) )
		{
			// record execute time --- start
			start = clock();

			fname[strlen(fname)-1] = 0x00;
			// get the translatation and scale of the two model
			if( ReadObj(fname, &vertex1, &triangle1, &NumVer1, &NumTri1) == 0 )
				continue;

			// ****************************************************************
			// Corase alignment
			// ****************************************************************

			// Translate and scale model 1
			TranslateScale(vertex1, NumVer1, triangle1, NumTri1, fname, &Translate1, &Scale1);

			// read RED only, so size is winw*winh
			for(srcCam=0; srcCam<ANGLE; srcCam++)
			{
				// capture CAMNUM silhouette of srcfn to memory
				for(i=0; i<CAMNUM; i++)
//					RenderToMem(srcBuff[i], ColorBuff[i], CamVertex[srcCam]+i, vertex1, triangle1, NumVer1, NumTri1);
					RenderToMem(srcBuff[i], NULL, CamVertex[srcCam]+i, vertex1, triangle1, NumVer1, NumTri1);

				// find center for each shape
				for(i=0; i<CAMNUM; i++)
					FindCenter(srcBuff[i], winw, winh, CenX+i, CenY+i);

				// from silhouette
//				for(i=0; i<CAMNUM; i++)
//				{
//					EdgeDetectSil(srcBuff[i], winw, winh);
//					sprintf(fn, "%s_%2d_%02d.bmp", fname, srcCam, i);
//					WriteBitmap8(srcBuff[i], winw, winh, fn);
//				}

				// get color descriptor
//				for(i=0; i<CAMNUM; i++)
//				{
//					RGB_To_YUV(YuvBuff, ColorBuff[i], winw, winh);
//					ExtractCCD(YuvBuff, CompactColor[srcCam]+i, srcBuff[i]);
					// print the results of compact color
//					for(j=0; j<NUM_BINS-1; j++)
//						if( CompactColor[i] & (0x8000000000000000 >> j) )
//							printf("1");
//						else
//							printf("0");
//					printf("\n");
//				}

				// get Zernike moment
				FindRadius(srcBuff, CenX, CenY);
				for(i=0; i<CAMNUM; i++)
				{
					// from depth
					// EdgeDetect(EdgeBuff, srcBuff[i], winw, winh);
//					WriteBitmap8(srcBuff[i], winw, winh, "test1.bmp");
//					WriteBitmap8(EdgeBuff, winw, winh, "test2.bmp");
					ExtractCoefficients(srcBuff[i], src_ArtCoeff[srcCam][i], EdgeBuff, CenX[i], CenY[i]);
				}

				// get Fourier descriptor
				for(i=0; i<CAMNUM; i++)
					FourierDescriptor(src_FdCoeff[srcCam][i], srcBuff[i], winw, winh, Contour, ContourMask, CenX[i], CenY[i]);

				// get eccentricity
				for(i=0; i<CAMNUM; i++)
					ecc_Coeff[srcCam][i] = Eccentricity(srcBuff[i], winw, winh, CenX[i], CenY[i]);

				// get circularity
				for(i=0; i<CAMNUM; i++)
				{
					EdgeDetectSil(EdgeBuff, srcBuff[i], winw, winh);
					cir_Coeff[srcCam][i] = Circularity(srcBuff[i], winw, winh, EdgeBuff);
				}

			}

			// free memory of 3D model
			free(vertex1);
			free(triangle1);

			// record execute time --- end
			finish = clock();
			fpt = fopen("feature_time.txt", "a");
			fprintf(fpt, "%s ( V: %d T: %d )\t: %f sec;\n", fname, NumVer1, NumTri1, (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);

			// **********************************************************************
			// save ART feature to file
//			sprintf(filename, "%s_v1.7.art", fname);
//			fpt = fopen(filename, "wb");
//			fwrite(src_ArtCoeff, ANGLE * CAMNUM * ART_ANGULAR * ART_RADIAL, sizeof(double), fpt);
//			fclose(fpt);

			// linear Quantization to 8 bits for each coefficient
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					// the order is the same with that defined in MPEG-7, total 35 coefficients
					k = 0;
					p = 0;
					for(r=1 ; r<ART_RADIAL ; r++, k++)
					{
						itmp = (int)(QUANT8 *  src_ArtCoeff[i][j][p][r]);
						if(itmp>255)
							q8_ArtCoeff[i][j][k] = 255;
						else
							q8_ArtCoeff[i][j][k] = itmp;
					}

					for(p=1; p<ART_ANGULAR ; p++)
						for(r=0 ; r<ART_RADIAL ; r++, k++)
						{
							itmp = (int)(QUANT8 *  src_ArtCoeff[i][j][p][r]);
							if(itmp>255)
								q8_ArtCoeff[i][j][k] = 255;
							else
								q8_ArtCoeff[i][j][k] = itmp;
						}
				}
			// save to disk
			fwrite(q8_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF, fpt_art_q8);
			sprintf(filename, "%s_q8_v1.8.art", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q8_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF, fpt);
			fclose(fpt);

			// non-linear Quantization to 4 bits for each coefficient using MPEG-7 quantization table
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					// the order is the same with that defined in MPEG-7, total 35 coefficients
					k = 0;
					p = 0;
					for(r=1 ; r<ART_RADIAL ; r++, k++)
					{
						high = 17;
						low = 0;
						while(high-low > 1)
						{
							middle = (high+low) / 2;

							if(QuantTable[middle] < src_ArtCoeff[i][j][p][r])
								low = middle;
							else
								high = middle;
						}
						q8_ArtCoeff[i][j][k] = low;
					}
					for(p=1; p<ART_ANGULAR ; p++)
						for(r=0 ; r<ART_RADIAL ; r++, k++)
						{
							high = 17;
							low = 0;
							while(high-low > 1)
							{
								middle = (high+low) / 2;

								if(QuantTable[middle] < src_ArtCoeff[i][j][p][r])
									low = middle;
								else
									high = middle;
							}
							q8_ArtCoeff[i][j][k] = low;
						}
				}

			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
					for(k=0, a=0; k<ART_COEF; k+=2, a++)
						if( k+1 < ART_COEF )
							q4_ArtCoeff[i][j][a] = ( (q8_ArtCoeff[i][j][k] << 4) & 0xf0 ) | 
												( q8_ArtCoeff[i][j][k+1] & 0x0f );
						else
							q4_ArtCoeff[i][j][a] = ( (q8_ArtCoeff[i][j][k] << 4) & 0xf0 );

			// save to disk
			fwrite(q4_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF_2, fpt_art_q4);
			sprintf(filename, "%s_q4_v1.8.art", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q4_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF_2, fpt);
			fclose(fpt);

			// **********************************************************************
			// save color descriptor to disk
//			fwrite(CompactColor, sizeof(unsigned char), ANGLE * CAMNUM * sizeof(unsigned __int64), fpt_ccd);
//			sprintf(filename, "%s.ccd", fname);
//			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
//			fwrite(CompactColor, sizeof(unsigned char), ANGLE * CAMNUM * sizeof(unsigned __int64), fpt);
//			fclose(fpt);

			// **********************************************************************
			// save circularity feature to file
//			sprintf(filename, "%s.cir", fname);
//			fpt = fopen(filename, "wb");
//			fwrite(cir_Coeff, ANGLE * CAMNUM, sizeof(double), fpt);
//			fclose(fpt);

			// linear Quantization to 8 bits for each coefficient
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					itmp = (int)(QUANT8 *  cir_Coeff[i][j]);
					if(itmp>255)		q8_cirCoeff[i][j] = 255;
					else				q8_cirCoeff[i][j] = itmp;
				}
			// save to disk
			fwrite(q8_cirCoeff, sizeof(unsigned char), ANGLE * CAMNUM, fpt_cir_q8);
			sprintf(filename, "%s_q8_v1.8.cir", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q8_cirCoeff, sizeof(unsigned char), ANGLE * CAMNUM, fpt);
			fclose(fpt);

			// **********************************************************************
			// save eccentricity feature to file
			// linear Quantization to 8 bits for each coefficient
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					itmp = (int)(QUANT8 * ecc_Coeff[i][j]);
					if(itmp>255)		q8_eccCoeff[i][j] = 255;
					else				q8_eccCoeff[i][j] = itmp;
				}
			// save to disk
			fwrite(q8_eccCoeff, sizeof(unsigned char), ANGLE * CAMNUM, fpt_ecc_q8);
			sprintf(filename, "%s_q8_v1.8.ecc", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q8_eccCoeff, sizeof(unsigned char), ANGLE * CAMNUM, fpt);
			fclose(fpt);

			// **********************************************************************
			// save Fourier descriptor feature to file
//			fwrite(src_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt_fd);
//			sprintf(filename, "%s.fd", fname);
//			fpt = fopen(filename, "wb");
//			fwrite(src_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt);
//			fclose(fpt);

			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					for(k=0; k<FD_COEFF_NO; k++)
					{
						itmp = (int)(QUANT8 * FD_SCALE * src_FdCoeff[i][j][k]);
						if(itmp>255)
							q8_FdCoeff[i][j][k] = 255;
						else
							q8_FdCoeff[i][j][k] = itmp;
					}
				}

			fwrite(q8_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(unsigned char), fpt_fd_q8);
			sprintf(filename, "%s_q8_v1.8.fd", fname);
			fpt = fopen(filename, "wb");
			fwrite(q8_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(unsigned char), fpt);
			fclose(fpt);

//			printf("%d.%s OK.\n", Count++, fname);
			printf("%d.", Count++);
		}

		for(i=0; i<CAMNUM; i++)
		{
			free(srcBuff[i]);
			free(ColorBuff[i]);
		}
		free(YuvBuff);
		free(EdgeBuff);
		free(Contour);
		free(ContourMask);
		fclose(fpt1);
		fclose(fpt_art_q8);
		fclose(fpt_art_q4);
//		fclose(fpt_ccd);
		fclose(fpt_cir_q8);
		fclose(fpt_ecc_q8);
//		fclose(fpt_fd);
		fclose(fpt_fd_q8);
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			free(CamVertex[destCam]);
			free(CamTriangle[destCam]);
		}
		NumTri = 0;
		break;

// *************************************************************************************************
	// calculate color feature only
	case 'm':
		// initialize ART
//		GenerateBasisLUT();
		// initialize: read camera set
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			sprintf(filename, "12_%d", destCam);
			ReadObj(filename, CamVertex+destCam, CamTriangle+destCam, CamNumVer+destCam, CamNumTri+destCam);
		}

		for(i=0; i<CAMNUM; i++)
		{
			srcBuff[i] = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
			ColorBuff[i] = (unsigned char *) malloc (3 * winw * winh * sizeof(unsigned char));
		}
		YuvBuff = (unsigned char *) malloc (3 * winw * winh * sizeof(unsigned char));
		// add edge to test retrieval
		EdgeBuff = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
		fpt1 = fopen("list.txt", "r");
		fpt4 = fopen("all.ccd", "wb");
		while( fgets(fname, 400, fpt1) )
		{
			// record execute time --- start
			start = clock();

			fname[strlen(fname)-1] = 0x00;
			// get the translatation and scale of the two model
			if( ReadObj(fname, &vertex1, &triangle1, &NumVer1, &NumTri1) == 0 )
				continue;

			// ****************************************************************
			// Corase alignment
			// ****************************************************************

			// Translate and scale model 1
			TranslateScale(vertex1, NumVer1, triangle1, NumTri1, fname, &Translate1, &Scale1);

			// read RED only, so size is winw*winh
			for(srcCam=0; srcCam<ANGLE; srcCam++)
			{
				// capture CAMNUM silhouette of srcfn to memory
				for(i=0; i<CAMNUM; i++)
					RenderToMem(srcBuff[i], ColorBuff[i], CamVertex[srcCam]+i, vertex1, triangle1, NumVer1, NumTri1);

				// from silhouette
//				for(i=0; i<CAMNUM; i++)
//				{
//					EdgeDetectSil(srcBuff[i], winw, winh);
//					sprintf(fn, "%s_%2d_%02d.bmp", fname, srcCam, i);
//					WriteBitmap8(srcBuff[i], winw, winh, fn);
//				}

				// get color descriptor
				for(i=0; i<CAMNUM; i++)
				{
					RGB_To_YUV(YuvBuff, ColorBuff[i], WIDTH, HEIGHT);
					ExtractCCD(YuvBuff, CompactColor[srcCam]+i, srcBuff[i]); // use srcBuff[i] as mask
					// print the results of compact color
//					for(j=0; j<NUM_BINS-1; j++)
//						if( CompactColor[i] & (0x8000000000000000 >> j) )
//							printf("1");
//						else
//							printf("0");
//					printf("\n");
				}

//				FindRadius(srcBuff);

//				for(i=0; i<CAMNUM; i++)
//				{
					// from depth
					// EdgeDetect(EdgeBuff, srcBuff[i], winw, winh);
//					WriteBitmap8(srcBuff[i], winw, winh, "test1.bmp");
//					WriteBitmap8(EdgeBuff, winw, winh, "test2.bmp");
//					ExtractCoefficients(srcBuff[i], src_ArtCoeff[srcCam][i], EdgeBuff);
//				}
			}

			// free memory of 3D model
			free(vertex1);
			free(triangle1);

			// record execute time --- end
			finish = clock();
			fpt = fopen("feature_time.txt", "a");
			fprintf(fpt, "%s ( V: %d T: %d )\t: %f sec;\n", fname, NumVer1, NumTri1, (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);

			// save feature to file
/*			sprintf(filename, "%s.art", fname);
			fpt = fopen(filename, "wb");
			fwrite(src_ArtCoeff, ANGLE * CAMNUM * ART_ANGULAR * ART_RADIAL, sizeof(double), fpt);
			fclose(fpt);

			// linear Quantization to 8 bits for each coefficient
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					src_ArtCoeff[i][j][0][0] = 1.0;

					for(p=0, k=0; p<ART_ANGULAR ; p++)
						for(r=0 ; r<ART_RADIAL ; r++, k++)
						{
							itmp = (int)(256 *  src_ArtCoeff[i][j][p][r]);
							if(itmp>255)
								q8_ArtCoeff[i][j][k] = 255;
							else
								q8_ArtCoeff[i][j][k] = itmp;
						}
				}
			// save to disk
			fwrite(q8_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF, fpt3);
			sprintf(filename, "%s_q8.art", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q8_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF, fpt);
			fclose(fpt);

			// non-linear Quantization to 4 bits for each coefficient using MPEG-7 quantization table
			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
				{
					src_ArtCoeff[i][j][0][0] = 1;	// no need this coefficient

					for(p=0, k=0; p<ART_ANGULAR ; p++)
						for(r=0 ; r<ART_RADIAL ; r++, k++)
						{
							high = 17;
							low = 0;
							while(high-low > 1)
							{
								middle = (high+low) >> 1;	// (high+low)/2

								if(QuantTable[middle] < src_ArtCoeff[i][j][p][r])
									low = middle;
								else
									high = middle;
							}
							q8_ArtCoeff[i][j][k] = low;
						}
				}

			for(i=0; i<ANGLE; i++)
				for(j=0; j<CAMNUM; j++)
					for(k=0, a=0; k<ART_COEF; k+=2, a++)
//						q4_ArtCoeff[i][j][a] = q8_ArtCoeff[i][j][k] | (q8_ArtCoeff[i][j][k+1] << 4);
						q4_ArtCoeff[i][j][a] = ( q8_ArtCoeff[i][j][k] & 0x0f ) | 
											( (q8_ArtCoeff[i][j][k+1] << 4) & 0xf0 );

			// save to disk
			fwrite(q4_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF_2, fpt2);
			sprintf(filename, "%s_q4.art", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(q4_ArtCoeff, sizeof(unsigned char), ANGLE * CAMNUM * ART_COEF_2, fpt);
			fclose(fpt);
*/
			// save color descriptor to disk
			fwrite(CompactColor, sizeof(unsigned char), ANGLE * CAMNUM * sizeof(unsigned __int64), fpt4);
			sprintf(filename, "%s.ccd", fname);
			if( (fpt = fopen(filename, "wb")) == NULL )	{	printf("Write %s error!!\n", filename);	return;	}
			fwrite(CompactColor, sizeof(unsigned char), ANGLE * CAMNUM * sizeof(unsigned __int64), fpt);
			fclose(fpt);
		}

		for(i=0; i<CAMNUM; i++)
		{
			free(srcBuff[i]);
			free(ColorBuff[i]);
		}
		free(YuvBuff);
		free(EdgeBuff);
		fclose(fpt1);
//		fclose(fpt2);
//		fclose(fpt3);
		fclose(fpt4);
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			free(CamVertex[destCam]);
			free(CamTriangle[destCam]);
		}
		NumTri = 0;
		break;

// *************************************************************************************************
	// calculate fourier feature only
/*	case 'f':
		// initialize: read camera set
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			sprintf(filename, "12_%d", destCam);
			ReadObj(filename, CamVertex+destCam, CamTriangle+destCam, CamNumVer+destCam, CamNumTri+destCam);
		}

		for(i=0; i<CAMNUM; i++)
			srcBuff[i] = (unsigned char *) malloc (winw * winh * sizeof(unsigned char));
		fpt1 = fopen("list.txt", "r");
		fpt3 = fopen("all.fd", "wb");
		while( fgets(fname, 400, fpt1) )
		{
			// record execute time --- start
 			start = clock();

			fname[strlen(fname)-1] = 0x00;
			// get the translatation and scale of the two model
			if( ReadObj(fname, &vertex1, &triangle1, &NumVer1, &NumTri1) == 0 )
				continue;

			// Translate and scale model 1
			TranslateScale(vertex1, NumVer1, triangle1, NumTri1, fname, &Translate1, &Scale1);

			// read DEPTH only, so size is winw*winh
			for(srcCam=0; srcCam<ANGLE; srcCam++)
			{
				// capture CAMNUM silhouette of srcfn to memory
				for(i=0; i<CAMNUM; i++)
				{
					RenderToMem(srcBuff[i], NULL, CamVertex[srcCam]+i, vertex1, triangle1, NumVer1, NumTri1);
					FourierDescriptor(src_FdCoeff[srcCam][i], srcBuff[i], winw, winh);
				}
			}

			// free memory of 3D model
			free(vertex1);
			free(triangle1);

			// record execute time --- end
			finish = clock();

//fpt = fopen(filename, "wt");
//for(srcCam=0; srcCam<ANGLE; srcCam++)
//for(i=0; i<CAMNUM; i++)
//{
//	for(j=0; j<FD_COEFF_NO; j++)
//		fprintf(fpt, "%3f ", src_FdCoeff[srcCam][i][j]);
//	fprintf(fpt, "\n");
//}
			// save feature to file
			fwrite(src_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt3);
			sprintf(filename, "%s.fd", fname);
			fpt = fopen(filename, "wb");
			fwrite(src_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt);
			fclose(fpt);

			// record execute time --- end
			fpt = fopen("feature_time.txt", "a");
			fprintf(fpt, "%s ( V: %d T: %d )\t: %f sec;\n", fname, NumVer1, NumTri1, (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);
		}

		for(i=0; i<CAMNUM; i++)
			free(srcBuff[i]);
		fclose(fpt1);
		fclose(fpt3);
		for(destCam=0; destCam<ANGLE; destCam++)
		{
			free(CamVertex[destCam]);
			free(CamTriangle[destCam]);
		}
		NumTri = 0;
		break;
*/
// *************************************************************************************************
	// compare one model to all other models (ART)
	case 'd':
		// initialize: read camera pair
		fpt = fopen("align20.txt", "r");
		for(i=0; i<60; i++)
			for(j=0; j<CAMNUM_2; j++)
				fscanf(fpt, "%d", &align[i][j]);
		fclose(fpt);

		// read filename of two models
		fpt1 = fopen("compare.txt", "r");
		if( fscanf(fpt1, "%s", srcfn) == EOF )
			break;
		fclose(fpt1);

		// read coefficient from model 1
		sprintf(filename, "%s_otho.art", srcfn);
		if( (fpt = fopen(filename, "rb")) == NULL )
		{
			printf("%s does not exist.\n", filename);
			break;
		}

/*		for(srcCam=0; srcCam<ANGLE; srcCam++)
			for(i=0; i<CAMNUM; i++)
				for(p=0 ; p<ART_ANGULAR ; p++)
					for(r=0 ; r<ART_RADIAL ; r++)
						fscanf(fpt, "%lf", &src_ArtCoeff[srcCam][i][p][r]);
*/
		fread(src_ArtCoeff, ANGLE * CAMNUM * ART_ANGULAR * ART_RADIAL, sizeof(double), fpt);
		fclose(fpt);

		// read feature of all models
		fpt1 = fopen("list.txt", "r");
		Count = 0;
		pSearch = NULL;
		while( fscanf(fpt1, "%s", destfn) != EOF )
		{
			// read coefficient from model 2
			sprintf(filename, "%s_otho.art", destfn);
			if( (fpt = fopen(filename, "rb")) == NULL )
			{
				printf("%s does not exist.\n", filename);
				break;
			}

/*			for(destCam=0; destCam<ANGLE; destCam++)
				for(i=0; i<CAMNUM; i++)
					for(p=0 ; p<ART_ANGULAR ; p++)
						for(r=0 ; r<ART_RADIAL ; r++)
							fscanf(fpt, "%lf", &dest_ArtCoeff[destCam][i][p][r]);
*/
			fread(dest_ArtCoeff, ANGLE * CAMNUM * ART_ANGULAR * ART_RADIAL, sizeof(double), fpt);
			fclose(fpt);

			// compare each coefficients pair from the two models first
			for(destCam=0; destCam<ANGLE; destCam++)
				for(i=0; i<CAMNUM_2; i++)
					for(srcCam=0; srcCam<ANGLE; srcCam++)
						for(j=0; j<CAMNUM_2; j++)
							cost[srcCam][destCam][j][i] = GetDistance(dest_ArtCoeff[destCam][CamMap[i]], src_ArtCoeff[srcCam][CamMap[j]]);

			// find minimum error of the two models from all camera pairs
			MinErr = DBL_MAX;
			for(srcCam=0; srcCam<ANGLE; srcCam++)		// each src angle
				for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
					for(i=0; i<60; i++)						// each align
					{
						err = 0;
						for(j=0; j<CAMNUM_2; j++)				// each vertex
							err += cost[srcCam][destCam][CamMap[j]][CamMap[align[i][j]]];

						if( err < MinErr )
							MinErr = err;
					}

//			printf("Difference of %s and %s: %f\n", srcfn, destfn, MinErr);
			// add to a list
			pmr = (pMatRes) malloc (sizeof(MatRes));
			strcpy(pmr->name, destfn);
			pmr->sim = MinErr;
			pmr->pointer = pSearch;
			pSearch = pmr;
			Count ++;
		}
		
		TopNum = 10;		// show top 10

		pTop = (pMatRes) malloc ( TopNum * sizeof(MatRes) );
		for(i=0; i<TopNum; i++)
		{
			pTop[i].sim = DBL_MAX;
			strcpy(pTop[i].name, "");
		}

		for(pmr=pSearch; pmr; pmr=pmr->pointer)
			for(i=0; i<TopNum; i++)
				if( pmr->sim < pTop[i].sim )
				{
					for(j=TopNum-2; j>=i; j--)
					{
						strcpy(pTop[j+1].name, pTop[j].name);
						pTop[j+1].sim = pTop[j].sim;
					}
					strcpy(pTop[i].name, pmr->name);
					pTop[i].sim = pmr->sim;
					break;
				}

		printf("%s\n", srcfn);
		for(i=0; i<TopNum && i<Count; i++)
			printf("%s %.6f\n", pTop[i].name, pTop[i].sim);
		printf("\n");

		pmr=pSearch;
		while(pmr)
		{
			pmrr = pmr;
			pmr = pmr->pointer;
			free(pmrr);
		}
		free(pTop);

		break;

// *************************************************************************************************
	// compare one model to all other models using color
	case 'x':
		// initialize: read camera pair
		fpt = fopen("align20.txt", "r");
		for(i=0; i<60; i++)
			for(j=0; j<CAMNUM_2; j++)
				fscanf(fpt, "%d", &align[i][j]);
		fclose(fpt);

		// read filename of two models
		fpt1 = fopen("compare.txt", "r");
		if( fscanf(fpt1, "%s", srcfn) == EOF )
			break;
		fclose(fpt1);

		// read coefficient from model 1
		sprintf(filename, "%s.ccd", srcfn);
		if( (fpt = fopen(filename, "rb")) == NULL )
		{
			printf("%s does not exist.\n", filename);
			break;
		}

		fread(CompactColor, ANGLE * CAMNUM, sizeof(unsigned __int64), fpt);
		fclose(fpt);

		// read feature of all models
		fpt1 = fopen("list.txt", "r");
		Count = 0;
		pSearch = NULL;
		while( fscanf(fpt1, "%s", destfn) != EOF )
		{
			// read coefficient from model 2
			sprintf(filename, "%s.ccd", destfn);
			if( (fpt = fopen(filename, "rb")) == NULL )
			{
				printf("%s does not exist.\n", filename);
				break;
			}

			fread(dest_CompactColor, ANGLE * CAMNUM,  sizeof(unsigned __int64), fpt);
			fclose(fpt);

			// compare each coefficients pair from the two models first
			for(destCam=0; destCam<ANGLE; destCam++)
				for(i=0; i<CAMNUM_2; i++)
					for(srcCam=0; srcCam<ANGLE; srcCam++)
						for(j=0; j<CAMNUM_2; j++)
							cost[srcCam][destCam][j][i] = ColorDistance(dest_CompactColor[destCam]+CamMap[i], CompactColor[srcCam]+CamMap[j]);

			// find minimum error of the two models from all camera pairs
			MinErr = DBL_MAX;
			for(srcCam=0; srcCam<ANGLE; srcCam++)		// each src angle
				for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
					for(i=0; i<60; i++)						// each align
					{
						err = 0;
						for(j=0; j<CAMNUM_2; j++)				// each vertex
							err += cost[srcCam][destCam][CamMap[j]][CamMap[align[i][j]]];

						if( err < MinErr )
							MinErr = err;
					}

//			printf("Difference of %s and %s: %f\n", srcfn, destfn, MinErr);
			// add to a list
			pmr = (pMatRes) malloc (sizeof(MatRes));
			strcpy(pmr->name, destfn);
			pmr->sim = MinErr;
			pmr->pointer = pSearch;
			pSearch = pmr;
			Count ++;
		}

		TopNum = 10;		// show top 10

		pTop = (pMatRes) malloc ( TopNum * sizeof(MatRes) );
		for(i=0; i<TopNum; i++)
		{
			pTop[i].sim = DBL_MAX;
			strcpy(pTop[i].name, "");
		}

		for(pmr=pSearch; pmr; pmr=pmr->pointer)
			for(i=0; i<TopNum; i++)
				if( pmr->sim < pTop[i].sim )
				{
					for(j=TopNum-2; j>=i; j--)
					{
						strcpy(pTop[j+1].name, pTop[j].name);
						pTop[j+1].sim = pTop[j].sim;
					}
					strcpy(pTop[i].name, pmr->name);
					pTop[i].sim = pmr->sim;
					break;
				}

		printf("%s\n", srcfn);
		for(i=0; i<TopNum && i<Count; i++)
			printf("%s %.6f\n", pTop[i].name, pTop[i].sim);
		printf("\n");

		pmr=pSearch;
		while(pmr)
		{
			pmrr = pmr;
			pmr = pmr->pointer;
			free(pmrr);
		}
		free(pTop);

		break;

// *************************************************************************************************
	// compare two models using circularity
	case 'z':
		// initialize: read camera pair
		fpt = fopen("align20.txt", "r");
		for(i=0; i<60; i++)
			for(j=0; j<CAMNUM_2; j++)
				fscanf(fpt, "%d", &align[i][j]);
		fclose(fpt);

		// read filename of two models
		fpt1 = fopen("compare.txt", "r");
		if( fscanf(fpt1, "%s", srcfn) == EOF )
			break;
		fclose(fpt1);

		// read coefficient from model 1
		sprintf(filename, "%s_q8.cir", srcfn);
		if( (fpt = fopen(filename, "rb")) == NULL )
		{
			printf("%s does not exist.\n", filename);
			break;
		}

		fread(q8_cirCoeff, ANGLE * CAMNUM, sizeof(unsigned char), fpt);
		fclose(fpt);

		// read feature of all models
		fpt1 = fopen("list.txt", "r");
		Count = 0;
		pSearch = NULL;
		while( fscanf(fpt1, "%s", destfn) != EOF )
		{
			// read coefficient from model 2
			sprintf(filename, "%s_q8.cir", destfn);
			if( (fpt = fopen(filename, "rb")) == NULL )
			{
				printf("%s does not exist.\n", filename);
				break;
			}

			fread(dest_cirCoeff, ANGLE * CAMNUM,  sizeof(unsigned char), fpt);
			fclose(fpt);

			// compare each coefficients pair from the two models first
			for(destCam=0; destCam<ANGLE; destCam++)
				for(i=0; i<CAMNUM_2; i++)
					for(srcCam=0; srcCam<ANGLE; srcCam++)
						for(j=0; j<CAMNUM_2; j++)
							cost[srcCam][destCam][j][i] = abs(q8_cirCoeff[srcCam][CamMap[j]] - dest_cirCoeff[destCam][CamMap[i]]);

			// find minimum error of the two models from all camera pairs
			MinErr = DBL_MAX;
			for(srcCam=0; srcCam<ANGLE; srcCam++)		// each src angle
				for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
					for(i=0; i<60; i++)						// each align
					{
						err = 0;
						for(j=0; j<CAMNUM_2; j++)				// each vertex
							err += cost[srcCam][destCam][CamMap[j]][CamMap[align[i][j]]];

						if( err < MinErr )
							MinErr = err;
					}

//			printf("Difference of %s and %s: %f\n", srcfn, destfn, MinErr);
			// add to a list
			pmr = (pMatRes) malloc (sizeof(MatRes));
			strcpy(pmr->name, destfn);
			pmr->sim = MinErr;
			pmr->pointer = pSearch;
			pSearch = pmr;
			Count ++;
		}

		TopNum = 10;		// show top 10

		pTop = (pMatRes) malloc ( TopNum * sizeof(MatRes) );
		for(i=0; i<TopNum; i++)
		{
			pTop[i].sim = DBL_MAX;
			strcpy(pTop[i].name, "");
		}

		for(pmr=pSearch; pmr; pmr=pmr->pointer)
			for(i=0; i<TopNum; i++)
				if( pmr->sim < pTop[i].sim )
				{
					for(j=TopNum-2; j>=i; j--)
					{
						strcpy(pTop[j+1].name, pTop[j].name);
						pTop[j+1].sim = pTop[j].sim;
					}
					strcpy(pTop[i].name, pmr->name);
					pTop[i].sim = pmr->sim;
					break;
				}

		printf("%s\n", srcfn);
		for(i=0; i<TopNum && i<Count; i++)
			printf("%s %.6f\n", pTop[i].name, pTop[i].sim);
		printf("\n");

		pmr=pSearch;
		while(pmr)
		{
			pmrr = pmr;
			pmr = pmr->pointer;
			free(pmrr);
		}
		free(pTop);
		break;

// *************************************************************************************************
	// compare two models using fourier descriptor
	case 'g':
		// initialize: read camera pair
		fpt = fopen("align20.txt", "r");
		for(i=0; i<60; i++)
			for(j=0; j<CAMNUM_2; j++)
				fscanf(fpt, "%d", &align[i][j]);
		fclose(fpt);

		// read filename of two models
		fpt1 = fopen("compare.txt", "r");
		if( fscanf(fpt1, "%s", srcfn) == EOF )
			break;
		fclose(fpt1);

		// read coefficient from model 1
		sprintf(filename, "%s.fd", srcfn);
		if( (fpt = fopen(filename, "rb")) == NULL )
		{	printf("%s does not exist.\n", filename);	break;	}
		fread(src_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt);
		fclose(fpt);

		// read feature of all models
		fpt1 = fopen("list.txt", "r");
		Count = 0;
		pSearch = NULL;
		while( fscanf(fpt1, "%s", destfn) != EOF )
		{
			// read coefficient from model 2
			sprintf(filename, "%s.fd", destfn);
			if( (fpt = fopen(filename, "rb")) == NULL )
			{	printf("%s does not exist.\n", filename);	break;	}
			fread(dest_FdCoeff, ANGLE * CAMNUM * FD_COEFF_NO, sizeof(double), fpt);
			fclose(fpt);

			// compare each coefficients pair from the two models first
			for(destCam=0; destCam<ANGLE; destCam++)
				for(i=0; i<CAMNUM_2; i++)
					for(srcCam=0; srcCam<ANGLE; srcCam++)
						for(j=0; j<CAMNUM_2; j++)
						{
							err = 0;
							for(k=0; k<FD_COEFF_NO; k++)						// each align
								err += abs(src_FdCoeff[srcCam][CamMap[j]][k]-dest_FdCoeff[destCam][CamMap[i]][k]);// * (src_FdCoeff[srcCam][j][k]-dest_FdCoeff[destCam][i][k]);

							cost[srcCam][destCam][j][i] = err;
						}

			// find minimum error of the two models from all camera pairs
			MinErr = DBL_MAX;
			for(srcCam=0; srcCam<ANGLE; srcCam++)		// each src angle
				for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
					for(i=0; i<60; i++)						// each align
					{
						err = 0;
						for(j=0; j<CAMNUM_2; j++)				// each vertex
							err += cost[srcCam][destCam][CamMap[j]][CamMap[align[i][j]]];

						if( err < MinErr )
							MinErr = err;
					}

//			printf("Difference of %s and %s: %f\n", srcfn, destfn, MinErr);
			// add to a list
			pmr = (pMatRes) malloc (sizeof(MatRes));
			strcpy(pmr->name, destfn);
			pmr->sim = MinErr;
			pmr->pointer = pSearch;
			pSearch = pmr;
			Count ++;
		}

		TopNum = 10;		// show top 10

		pTop = (pMatRes) malloc ( TopNum * sizeof(MatRes) );
		for(i=0; i<TopNum; i++)
		{
			pTop[i].sim = DBL_MAX;
			strcpy(pTop[i].name, "");
		}

		for(pmr=pSearch; pmr; pmr=pmr->pointer)
			for(i=0; i<TopNum; i++)
				if( pmr->sim < pTop[i].sim )
				{
					for(j=TopNum-2; j>=i; j--)
					{
						strcpy(pTop[j+1].name, pTop[j].name);
						pTop[j+1].sim = pTop[j].sim;
					}
					strcpy(pTop[i].name, pmr->name);
					pTop[i].sim = pmr->sim;
					break;
				}

		printf("%s\n", srcfn);
		for(i=0; i<TopNum && i<Count; i++)
			printf("%s %.6f\n", pTop[i].name, pTop[i].sim);
		printf("\n");

		pmr=pSearch;
		while(pmr)
		{
			pmrr = pmr;
			pmr = pmr->pointer;
			free(pmrr);
		}
		free(pTop);
		break;

	default:
		break;
	}
}

void init(void) 
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
}

void reshape (int w, int h)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
//	gluPerspective(90.0, (GLfloat) winw/(GLfloat) winh, 0.1, 10.0);
	glOrtho(-1, 1, -1, 1, 0.0, 2.0);
//	glOrtho(-1, 1, -1, 1, 0.5, 1.5);
	glViewport (0, 0, (GLsizei) winw, (GLsizei) winh); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1,0,0,0,0,0,0,1,0);
//	gluLookAt(CAMSCALE*-0.85065, CAMSCALE*0.61803, CAMSCALE*-0.20081, 0,0,0,0,1,0);
//	gluLookAt(-0.85065, 0.61803, -0.20081, 0,0,0,0,1,0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (WIDTH, HEIGHT); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	//   glutMouseFunc(mouse);
	//   glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
