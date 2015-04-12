#include "StdAfx.h"
#include "ModelKernel.h"
#include "../../MISC/LFD.h"
#include "../Scene.h"
#include "../SceneList.h"

CModelKernel::CModelKernel(void)
{
}


CModelKernel::~CModelKernel(void)
{
}

CModelKernel::CModelKernel(CSceneList *pSceneList)
{
	m_ModelList = pSceneList->GetModelList();
	m_sSceneNameList = pSceneList->GetSceneNameList();
	m_sSceneListPath = pSceneList->GetSceneListPath();
	m_sSceneListName = pSceneList->GetSceneListName();

	int idx = 0;

	for (int i = 0; i < m_ModelList.size(); i ++)
	{
		for (int j = 0; j < m_ModelList[i].size(); j++)
		{
			m_ModelList[i][j]->SetIdx(idx);

			std::pair<int, int> idx2D(i, j);
			m_IdMap.push_back(idx2D);

			idx++;
		}
	}

	m_iNum = idx;
}

void CModelKernel::ComputeLightFieldAffinity()
{
	CDistMat<FTP> DM;
	CString distFileName = gsAppPath+"\\Thirdpart\\LFD\\" + m_sSceneListName + "_dist_mat.txt";
	std::vector<std::vector<MatrixEntry>> matrix;

	std::ifstream ifs(distFileName);
	
	if (!ifs.good())
	{
		std::vector<CMultiCompObj*> ML;
		std::vector<CString> NL(m_iNum);

		int idx = 0;
		for (unsigned int i=0; i<m_ModelList.size(); i++) {
			for (unsigned int j = 0; j < m_ModelList[i].size(); j++)
			{
				ML.push_back(new CMultiCompObj((CMultiCompObj&) *m_ModelList[i][j]));
				NL[idx] = m_sSceneNameList[i]+"_"+m_ModelList[i][j]->GetFName();

				// Alignment the models by translation and rotation
				Matrix3& rotateM = m_ModelList[i][j]->GetOritationUnitizeMatrix3();
				ML[idx]->Alignment(rotateM);

				//CString fileName = ML[idx]->GetPath() + ML[idx]->GetFName() + ".obj";
				CString fileName = UNITIZED_OBJ_MODEL_PATH + NL[idx] + ".obj";

				std::ifstream ifs(fileName);
				if (!ifs.good())
				{
					ML[idx]->WriteCompAsSingleObj(fileName);
				}
				idx++;
			}
		}

		LFD::calc(ML, m_sSceneListName, NL, DM);

		for (int i=0; i < ML.size(); i++)
		{
			SAFE_DELETE(ML[i]);
		}

		ML.clear();
	}

	else{
		std::string s;
		std::getline(ifs,s);
		DM.n = StringToIntegerList(s, "N")[0];

		std::getline(ifs,s);	
		std::vector<FTP> distVec;

		while(!ifs.eof())
		{
			std::getline(ifs,s);
			if (s!="")
			{
				distVec.push_back(StringToInt(s));
			}
		}

		DM.m = distVec;
	}

	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;

		for (int j = 0; j < m_iNum; j++)
		{
			row.push_back(MatrixEntry(j, DM.Get(i, j)));
		}

		matrix.push_back(row);
	}

	m_FeatureDistAffinity.Clear();
	m_FeatureDistAffinity = CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::DistanceMatrix);
	m_FeatureDistNeighborMap = m_FeatureDistAffinity.ComputeNeighborMap();
}

void CModelKernel::ComputeXYSizeAffinity()
{
	// initialize size feature vector
	FTP maxEdge = 0;
	for (int i = 0; i < m_ModelList.size(); i++)
	{
		for (int j = 0; j < m_ModelList[i].size(); j++)
		{
			FTP sz [] = {m_ModelList[i][j]->m_GOBB.size[0], m_ModelList[i][j]->m_GOBB.size[1], m_ModelList[i][j]->m_GOBB.size[2] };

			std::vector<FTP> bbSize(sz, sz + 3);
			std::sort(bbSize.begin(), bbSize.begin() + 1);

			m_XYSizeFeature.push_back(bbSize);

			if (GetLargestStdVector(bbSize) > maxEdge)
			{
				maxEdge = GetLargestStdVector(bbSize);
			}
		}
	}

	// compute size affinity
	std::vector<std::vector<MatrixEntry>> matrix;
	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;
		for (int j = 0; j < m_iNum; j++)
		{
			FTP szDiff = EuclideanDistance(m_XYSizeFeature[i], m_XYSizeFeature[j]);

			//FTP thetaSz = min(maxEdge- m_XYSizeFeature[i][1], maxEdge - m_XYSizeFeature[j][1]);
			FTP s = exp(-pow(2*szDiff/maxEdge, 2));

			row.push_back(MatrixEntry(j, s));
		}

		matrix.push_back(row);
	}

	m_XYSizeKernelAffinity.Clear();
	m_XYSizeKernelAffinity =CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::SimilarityMatrix);
}

void CModelKernel::ComputeHeightAffinity()
{
	// initialize height vector
	FTP maxHeight = 0;
	for (int i = 0; i < m_ModelList.size(); i++)
	{
		for (int j = 0; j <  m_ModelList[i].size(); j++)
		{
			Vector3 sz = m_ModelList[i][j]->m_GOBB.size;
			FTP height = sz[2];
			m_HeightFeature.push_back(height);

			if (height > maxHeight)
			{
				maxHeight = height;
			}
		}
	}

	// compute height affinity
	std::vector<std::vector<MatrixEntry>> matrix;
	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;

		for (int j = 0; j < m_iNum; j++)
		{
			FTP heightDiff = m_HeightFeature[i] - m_HeightFeature[j];

			//FTP thetaHeight = min(maxHeight - m_HeightFeature[i], maxHeight - m_HeightFeature[j]);
			FTP s = exp(-pow(2*heightDiff/maxHeight, 2));

			row.push_back(MatrixEntry(j, s));
		}

		matrix.push_back(row);
	}

	m_HeightKernelAffinity.Clear();
	m_HeightKernelAffinity =CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::SimilarityMatrix);
}

void CModelKernel::ComputeVolAffinity()
{
	// initialize vol vector
	FTP maxVol = 0;
	for (int i = 0; i < m_ModelList.size(); i++)
	{
		for (int j = 0; j <  m_ModelList[i].size(); j++)
		{
			FTP vol = m_ModelList[i][j]->m_GOBB.vol;
			m_VolFeature.push_back(vol);

			if (vol > maxVol)
			{
				maxVol = vol;
			}
		}
	}

	std::vector<FTP> tempVolFeature(m_iNum);
	std::copy(m_VolFeature.begin(), m_VolFeature.end(), tempVolFeature.begin());
	std::sort(tempVolFeature.begin(), tempVolFeature.end());

	FTP midVol = tempVolFeature[m_iNum/2];

	// compute vol affinity
	std::vector<std::vector<MatrixEntry>> matrix;
	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;

		for (int j = 0; j < m_iNum; j++)
		{
			FTP volDiff = m_VolFeature[i] - m_VolFeature[j];

			//FTP thetaVol = min(maxVol - m_VolFeature[i], maxVol - m_VolFeature[j]);
			FTP s = exp(-pow(2*volDiff/maxVol, 2));

			row.push_back(MatrixEntry(j, s));
		}

		matrix.push_back(row);
	}

	m_VolKernelAffinity.Clear();
	m_VolKernelAffinity =CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::SimilarityMatrix);
}

void CModelKernel::ComputeGeoKernelAffinity(FTP featureWeight, FTP xySizeWeight, FTP heightWeight, FTP volWeight)
{
	m_dFeatureWeight = featureWeight;
	m_dXYSizeWeight = xySizeWeight;
	m_dHeightWeight = heightWeight;
	m_dVolWeight = volWeight;

	std::vector<std::vector<MatrixEntry>> matrix;

	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;

		for (int j = 0; j < m_iNum; j++)
		{
			double kernelValue = GetNodeGeoKernel(i, j);

			row.push_back(MatrixEntry(j, kernelValue));
		}

		matrix.push_back(row);
	}

	m_GeoKernelAffinity.Clear();
	m_GeoKernelAffinity = CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::SimilarityMatrix);
	CString fileName = m_sSceneListPath + "\\" + "GeoKernels\\" + m_sSceneListName + ".GeoAff";
	m_GeoKernelAffinity.Write(fileName);
}

void CModelKernel::ComputeGeoTagKernelAffinity( FTP tagWeight )
{
	std::vector<std::vector<MatrixEntry>> matrix;

	for (int i = 0; i < m_iNum; i++)
	{
		std::vector<MatrixEntry> row;

		CString tagI;
		int sceneIdx = m_IdMap[i].first;
		int modelIdx = m_IdMap[i].second;
		tagI = m_ModelList[sceneIdx][modelIdx]->GetName();

		for (int j = 0; j < m_iNum; j++)
		{
			double geoKernelValue = GetNodeGeoKernel(i, j);

			CString tagJ;
			int sceneIdx = m_IdMap[j].first;
			int modelIdx = m_IdMap[j].second;
			tagJ = m_ModelList[sceneIdx][modelIdx]->GetName();

			double tagKernelValue = (tagI==tagJ)?1:0;
			double finalKernelValue = (1-tagWeight)*geoKernelValue+tagWeight*tagKernelValue;

			row.push_back(MatrixEntry(j, finalKernelValue));
		}
		matrix.push_back(row);
	}

	m_GeoTagKernelAffinity.Clear();
	m_GeoTagKernelAffinity = CAffinityMatrix(m_iNum, matrix, CAffinityMatrix::SimilarityMatrix);
	CString fileName = m_sSceneListPath + "\\" + "GeoKernels\\" + m_sSceneListName + ".GeoTAff";
	m_GeoTagKernelAffinity.Write(fileName);
}

void CModelKernel::ComputeModelAffinity( std::vector<FTP> GeoParas, FTP tagWeight, CString kernelType )
{

	ComputeLightFieldAffinity();

	if (kernelType == "ModelClustering")
	{
		ComputeXYSizeAffinity();
		ComputeHeightAffinity();
		ComputeVolAffinity();
	}

	ComputeGeoKernelAffinity(GeoParas[0], GeoParas[1], GeoParas[2], GeoParas[3]);
	ComputeGeoTagKernelAffinity(tagWeight);
}

double CModelKernel::Distance2GeoNeighbor( int a , int n_neighbor )
{
	int neighbor_idx = m_FeatureDistNeighborMap[a][n_neighbor];
	return m_FeatureDistAffinity.GetEntryValue(a, neighbor_idx);
}

double CModelKernel::GetNodeGeoKernel( int a, int b )
{
	if (a == b) return 1;

	else
	{
		double distance = m_FeatureDistAffinity.GetEntryValue(a, b);
		double neighbor_distance;

		// normalize the distance, equvilent to local scaling
		if (m_iNum > 100)
		{
			neighbor_distance = std::min(Distance2GeoNeighbor(a, 100), Distance2GeoNeighbor(b, 100));
		}
		else
			neighbor_distance = std::min(Distance2GeoNeighbor(a, m_iNum-1), Distance2GeoNeighbor(b, m_iNum - 1));

		double featureKernel = exp(-pow(distance/neighbor_distance, 2));
		double kernelValue;

		if (m_XYSizeKernelAffinity.IsEmpty() && m_HeightKernelAffinity.IsEmpty() && m_VolKernelAffinity.IsEmpty())
		{
			kernelValue = featureKernel;
		}
		else
		{
			double xySizeKernel = m_XYSizeKernelAffinity.GetEntryValue(a, b);
			double heightKernel = m_HeightKernelAffinity.GetEntryValue(a, b);
			double volKernel = m_VolKernelAffinity.GetEntryValue(a, b);

			//double kernelValue = m_dFeatureWeight*featureKernel+ m_dXYSizeWeight*xySizeKernel + m_dHeightWeight*heightKernel +m_dVolWeight*volKernel;
			kernelValue = pow(featureKernel, m_dFeatureWeight)*pow(xySizeKernel, m_dXYSizeWeight)*pow(heightKernel, m_dHeightWeight)*pow(volKernel, m_dVolWeight);
		}

		if (kernelValue > 1 - 1e-6)
		{
			kernelValue = 1;
		}

		if (kernelValue < 1e-6)
		{
			kernelValue = 0;
		}

		return kernelValue;
	}
}

std::vector<std::vector<int>> CModelKernel::DoClustering( CAffinityMatrix &m, int numDim, CString clusterMethod )
{
	std::vector<std::vector<int>> clusters;

	if (clusterMethod == "NCut")
	{
		clusters = DoNCutClustering(m, numDim);
	}

	else if (clusterMethod=="Self-Tuning")
	{
		clusters = DoSelfTuningClustering(m, numDim);
	}

	return clusters;
}

std::vector<std::vector<int>> CModelKernel::DoSelfTuningClustering(CAffinityMatrix &m, int numDim)
{
	SpectralClustering* c = new SpectralClustering(m.ConvertToEigenFormat(), numDim);

	std::vector<std::vector<int>> clusters = c->clusterRotate();
	//std::vector<std::vector<int>> clusters = c->clusterKmeans(numDim);

	m_ModelClusters.clear();

	for (int i = 0; i <  clusters.size(); i++)
	{
		// filter empty cluster
		if (clusters[i].size() != 0)
		{
			std::sort(clusters[i].begin(), clusters[i].end());
			m_ModelClusters.push_back(clusters[i]);
		}
	}

	// assign cluster id to each model
	for (int i =0; i < m_ModelClusters.size(); i++)
	{
		for (int j = 0; j < m_ModelClusters[i].size(); j++)
		{
			int idx = m_ModelClusters[i][j];
			int sceneIdx = m_IdMap[idx].first;
			int modelIdx = m_IdMap[idx].second;
			m_ModelList[sceneIdx][modelIdx]->SetClusterID(i);
		}
	}

	return m_ModelClusters;
}

std::vector<std::vector<int>> CModelKernel::DoNCutClustering( CAffinityMatrix &m, int numDim )
{
	std::vector<unsigned int> assignmentVector = ncutW(m.ConvertToEigenFormat(), numDim);

	for (int i = 0; i < assignmentVector.size(); i++)
	{
		int sceneIdx = m_IdMap[i].first;
		int modelIdx = m_IdMap[i].second;

		m_ModelList[sceneIdx][modelIdx]->SetClusterID(assignmentVector[i]);
	}

	std::set<int> uniqueValueSet;

	for (int i = 0; i < assignmentVector.size(); i++)
	{
		uniqueValueSet.insert(assignmentVector[i]);
	}

	int clusterNum = uniqueValueSet.size();

	m_ModelClusters.clear();
	m_ModelClusters.resize(clusterNum);

	// need to correct the assignment vector to be consecutive since ncut return non-consecutive results
		int clusterID = 0;
		for (std::set<int>::const_iterator it = uniqueValueSet.begin(); it != uniqueValueSet.end(); it++)
		{
			int currClusterID = *it;
			if (currClusterID != clusterID)
			{
				for (int i = 0; i < assignmentVector.size(); i++)
				{
					if (assignmentVector[i] == currClusterID)
					{
						assignmentVector[i] = clusterID;
					}
				}
			}
			clusterID++;
		}

		for(int i = 0; i < assignmentVector.size(); i++)
		{
			m_ModelClusters[assignmentVector[i]].push_back(i);
		}

	return m_ModelClusters;
}

void CModelKernel::SaveModelClusters( CString filename )
{
	//std::ofstream file(filename);

	//if (file.good())
	//{
	//	file << "Num "<< m_ModelClusters.size() <<"\n";

	//	for (int i = 0; i < m_ModelClusters.size(); i++)
	//	{
	//		file << "C " << i << "\n";
	//		for (int j = 0; j < m_ModelClusters[i].size(); j++)
	//		{
	//			int idx = m_ModelClusters[i][j];
	//			int sceneIdx = m_IdMap[idx].first;
	//			int modelIdx = m_IdMap[idx].second;
	//			CString modelName = m_sSceneNameList[sceneIdx] + "_" 
	//				+ m_ModelList[sceneIdx][modelIdx]->GetFName();
	//			file<< modelName <<" ";
	//		}
	//		file<<"\n";
	//	}
	//}

	//file.close();

	std::ofstream file(filename);

	if (file.good())
	{
		for (int i = 0; i < m_ModelList.size(); i++)
		{
			for (int j = 0; j < m_ModelList[i].size(); j++)
			{
				file<<m_ModelList[i][j]->GetClusterID()<<"\n";
			}
		}
	}
}

int CModelKernel::ToGlobalModelIdx( int sceneIdx, int modelIdx )
{
	int idx = 0;

	for (int i = 0; i < sceneIdx; i++)
	{
		idx += m_ModelList[i].size();
	}

	idx += modelIdx;

	return idx;
}

void CModelKernel::NormalizeKernelTable( std::vector<std::vector<FTP>> &kernelTable )
{
	//FTP maxValue = 0;
	//for (int i = 0; i < kernelTable.size(); i++)
	//{
	//	if (kernelTable[i][i] > maxValue)
	//	{
	//		maxValue = kernelTable[i][i];
	//	}
	//}

	//FTP scalingFactor = 1/maxValue;
	//for (int i = 0; i < kernelTable.size(); i++)
	//{
	//	for (int j = 0; j < kernelTable[i].size(); j++)
	//	{
	//		kernelTable[i][j] = scalingFactor * kernelTable[i][j];
	//	}
	//}

	//for (int i = 0; i < kernelTable.size(); i++)
	//{
	//	double scaleFactor = 1/kernelTable[i][i];
	//	for (int j = 0; j < kernelTable[i].size(); j++)
	//	{
	//		kernelTable[i][j] = scaleFactor*kernelTable[i][j];
	//	}
	//}

	std::vector<FTP> diagVector;
	for (int i = 0; i < kernelTable.size(); i++)
	{
		diagVector.push_back(kernelTable[i][i]);
	}

	for (int i = 0; i < kernelTable.size(); i++)
	{
		for (int j = 0; j < kernelTable[i].size(); j++)
		{
			kernelTable[i][j] = kernelTable[i][j]/max(diagVector[i], diagVector[j]);
		}
	}
}

void CModelKernel::OpenAffinities()
{
	CString fileName = m_sSceneListPath + "\\" + "GeoKernels\\" + m_sSceneListName + ".GeoAff";
	m_GeoKernelAffinity.Load(fileName);
	m_GeoKernelAffinity.SetMatrixType(CAffinityMatrix::SimilarityMatrix);

	fileName = m_sSceneListPath + "\\" + "GeoKernels\\" + m_sSceneListName + ".GeoTAff";
	m_GeoTagKernelAffinity.Load(fileName);
	m_GeoTagKernelAffinity.SetMatrixType(CAffinityMatrix::SimilarityMatrix);
}

void CModelKernel::ComputeLightFieldDistBPWS()
{
	for (int si=1; si < m_ModelList.size();si++)
	{
		for (int sj=1; sj < m_ModelList.size();sj++)
		{
			ComputeLFDDistBetweenTwoScenes(si, sj);
		}
	}
}

void CModelKernel::ComputeLFDDistBetweenTwoScenes(int si, int sj)
{
	CDistMat<FTP> DM;

	CString listFileName = m_sSceneNameList[si] + "_" + m_sSceneNameList[sj];
	CString distFileName = m_sSceneListPath + "\\GeoKernels\\LFD\\" + listFileName + "_dist_mat.txt";

	std::ifstream ifs(distFileName);

	if (!ifs.good())
	{
		std::vector<CModel*> mListi = m_ModelList[si];
		std::vector<CModel*> mListj = m_ModelList[sj];

		int idx = 0;

		std::vector<CMultiCompObj*> ML;
		std::vector<CString> NL(mListi.size()+mListj.size());

		// collcet models from scene si
		for (int mi=0; mi < mListi.size(); mi++)
		{

			ML.push_back(new CMultiCompObj((CMultiCompObj&) *mListi[mi]));
			NL[idx] = m_sSceneNameList[si]+"_"+mListi[mi]->GetFName();

			//// Alignment the models by translation and rotation
			Matrix3& rotateM = mListi[mi]->GetOritationUnitizeMatrix3();
			ML[idx]->Alignment(rotateM);

			CString fileName = UNITIZED_OBJ_MODEL_PATH + NL[idx] + ".obj";

			std::ifstream ifs(fileName);
			if (!ifs.good())
			{
				ML[idx]->WriteCompAsSingleObj(fileName);
			}

			idx++;
		}

		// collcet models from scene sj
		for (int mj=0; mj < mListj.size(); mj++)
		{

			ML.push_back(new CMultiCompObj((CMultiCompObj&) *mListj[mj]));
			NL[idx] = m_sSceneNameList[sj]+"_"+mListj[mj]->GetFName();

			//// Alignment the models by translation and rotation
			Matrix3& rotateM = mListj[mj]->GetOritationUnitizeMatrix3();
			ML[idx]->Alignment(rotateM);

			CString fileName = UNITIZED_OBJ_MODEL_PATH + NL[idx] + ".obj";

			std::ifstream ifs(fileName);
			if (!ifs.good())
			{
				ML[idx]->WriteCompAsSingleObj(fileName);
			}

			idx++;
		}

		LFD::calc(ML, listFileName, NL, DM);

		for (int i=0; i < ML.size(); i++)
		{
			SAFE_DELETE(ML[i]);
		}

		ML.clear();
	}
}

void CModelKernel::ComputeLFDAffinityBetweenTwoScenes( int si, int sj )
{
		CString distFileName = m_sSceneListPath + "\\GeoKernels\\LFD\\" + m_sSceneNameList[si] + "-" + m_sSceneNameList[sj] + "_dist_mat.LFDSim";
		CDistMat<FTP> DM;
	
		// Read back result
		std::ifstream ifs(distFileName);
		if (!ifs.good()) {
			MSG_BOX_ERROR("LFD::calc: cannot open result file %s!", distFileName);
			return;
		}
		std::string sFlag;
	
		ifs >> sFlag;
		if (sFlag != "N") {
			MSG_BOX_ERROR("LFD::calc: error format! expected 'N'");
			return;
		}
		unsigned int iNum, iLen;
		ifs >> iNum;
		if (iNum != (m_ModelList[si].size()+m_ModelList[sj].size())) {
			MSG_BOX_ERROR("LFD::calc: scale of distance matrix is incorrect!");
			return ;
		}
		ifs >> sFlag;
		if (sFlag != "M") {
			MSG_BOX_ERROR("LFD::calc: error format! expected 'M'");
			return ;
		}
		ifs >> iLen;
		if (iLen != iNum*(iNum-1)/2) {
			MSG_BOX_ERROR("LFD::calc: size of distance matrix is incorrect!");
			return ;
		}
		DM.Resize(iNum);
		for (unsigned int i=0; i<iLen; i++) {
			ifs >> DM.m[i];
		}
		std::vector<std::vector<MatrixEntry>> matrix;

		for (int i = 0; i < iNum; i++)
		{
			std::vector<MatrixEntry> row;

			for (int j = 0; j < iNum; j++)
			{
				row.push_back(MatrixEntry(j, DM.Get(i, j)));
			}

			matrix.push_back(row);
		}

		CAffinityMatrix LFDDistAffinity = CAffinityMatrix(iNum, matrix, CAffinityMatrix::DistanceMatrix);
		std::vector<std::vector<int>> LFDNeighborMap = LFDDistAffinity.ComputeNeighborMap();

		matrix.clear();

		for (int i = 0; i < iNum; i++)
		{
			std::vector<MatrixEntry> row;

			for (int j = 0; j < iNum; j++)
			{
				double kernelValue = GetNodeGeoKernelBPWS(i, j, LFDDistAffinity, LFDNeighborMap);

				row.push_back(MatrixEntry(j, kernelValue));
			}

			matrix.push_back(row);
		}

		CAffinityMatrix LFDAffinity = CAffinityMatrix(iNum, matrix, CAffinityMatrix::SimilarityMatrix);
		CString fileName = m_sSceneListPath + "\\" + "GeoKernels\\LFD_Aff" + m_sSceneNameList[si] + "-" + m_sSceneNameList[sj] + ".LFDAff";
		LFDAffinity.Write(fileName);
		
}

double CModelKernel::Distance2GeonNeighborBPWS( int a, int n_neighbor, CAffinityMatrix &affnity, std::vector<std::vector<int>> &neighborMap )
{
	int neighbor_idx = neighborMap[a][n_neighbor];
	return affnity.GetEntryValue(a, neighbor_idx);
}

double CModelKernel::GetNodeGeoKernelBPWS( int a, int b, CAffinityMatrix &affnity, std::vector<std::vector<int>> &neighborMap  )
{
	if (a == b) return 1;

	else
	{
		double distance = affnity.GetEntryValue(a, b);
		double neighbor_distance;

		int modelNum = m_ModelList[a].size()+m_ModelList[b].size();
		// normalize the distance, equvilent to local scaling
		if (modelNum > 20)
		{
			neighbor_distance = std::min(Distance2GeonNeighborBPWS(a, 20, affnity, neighborMap), Distance2GeonNeighborBPWS(b, 20, affnity, neighborMap));
		}
		else
			neighbor_distance = std::min(Distance2GeonNeighborBPWS(a, modelNum-1, affnity, neighborMap), Distance2GeonNeighborBPWS(b, modelNum - 1, affnity, neighborMap));

		double featureKernel = exp(-pow(distance/neighbor_distance, 2));
		double kernelValue;

		if (m_XYSizeKernelAffinity.IsEmpty() && m_HeightKernelAffinity.IsEmpty() && m_VolKernelAffinity.IsEmpty())
		{
			kernelValue = featureKernel;
		}
		else
		{
			double xySizeKernel = m_XYSizeKernelAffinity.GetEntryValue(a, b);
			double heightKernel = m_HeightKernelAffinity.GetEntryValue(a, b);
			double volKernel = m_VolKernelAffinity.GetEntryValue(a, b);

			//double kernelValue = m_dFeatureWeight*featureKernel+ m_dXYSizeWeight*xySizeKernel + m_dHeightWeight*heightKernel +m_dVolWeight*volKernel;
			kernelValue = pow(featureKernel, m_dFeatureWeight)*pow(xySizeKernel, m_dXYSizeWeight)*pow(heightKernel, m_dHeightWeight)*pow(volKernel, m_dVolWeight);
		}

		if (kernelValue > 1 - 1e-6)
		{
			kernelValue = 1;
		}

		if (kernelValue < 1e-6)
		{
			kernelValue = 0;
		}

		return kernelValue;
	}
}

//void CModelKernel::GetLFDAffinityBetweenTwoScenes( int si, int sj )
//{
//	CString distFileName = m_sSceneListPath + "\\GeoKernels\\LFD\\" + m_sSceneNameList[si] + "_" + m_sSceneNameList[sj] + "_dist_mat.LFDSim";
//	CDistMat<FTP> DM;
//
//	// Read back result
//	std::ifstream ifs(distFileName);
//	if (!ifs.good()) {
//		MSG_BOX_ERROR("LFD::calc: cannot open result file %s!", resultFN);
//		return -1;
//	}
//	std::string sFlag;
//
//	ifs >> sFlag;
//	if (sFlag != "N") {
//		MSG_BOX_ERROR("LFD::calc: error format! expected 'N'");
//		return -1;
//	}
//	unsigned int iNum, iLen;
//	ifs >> iNum;
//	if (iNum != MCOL.size()) {
//		MSG_BOX_ERROR("LFD::calc: scale of distance matrix is incorrect!");
//		return -1;
//	}
//	ifs >> sFlag;
//	if (sFlag != "M") {
//		MSG_BOX_ERROR("LFD::calc: error format! expected 'M'");
//		return -1;
//	}
//	ifs >> iLen;
//	if (iLen != iNum*(iNum-1)/2) {
//		MSG_BOX_ERROR("LFD::calc: size of distance matrix is incorrect!");
//		return -1;
//	}
//	DM.Resize(iNum);
//	for (unsigned int i=0; i<iLen; i++) {
//		ifs >> DM.m[i];
//	}
//
//	return DM;
//}




