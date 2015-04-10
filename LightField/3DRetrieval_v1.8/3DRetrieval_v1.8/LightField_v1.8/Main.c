#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>

#include "ds.h"

// ****************
// NNNNOOOOTTTTEEEE: the ModelNum should be the same as the number of all models
#define			ModelNum	4

// #1
unsigned char	q8_table[256][256];
unsigned char	src_ArtCoeff[SRC_ANGLE][CAMNUM][ART_COEF];
unsigned char	dest_ArtCoeff[ModelNum][SRC_ANGLE][CAMNUM][ART_COEF];
unsigned char	align10[60][CAMNUM_2];
int				cost[SRC_ANGLE][ANGLE][CAMNUM][CAMNUM];

// for Fourier Descriptor matching
unsigned char	src_FdCoeff_q8[ANGLE][CAMNUM][FD_COEF];
unsigned char	dest_FdCoeff_q8[ModelNum][ANGLE][CAMNUM][FD_COEF];

// for Circularity
unsigned char	src_CirCoeff_q8[ANGLE][CAMNUM];
unsigned char	dest_CirCoeff_q8[ModelNum][ANGLE][CAMNUM];

// for Eccentricity
unsigned char	src_EccCoeff_q8[ANGLE][CAMNUM];
unsigned char	dest_EccCoeff_q8[ModelNum][ANGLE][CAMNUM];

// #2
//double	SHsrc_Coeff[SH_KEY_NUM];
//double	SHdest_Coeff[ModelNum][SH_KEY_NUM];

// #3
//double	S3Dsrc_Coeff[S3D_KEY_NUM];
//double	S3Ddest_Coeff[ModelNum][S3D_KEY_NUM];


#define abs(a) ((a)>0)?(a):(-(a))
#define			RESAM		1000

// #1
void ShowResults(char *filename, pMatRes pTop, int ClassNum)
{
	FILE	*fpt;
	int		i;
//	char	fn[200];

	fpt = fopen(filename, "w");
	fprintf(fpt, "<html>\n<head><title>Results</title></head>\n\n<body>");

	for(i=0; i<5*ClassNum; i++)
	{
		fprintf(fpt, "<img src=\"..\\..\\GroundTruthDatabase\\%s.jpg\"> %s %d<br>\n", pTop[i].name, pTop[i].name, pTop[i].sim);
		if( i == ClassNum )
			fprintf(fpt, "\n<hr>\n");
	}

	fprintf(fpt, "\n</body></html>\n");
	fclose(fpt);

//	sprintf(fn, "\"d:\\program files\\internet explorer\\iexplore.exe \"%s\"\"", filename);
//	system(fn);
}

//******************************************************************************************************
// match from 20 shapes, and match 36 coeff for each shape, and each coeff has 4 bits
int MatchART_q8(unsigned char dest_ArtCoeff[SRC_ANGLE][CAMNUM][ART_COEF])
{
	int						i, j, srcCam, destCam;
	register unsigned char	m;
	int						err, MinErr;
	int						distance;
	static int				cost_q8[ANGLE][ANGLE][CAMNUM][CAMNUM];

	// compare each coefficients pair from the two models first
	for(srcCam=0; srcCam<ANGLE; srcCam++)
		for(destCam=0; destCam<ANGLE; destCam++)
			for(i=0; i<CAMNUM; i++)
				for(j=0; j<CAMNUM; j++)
				{
					// un-loop to speed-up
					// un-loop to speed-up
					m=0;
					distance = q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 11	
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 21
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 31
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];

					cost_q8[srcCam][destCam][i][j] = distance;
				}

	// find minimum error of the two models from all camera pairs
	MinErr = INT_MAX;
	for(srcCam=0; srcCam<SRC_ANGLE; srcCam++)		// each src angle
		for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
			for(j=0; j<60; j++)						// each align
			{
//					err = 0;
//					for(m=0; m<CAMNUM; m++)				// each vertex
//						err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				// un-loop to speed-up
				m=0;
				err = cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				if( err < MinErr )	MinErr = err;
			}

	return MinErr;
//	return MinErr<<3;
}



int MatchFD_q8(unsigned char dest_FdCoeff_q8[ANGLE][CAMNUM][FD_COEF])
{
	int						i, j, srcCam, destCam;
	register unsigned char	m;
	int						err, MinErr;
	int						distance;
	static int				cost_q8[ANGLE][ANGLE][CAMNUM][CAMNUM];

	// compare each coefficients pair from the two models first
	for(srcCam=0; srcCam<ANGLE; srcCam++)
		for(destCam=0; destCam<ANGLE; destCam++)
			for(i=0; i<CAMNUM; i++)
				for(j=0; j<CAMNUM; j++)
				{
					// un-loop to speed-up
					m=0;
					distance = q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];

					cost_q8[srcCam][destCam][i][j] = distance;
				}

	// find minimum error of the two models from all camera pairs
	MinErr = INT_MAX;
	for(srcCam=0; srcCam<SRC_ANGLE; srcCam++)		// each src angle
		for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
			for(j=0; j<60; j++)						// each align
			{
//					err = 0;
//					for(m=0; m<CAMNUM; m++)				// each vertex
//						err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				// un-loop to speed-up
				m=0;
				err = cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				if( err < MinErr )
					MinErr = err;
			}

	return MinErr<<1;
//	return MinErr;
}

/*
// #2
double MatchSH(double SHdest_Coeff[SH_KEY_NUM]) 
{
	double					distance;
	int						m;

	// compare each coefficients pair from the two models first
	distance = 0;
	for(m=0 ; m<SH_KEY_NUM ; m++)
		distance += abs(SHdest_Coeff[m] - SHsrc_Coeff[m]);

	return distance;
}

// #3
double MatchS3D(double S3Ddest_Coeff[S3D_KEY_NUM]) 
{
	double					distance;
	int						m;

	// compare each coefficients pair from the two models first
	distance = 0;
	for(m=0 ; m<S3D_KEY_NUM ; m++)
		distance += abs(S3Ddest_Coeff[m] - S3Dsrc_Coeff[m]);

	return distance;
}
*/

// matching FightField descriptor ( ART + Fourier )
int MatchLF(unsigned char dest_ArtCoeff[SRC_ANGLE][CAMNUM][ART_COEF], 
			unsigned char dest_FdCoeff_q8[ANGLE][CAMNUM][FD_COEF],
			unsigned char dest_CirCoeff_q8[ANGLE][CAMNUM],
			unsigned char dest_EccCoeff_q8[ANGLE][CAMNUM])
{
	int						i, j, srcCam, destCam;
	register unsigned char	m;
	int						err, MinErr;
	int						art_distance, fd_distance, cir_distance, ecc_distance;
	static int				cost_q8[ANGLE][ANGLE][CAMNUM][CAMNUM];

	// compare each coefficients pair from the two models first
	for(srcCam=0; srcCam<ANGLE; srcCam++)
		for(destCam=0; destCam<ANGLE; destCam++)
			for(i=0; i<CAMNUM; i++)
				for(j=0; j<CAMNUM; j++)
				{
					// un-loop to speed-up
					// for ART (Zernike moment)
					m=0;
					art_distance = q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 11	
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 21
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					// 31
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];
					art_distance += q8_table[dest_ArtCoeff[destCam][i][m]][src_ArtCoeff[srcCam][j][m++]];

					// for Fourier Descriptor
					m=0;
					fd_distance = q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance += q8_table[dest_FdCoeff_q8[destCam][i][m]][src_FdCoeff_q8[srcCam][j][m++]];
					fd_distance <<= 1;

					// for Circularity
					cir_distance = q8_table[dest_CirCoeff_q8[destCam][i]][src_CirCoeff_q8[srcCam][j]];
					cir_distance <<= 1;

					// for Eccentricity
					ecc_distance = q8_table[dest_EccCoeff_q8[destCam][i]][src_EccCoeff_q8[srcCam][j]];
					//ecc_distance <<= 1;

					cost_q8[srcCam][destCam][i][j] = art_distance + fd_distance + cir_distance + ecc_distance;
				}

	// find minimum error of the two models from all camera pairs
	MinErr = INT_MAX;
	for(srcCam=0; srcCam<SRC_ANGLE; srcCam++)		// each src angle
		for(destCam=0; destCam<ANGLE; destCam++)	// each dest angle
			for(j=0; j<60; j++)						// each align
			{
//					err = 0;
//					for(m=0; m<CAMNUM; m++)				// each vertex
//						err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				// un-loop to speed-up
				m=0;
				err = cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];
//				err += cost_q8[srcCam][destCam][align10[j][m]][align10[0][m++]];

				if( err < MinErr )
					MinErr = err;
			}

	return MinErr;
}

void main(int argc, char *argv[])
{
	// for region shape descriptor
	int				i, j, k, n, m, Count, TopNum, ClassNum, AllClassNum;
	pMatRes			pmr, pTop;
	int				SIM;
//	double			SIM;
//	clock_t			start, finish;
	FILE			*fpt, *fpt1, *fpt2, *fpt4;
	char			filename[200], cmpfn[200], srcfn[2000][200], destfn[200], c1[200];
	float			PreciseRecall[RESAM], OneClassPreciseRecall[RESAM], AllClassPreciseRecall[RESAM];
	float			locate, interpol_1, interpol_2;
	double			dtmp;
	clock_t			start, finish;

	// initialize: read camera pair
	fpt = fopen("q8_table", "rb");
	fread(q8_table, sizeof(unsigned char), 65536, fpt);
	fclose(fpt);

	// initialize: read camera pair
	fpt = fopen("align10.txt", "rb");
	fread(align10, sizeof(unsigned char), 60*CAMNUM_2, fpt);
	fclose(fpt);

	// read feature of all models
	fpt1 = fopen("list.txt", "r");
	Count = 0;
	while( fgets(destfn, 200, fpt1) )
	{
		// read coefficient from model 2
		destfn[strlen(destfn)-1] = 0x00;
		// #1
		sprintf(filename, "%s_q8_v1.8.art", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		fread(dest_ArtCoeff[Count], ANGLE * CAMNUM * ART_COEF, sizeof(unsigned char), fpt);
		fclose(fpt);
		sprintf(filename, "%s_q8_v1.8.fd", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		fread(dest_FdCoeff_q8[Count], sizeof(unsigned char),  ANGLE * CAMNUM * FD_COEF, fpt);
		fclose(fpt);
		sprintf(filename, "%s_q8_v1.8.cir", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		fread(dest_CirCoeff_q8[Count], sizeof(unsigned char),  ANGLE * CAMNUM, fpt);
		fclose(fpt);
		sprintf(filename, "%s_q8_v1.8.ecc", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		fread(dest_EccCoeff_q8[Count], sizeof(unsigned char),  ANGLE * CAMNUM, fpt);
		fclose(fpt);

/*		// #2
		sprintf(filename, "%s.sh", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		for(i=0, k=0; i<SPHERE_NUM; i++)
		{
			fscanf(fpt, "%s", c1);
			for(j=0 ;j<HAMONIC_NUM; j++, k++)
			{
				fscanf(fpt, "%lf", &dtmp);
				SHdest_Coeff[Count][k] = dtmp;
			}
		}
		fclose(fpt);

		// #3
		sprintf(filename, "%s.s3d", destfn);
		if( (fpt = fopen(filename, "rb")) == NULL )				{	printf("%s does not exist.\n", filename);	continue;	}
		for(i=0; i<S3D_KEY_NUM; i++)
		{
			fscanf(fpt, "%lf", &dtmp);
			S3Ddest_Coeff[Count][i] = dtmp;
		}
		fclose(fpt);
*/
		Count ++;
	}
	fclose(fpt1);

	if( Count != ModelNum )
	{	printf("Error!!\n");	return ;	}

	// ****************
	// NNNNOOOOTTTTEEEE: the ModelNum should be the same as the number of all models
//	ModelNum = 1833;
	TopNum = ModelNum;		// show top 10
	pmr = (pMatRes) malloc (ModelNum * sizeof(MatRes));
	pTop = (pMatRes) malloc ( TopNum * sizeof(MatRes) );

	// initialize
	for(i=0; i<RESAM; i++)
		AllClassPreciseRecall[i] = 0;
	AllClassNum = 0;

	// read filename of two models
	fpt4 = fopen("compare.txt", "r");
	while( fscanf(fpt4, "%s", cmpfn) != EOF )
	{
		sprintf(filename, "%s.txt", cmpfn);
		fpt2 = fopen(filename, "r");
		ClassNum = 0;
		while( fgets(srcfn[ClassNum], 200, fpt2) )
		{
			srcfn[ClassNum][strlen(srcfn[ClassNum])-1] = 0x00;
			ClassNum++;
		}
		fclose(fpt2);

		AllClassNum += ClassNum;
		// initialize
		for(i=0; i<RESAM; i++)
			OneClassPreciseRecall[i] = 0;

		for(n=0; n<ClassNum; n++)
		{
			// read coefficient from model
			// #1
			// ART
			sprintf(filename, "%s_q8_v1.8.art", srcfn[n]);
			if( (fpt = fopen(filename, "rb")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			fread(src_ArtCoeff, SRC_ANGLE * CAMNUM * ART_COEF, sizeof(unsigned char), fpt);
			fclose(fpt);
			// FD
			sprintf(filename, "%s_q8_v1.8.fd", srcfn[n]);
			if( (fpt = fopen(filename, "rb")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			fread(src_FdCoeff_q8, sizeof(unsigned char),  ANGLE * CAMNUM * FD_COEF, fpt);
			fclose(fpt);
			// CIR
			sprintf(filename, "%s_q8_v1.8.cir", srcfn[n]);
			if( (fpt = fopen(filename, "rb")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			fread(src_CirCoeff_q8, sizeof(unsigned char),  ANGLE * CAMNUM, fpt);
			fclose(fpt);
			// ECC
			sprintf(filename, "%s_q8_v1.8.ecc", srcfn[n]);
			if( (fpt = fopen(filename, "rb")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			fread(src_EccCoeff_q8, sizeof(unsigned char),  ANGLE * CAMNUM, fpt);
			fclose(fpt);

			// #2
			//SH
/*			sprintf(filename, "%s.sh", srcfn[n]);
			if( (fpt = fopen(filename, "r")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			for(i=0, k=0; i<SPHERE_NUM; i++)
			{
				fscanf(fpt, "%s", c1);
				for(j=0 ;j<HAMONIC_NUM; j++, k++)
				{
					fscanf(fpt, "%lf", &dtmp);
					SHsrc_Coeff[k] = dtmp;
				}
			}
			fclose(fpt);

			// #3
			//S3D
			sprintf(filename, "%s.s3d", srcfn[n]);
			if( (fpt = fopen(filename, "r")) == NULL )			{	printf("%s does not exist.\n", filename);	return;	}
			for(i=0; i<S3D_KEY_NUM; i++)
			{
				fscanf(fpt, "%lf", &dtmp);
				S3Dsrc_Coeff[i] = dtmp;
			}
			fclose(fpt);
*/

			// record execute time --- start
			start = clock();

			// FIRST MATCH
			fpt1 = fopen("list.txt", "r");
			Count = 0;
			while( fgets(destfn, 200, fpt1) )
			{
				destfn[strlen(destfn)-1] = 0x00;
				// Matching
				// to be 1.0 : 0.55 : 0.48
//				SIM =	(double)MatchART_q8(dest_ArtCoeff[Count]) + 
//						(double)MatchFD_q8(dest_FdCoeff_q8[Count]) + 
//						16 * MatchSH(SHdest_Coeff[Count]) + 
//						2048 * MatchS3D(S3Ddest_Coeff[Count]);
//				SIM = MatchART_q8(dest_ArtCoeff[Count]) + MatchFD_q8(dest_FdCoeff_q8[Count]);
				SIM = MatchLF(dest_ArtCoeff[Count], dest_FdCoeff_q8[Count], dest_CirCoeff_q8[Count], dest_EccCoeff_q8[Count]);

				// add to a list
				strcpy(pmr[Count].name, destfn);
				pmr[Count].sim = SIM;
				pmr[Count].index = Count;

				Count ++;
			}
			fclose(fpt1);

			// sort
			for(i=0; i<TopNum; i++)
			{
				pTop[i].sim = INT_MAX;
				strcpy(pTop[i].name, "");
			}
			for(k=0; k<ModelNum; k++)
				for(i=0; i<TopNum; i++)
					if( pmr[k].sim < pTop[i].sim )
					{
						for(j=TopNum-2; j>=i; j--)
						{
							strcpy(pTop[j+1].name, pTop[j].name);
							pTop[j+1].sim = pTop[j].sim;
							pTop[j+1].index = pTop[j].index;
						}
						strcpy(pTop[i].name, pmr[k].name);
						pTop[i].sim = pmr[k].sim;
						pTop[i].index = pmr[k].index;
						break;
					}
		
			// record execute time --- end
			finish = clock();
			fpt = fopen("compare_time.txt", "a");
			fprintf(fpt, "%s: %f sec\n", srcfn[n], (double)(finish - start) / CLOCKS_PER_SEC );
			fclose(fpt);

			// check if each item is in this same class
			for(k=0; k<TopNum; k++)
			{
				pTop[k].Recall = 0;
				for(m=0; m<ClassNum; m++)
					if( strcmp(pTop[k].name, srcfn[m]) == 0 )
					{
						pTop[k].Recall = 1;
						break;
					}
			}

			// 1 1 0 0 1 0 1 0 0 0 1 ...
			// 1 2 2 2 3 3 4 4 4 4 5 ...
			for(k=1; k<TopNum; k++)
				pTop[k].Recall += pTop[k-1].Recall;

			// 1 2 2 2 3 3 4 4 4  4  5 ...  r
			// - - - - - - - - -  -  - ...  - =  precise ratio
			// 1 2 3 4 5 6 7 8 9 10 11 ...  K
			for(k=0; k<TopNum; k++)
			{
				if( pTop[k].Recall < ClassNum )
					pTop[k].Precise = (float)pTop[k].Recall / (float)(k+1);
				else if( pTop[k].Recall == ClassNum )
				{
					pTop[k].Precise = (float)pTop[k].Recall / (float)(k+1);
					//Recall = k+1;
					break;
				}
			}

			// ******************************************************************************
			// copy pTop[k].Precise according to the order of Recall
			// check if each item is in this same class
			i = 0;
			for(k=0; k<TopNum && i<ClassNum; k++)
			{
				for(m=0; m<ClassNum; m++)
					if( strcmp(pTop[k].name, srcfn[m]) == 0 )
					{
						pTop[i++].Precise = pTop[k].Precise;
						break;
					}
			}

			// normaliza K (recall all) to 1000
			for(i=0; i<RESAM; i++)
			{
				locate = (float) (i * (ClassNum-1)) / (float) (RESAM-1);
				interpol_1 = locate - (int)locate;
				interpol_2 = 1.0f - interpol_1;
				PreciseRecall[i] =	interpol_2 * pTop[(int)locate].Precise +
									interpol_1 * pTop[(int)locate+1].Precise;
				// in average
				OneClassPreciseRecall[i] += PreciseRecall[i];
				// all class in average
				AllClassPreciseRecall[i] += PreciseRecall[i];
			}
//			sprintf(filename, "Results\\%s\\%s_%03d__LightField_v1.7.txt", cmpfn, cmpfn, n);
//			fpt = fopen(filename, "w");
	//		fprintf(fpt, "%s - Recall: %d/%d\n", srcfn[n], ClassNum, Recall);
//			for(i=0; i<RESAM; i++)
//				fprintf(fpt, "%f\n", PreciseRecall[i]);
//			fprintf(fpt, "\n");
//			fclose(fpt);

			printf("%s_%03d\n", cmpfn, n);

			//Show results
			sprintf(filename, "Results\\%s\\%s_%03d_v1.8.html", cmpfn, cmpfn, n);
			ShowResults(filename, pTop, ClassNum);
		}

		// in average
		sprintf(filename, "Results\\%s\\%s_v1.8.txt", cmpfn, cmpfn);
		fpt = fopen(filename, "w");
	//	fprintf(fpt, "%s - Recall: %d/%d\n", srcfn[n], ClassNum, Recall);
		for(i=0; i<RESAM; i++)
			fprintf(fpt, "%f\n", OneClassPreciseRecall[i] / ClassNum );
		fprintf(fpt, "\n");
		fclose(fpt);

		printf("\"%s\" OK.\n", cmpfn);
	}

	// all class in average
	sprintf(filename, "Results\\total_v1.8.txt");
	fpt = fopen(filename, "w");
//	fprintf(fpt, "%s - Recall: %d/%d\n", srcfn[n], ClassNum, Recall);
	for(i=0; i<RESAM; i++)
		fprintf(fpt, "%f\n", AllClassPreciseRecall[i] / AllClassNum );
	fprintf(fpt, "\n");
	fclose(fpt);

	printf("\"%s\" OK.\n", cmpfn);
	
	fclose(fpt4);
	
	free(pmr);
	free(pTop);
}


