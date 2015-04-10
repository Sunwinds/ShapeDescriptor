void ColorHistogram(double *HistogramValue, unsigned char *yuvBuff, int width, int height);
void CompactColor(unsigned char *CompactValue, double *HistogramValue);
void ExtractCCD(unsigned char *YuvBuff, unsigned __int64 *CCD, unsigned char *SrcBuff);
double ColorDistance(unsigned __int64 *dest, unsigned __int64 *src);
