#pragma once
#include "../Model.h"
#include "AffinityMatrix.h"


#include "../Clustering/SelfTuning/SpectralClustering.h"
#include "../Clustering/NCut/ncutLib/include/ncuts/ncutW.h"

class CSceneList;

class CModelKernel
{
public:
	CModelKernel(void);
	~CModelKernel(void);

	CModelKernel(CSceneList *pSceneList);

	//// Affinity
	void ComputeLightFieldAffinity();  // compute LFD between all models
	void ComputeXYSizeAffinity();
	void ComputeHeightAffinity();
	void ComputeVolAffinity();
	void ComputeGeoKernelAffinity(FTP featureWeight, FTP sizeWeight, FTP heightWeight, FTP volWeight);
	void ComputeGeoTagKernelAffinity(FTP tagWeight);
	void ComputeModelAffinity(std::vector<FTP> GeoParas, FTP tagWeight, CString kernelType);
	void OpenAffinities();

	CAffinityMatrix& GetGeoKernelAffinity() {return m_GeoKernelAffinity;};
	CAffinityMatrix& GetGeoTagKernelAffinity() {return m_GeoTagKernelAffinity;};

	double Distance2GeoNeighbor(int a , int n_neighbor); 	// Neighbor map, used for normalizing geometry feature distance
	double GetNodeGeoKernel(int a, int b);

	// Clustering
	std::vector<std::vector<int>> DoClustering(CAffinityMatrix &m, int numDim, CString clusterMethod);
	std::vector<std::vector<int>> DoSelfTuningClustering(CAffinityMatrix &m, int numDim);
	std::vector<std::vector<int>> DoNCutClustering(CAffinityMatrix &m, int numDim);
	void SaveModelClusters(CString filename);

	int ToGlobalModelIdx(int sceneIdx, int modelIdx);  	// Converting idx
	void NormalizeKernelTable(std::vector<std::vector<FTP>> &kernelTable);  // scale max value in the diagonal to 1

	std::vector<std::vector<int>> GetModelClusters() {return m_ModelClusters;};
	int GetModelNum() { return m_iNum; };
	std::vector<std::pair<int, int>> GetModelIDMap() { return m_IdMap;};

	void ComputeLightFieldDistBPWS(); // compute LFD distance between pair-wise scenes
	void ComputeLFDDistBetweenTwoScenes(int si, int sj);
	void ComputeLFDAffinityBetweenTwoScenes(int si, int sj);
	CDistMat<FTP> GetGeoAffinityBetweenTwoScenes(int si, int sj);
	double Distance2GeonNeighborBPWS(int a, int n_neighbor, CAffinityMatrix &affnity, std::vector<std::vector<int>> &neighborMap);
	double GetNodeGeoKernelBPWS(int a, int b, CAffinityMatrix &affnity, std::vector<std::vector<int>> &neighborMap);

private:
	std::vector<std::vector<CModel*>> m_ModelList;
	int m_iNum;
	std::vector<std::pair<int, int>> m_IdMap;   // idx in the vector corresponds to model (i, j)

	CString m_sSceneListPath;
	CString m_sSceneListName;
	std::vector<CString> m_sSceneNameList;

	std::vector<std::vector<int>> m_FeatureDistNeighborMap;
	std::vector<std::vector<FTP>> m_XYSizeFeature;
	std::vector<FTP> m_HeightFeature;
	std::vector<FTP> m_VolFeature;

	// Affinity Matrix
	CAffinityMatrix m_FeatureDistAffinity;
	CAffinityMatrix m_XYSizeKernelAffinity;
	CAffinityMatrix m_HeightKernelAffinity;
	CAffinityMatrix m_VolKernelAffinity;
	CAffinityMatrix m_GeoKernelAffinity;

	CAffinityMatrix m_GeoTagKernelAffinity;

	// Clusters
	std::vector<std::vector<int>> m_ModelClusters;

	//Parameters
	FTP m_dFeatureWeight;
	FTP m_dXYSizeWeight;
	FTP m_dHeightWeight;
	FTP m_dVolWeight;
};
