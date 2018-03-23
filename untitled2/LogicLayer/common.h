#pragma once

#include <string>
#include <iostream>

#define SW_MAXFACECOUNT			1		//单张图像最大支持检测人脸数
#define SW_RANKNUM				10		//返回识别排名前几位的信息
#define SW_MAXIDLEN				100		//最大人员ID长度
#define SW_MAXNAMELEN			100		//最大人员姓名长度
#define SW_MAXLANDMARKNUM		5		//最大landmark数
#define SW_MAXVERLENGTH			30		//版本号信息长度
#define SW_MAXDEVICESCOUNT		50      //支持最大设备数量

enum E_Gender
{
	Invaild = 0,
	Male,
	Female,
};

// 图像数据结构
struct swImageData
{
	int width;              //图像宽度
	int height;             //图像高度
	int channel;            //通道数
	unsigned char* data;	//图像数据
	swImageData() : width(0), height(0), channel(0), data(0){}
	~swImageData()
	{
		if (data)
		{
			delete[] data;
			data = 0;
		}
	}
	swImageData(unsigned char* indata, int inwidth, int inheight, int inchannel)
	{
		width = inwidth;
		height = inheight;
		channel = inchannel;
		data = new unsigned char[width*height*channel];
		memcpy(data, indata, width*height*channel*sizeof(unsigned char));
	}
	swImageData& operator =(const swImageData&other)
	{
		width = other.width;
		height = other.height;
		channel = other.channel;
		if (data)
		{
			delete[] data;
			data = NULL;
		}
		data = new unsigned char[width*height*channel];
		memcpy(data, other.data, width*height*channel*sizeof(unsigned char));
		return*this;
	}
	int GetLength()
	{
		int len = sizeof(*this) - sizeof(unsigned char*) + width*height*channel*sizeof(unsigned char);
		return len;
	}
	int Pack(char *buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(p, &width, sizeof(width));
		pos += sizeof(width);
		memcpy(p + pos, &height, sizeof(height));
		pos += sizeof(height);
		memcpy(p + pos, &channel, sizeof(channel));
		pos += sizeof(channel);
		if (width != 0 && height != 0 && channel != 0)
		{
			memcpy(p + pos, data, sizeof(unsigned char)*width*height*channel);
			pos += sizeof(char)*width*height*channel;

		}

		return pos;
	}
	int UnPack(char* buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(&width, p, sizeof(width));
		pos += sizeof(width);
		memcpy(&height, p + pos, sizeof(height));
		pos += sizeof(height);
		memcpy(&channel, p + pos, sizeof(channel));
		pos += sizeof(channel);
		if (width != 0 && height != 0 && channel != 0)
		{
			if (data)
			{
				delete[] data;
				data = NULL;
			}
			data = new unsigned char[width*height*channel];
			memcpy(data, p + pos, width*height*channel*sizeof(unsigned char));

			pos += width*height*channel*sizeof(unsigned char);

		}

		return pos;
	}
};
// 矩形区域结构
struct swRect
{
	int x;
	int y;
	int width;
	int height;

	swRect() :x(0), y(0), width(0), height(0){}
	swRect(int inx, int iny, int inwidth, int inheight)
	{
		x = inx;
		y = iny;
		width = inwidth;
		height = inheight;
	}
	swRect& operator =(const swRect&other)
	{
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
		return*this;
	}
	int GetLength()
	{
		int len = sizeof(*this);
		return len;
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 点数据结构
struct swPoint
{
	int x;//坐标X
	int y;//坐标Y

	swPoint() :x(0), y(0){}
	swPoint(int inx, int iny)
	{
		x = inx;
		y = iny;
	}
	swPoint& operator =(const swPoint& other)
	{
		x = other.x;
		y = other.y;
		return*this;
	}
	int Pack(char* buf)
	{
		memcpy(buf, this, sizeof(*this));

		return sizeof(*this);
	}
	int UnPack(char* buf)
	{
		memcpy(this, buf, sizeof(*this));

		return sizeof(*this);
	}
};
// 点数据结构
struct swPoint2f
{
	float x;//坐标X
	float y;//坐标Y

	swPoint2f() :x(0), y(0){}
	swPoint2f(float inx, float iny)
	{
		x = inx;
		y = iny;
	}
	swPoint2f& operator =(const swPoint2f& other)
	{
		x = other.x;
		y = other.y;
		return*this;
	}
	int Pack(char* buf)
	{
		memcpy(buf, this, sizeof(*this));

		return sizeof(*this);
	}
	int UnPack(char* buf)
	{
		memcpy(this, buf, sizeof(*this));

		return sizeof(*this);
	}
};
// 点数据结构
struct swPoint3D
{
	float x;//坐标X
	float y;//坐标Y
	float z;

	swPoint3D() :x(0), y(0), z(0){}
	swPoint3D(float inx, float iny, float inz)
	{
		x = inx;
		y = iny;
		z = inz;
	}
	swPoint3D& operator =(const swPoint3D& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return*this;
	}
};
// 2d特征点坐标结构
struct swLandmark2D
{
	swPoint point[SW_MAXLANDMARKNUM];
	swLandmark2D(){};
	swLandmark2D(swPoint pts[SW_MAXLANDMARKNUM])
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			point[i] = pts[i];
		}
	}
	swLandmark2D& operator =(const swLandmark2D&other)
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			point[i] = other.point[i];
		}
		return*this;
	}
	int GetLength()
	{
		int len = sizeof(*this);
		return len;
	}

	int Pack(char* buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char* buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 3d特征点位置结构
struct swLandmark3D
{
	swPoint3D point[SW_MAXLANDMARKNUM];
	swLandmark3D(){};
	swLandmark3D(swPoint3D pts[SW_MAXLANDMARKNUM])
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			point[i] = pts[i];
		}
	}
	swLandmark3D& operator =(const swLandmark3D&other)
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			point[i] = other.point[i];
		}
		return*this;
	}
	int GetLength()
	{
		int len = sizeof(*this);
		return len;
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 3d特征点索引结构
struct swLandmarkIndex3D
{
	int Index3D[SW_MAXLANDMARKNUM];
	swLandmarkIndex3D()
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			Index3D[i] = -1;
		}
	}
	swLandmarkIndex3D& operator =(const swLandmarkIndex3D&other)
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			Index3D[i] = other.Index3D[i];
		}
		return *this;
	}
	int GetLength()
	{
		int len = sizeof(*this);
		return len;
	}
	bool IsVaild()
	{
		for (int i = 0; i < SW_MAXLANDMARKNUM; i++)
		{
			if (Index3D[i] <= 0)
			{
				return false;
			}
		}
		return true;
	}
	int Pack(char* buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char* buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 注册信息结构
struct swPersonInfo
{
	char  sPersonID[SW_MAXIDLEN];	//ID
	char  sPersonName[SW_MAXNAMELEN];// 姓名
	int	  nGender;					// 性别
	swPersonInfo()
	{
		memset(sPersonID, 0, sizeof(char)*SW_MAXIDLEN);
		memset(sPersonName, 0, sizeof(char)*SW_MAXNAMELEN);
		nGender = E_Gender::Invaild;
	}
	swPersonInfo(char sPersonID_[SW_MAXIDLEN], char sPersonName_[SW_MAXNAMELEN], int nGender_)
	{
		memcpy(sPersonID, sPersonID_, SW_MAXIDLEN);
		memcpy(sPersonName, sPersonName_, SW_MAXNAMELEN);
		nGender = nGender_;
	}
	swPersonInfo& operator =(const swPersonInfo&other)
	{
		memcpy(sPersonID, other.sPersonID, sizeof(char)*SW_MAXIDLEN);
		memcpy(sPersonName, other.sPersonName, sizeof(char)*SW_MAXNAMELEN);
		nGender = other.nGender;
		return*this;
	}
	int GetLength()
	{
		int len = sizeof(*this);
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 匹配信息结构
struct swMatchInfo
{
	swRect rect;	// 人脸区域
	swPersonInfo personinfo;// 识别信息
	char face3dfile[256];// wrl 文件
	char face2dfile[256];// 2d face文件
	swMatchInfo()
	{
		memset(face3dfile, 0, sizeof(face3dfile));
		memset(face2dfile, 0, sizeof(face2dfile));
	}
	swMatchInfo& operator =(const swMatchInfo&other)
	{
		rect = other.rect;
		personinfo = other.personinfo;
		memcpy(face3dfile, other.face3dfile, sizeof(face3dfile));
		memcpy(face2dfile, other.face2dfile, sizeof(face2dfile));

		return*this;
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};
// 点云结构
struct swPointsCloud
{
	int dim;//每点的坐标维度，默认为3，即3维点云x、y、z
	int nCount;// 总点数
	float* data;//点云数据(data[0]-data[dim-1] 为第一点的坐标值，data[dim]-data[dim*2-1] 为第二点的值，以此类推)
	swPointsCloud() :data(0), dim(3), nCount(0){}
	~swPointsCloud()
	{
		if (data)
		{
			delete[] data;
			data = NULL;
		}
	}
	swPointsCloud(float* data_, int dim_, int count_)
	{
		dim = dim_;
		nCount = count_;
		data = new float[dim*nCount];
		memcpy(data, data_, dim*nCount*sizeof(float));
	}
	swPointsCloud& operator =(const swPointsCloud&other)
	{
		dim = other.dim;
		nCount = other.nCount;
		if (data)
			delete data;
		data = new float[dim*nCount];
		memcpy(data, other.data, dim*nCount*sizeof(float));
		return *this;
	}
	int GetLength()
	{
		int len = sizeof(*this) - sizeof(float*) + dim * nCount * sizeof(float*);
		return len;
	}
	int Pack(char *buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(p + pos, &dim, sizeof(dim));
		pos += sizeof(dim);
		memcpy(p + pos, &nCount, sizeof(nCount));
		pos += sizeof(nCount);

		if (dim != 0 && nCount != 0)
		{
			memcpy(p + pos, data, sizeof(float)*dim*nCount);
			pos += sizeof(float)*dim*nCount;
		}

		return pos;
	}
	int UnPack(char* buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(&dim, p, sizeof(dim));
		pos += sizeof(dim);
		memcpy(&nCount, p + pos, sizeof(nCount));
		pos += sizeof(nCount);
		if (dim != 0 && nCount != 0)
		{
			if (data)
			{
				delete data;
			}
			data = new float[dim*nCount];
			memcpy(data, p + pos, sizeof(float)*dim*nCount);

			pos += sizeof(float)*dim*nCount;

		}

		return pos;
	}
};
// 三角面片结构
struct swTriangle
{
	int dim;
	int nCount;// 三角面片个数
	int* data;// 三角面片数据
	swTriangle() :dim(3), nCount(0), data(0){}
	~swTriangle()
	{
		if (data)
			delete[] data;
		data = NULL;
	}
	swTriangle(int* data_, int count_, int dim_)
	{
		dim = dim_;
		nCount = count_;
		data = new int[dim*nCount];
		memcpy(data, data_, dim*nCount*sizeof(int));
	}
	swTriangle& operator =(const swTriangle&other)
	{
		dim = other.dim;
		nCount = other.nCount;
		if (data)
			delete data;
		data = new int[dim*nCount];
		memcpy(data, other.data, dim*nCount*sizeof(int));
		return *this;
	}
	int GetLength()
	{
		int len = sizeof(*this) - sizeof(data) + dim * nCount * sizeof(int);
		return len;
	}

	int Pack(char *buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(p + pos, &dim, sizeof(dim));
		pos += sizeof(dim);
		memcpy(p + pos, &nCount, sizeof(nCount));
		pos += sizeof(nCount);
		if (dim != 0 && nCount != 0)
		{
			memcpy(p + pos, data, sizeof(int) * dim * nCount);
			pos += sizeof(int) * dim * nCount;

		}

		return pos;
	}
	int UnPack(char* buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(&dim, p + pos, sizeof(dim));
		pos += sizeof(dim);
		memcpy(&nCount, p + pos, sizeof(nCount));
		pos += sizeof(nCount);

		if (dim != 0 && nCount != 0)
		{
			if (data)
			{
				delete data;
			}
			data = new int[dim * nCount];
			memcpy(data, p + pos, sizeof(int)*dim*nCount);

			pos += sizeof(int)*dim*nCount;

		}

		return pos;
	}
};
// 纹理坐标结构
struct swTexCord
{
	//纹理图片的uv（0到1之间的值）
	int dim; // 2维
	int nCount;
	float *data;
	swTexCord() :nCount(0), data(0), dim(0){};
	~swTexCord()
	{
		if (data)
			delete[] data;
		data = 0;
	}
	int GetLength()
	{
		int len = sizeof(*this) - sizeof(data) + nCount*sizeof(float)*dim;
		return len;
	}
	int Pack(char *buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(p + pos, &dim, sizeof(dim));
		pos += sizeof(dim);
		memcpy(p + pos, &nCount, sizeof(nCount));
		pos += sizeof(nCount);
		if (dim != 0 && nCount != 0)
		{
			memcpy(p + pos, data, sizeof(float) * 2 * nCount);
			pos += sizeof(float) * 2 * nCount;

		}

		return pos;
	}
	int UnPack(char* buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(&dim, p + pos, sizeof(dim));
		pos += sizeof(dim);
		memcpy(&nCount, p + pos, sizeof(nCount));
		pos += sizeof(nCount);
		if (dim != 0 && nCount != 0)
		{
			if (data)
			{
				delete data;
			}
			data = new float[2 * nCount];
			memcpy(data, p + pos, sizeof(float) * 2 * nCount);

			pos += sizeof(float) * 2 * nCount;

		}

		return pos;
	}

};
// 纹理点的RGB
struct swTexRGB
{
	int width;
	int height;
	unsigned char *rgb;

	swTexRGB() :width(0), rgb(0), height(0){};
	~swTexRGB()
	{
		if (rgb)
			delete[] rgb;
		rgb = 0;
	}
	int GetLength()
	{
		int len = sizeof(*this) - sizeof(rgb) + 3 * width*height*sizeof(unsigned char);
		return len;
	}
	int Pack(char *buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(p, &width, sizeof(width));
		pos += sizeof(width);
		memcpy(p + pos, &height, sizeof(height));
		pos += sizeof(height);
		if (width != 0 && height != 0)
		{
			memcpy(p + pos, rgb, sizeof(unsigned char) * 3 * width * height);
			pos += sizeof(unsigned char) * 3 * width * height;
		}
		return pos;
	}
	int UnPack(char* buf)
	{
		int pos = 0;
		char *p = buf;
		memcpy(&width, p, sizeof(width));
		pos += sizeof(width);
		memcpy(&height, p + pos, sizeof(height));
		pos += sizeof(height);
		if (width != 0 && height != 0)
		{
			if (rgb)
			{
				delete rgb;
			}
			rgb = new unsigned char[3 * width * height];
			memcpy(rgb, p + pos, sizeof(unsigned char) * 3 * width * height);

			pos += sizeof(unsigned char) * 3 * width * height;
		}

		return pos;
	}
};

// 3维注册数据结构
struct swRegist3DModelInfo
{
	swLandmarkIndex3D landmarkIndex3D;
	swPointsCloud pts;
	swTriangle tris;
	// 以下信息为注册时需要的信息，识别时不需要赋值
	swTexCord texCord;
	swTriangle texTri;
	swTexRGB	texRgb;
	swRegist3DModelInfo(){}
	swRegist3DModelInfo(swLandmarkIndex3D landmark3D_, swPointsCloud pts_, swTriangle tris_,
		swTexCord texCord_, swTriangle texTri_, swTexRGB	texRgb_)
	{
		landmarkIndex3D = landmark3D_;
		pts = pts_;
		tris = tris_;
		texCord = texCord_;
		texTri = texTri_;
		texRgb = texRgb_;
	}
	swRegist3DModelInfo& operator =(const swRegist3DModelInfo&other)
	{
		landmarkIndex3D = other.landmarkIndex3D;
		pts = other.pts;
		tris = other.tris;
		texCord = other.texCord;
		texTri = other.texTri;
		texRgb = other.texRgb;
		return *this;
	}
	bool IsValid()
	{
		for (int i = 0; i < 5; i++)
		{
			if (landmarkIndex3D.Index3D[i] == -1)
			{
				return false;
			}
		}
		return true;
	}
	int GetLength()
	{
		int len = landmarkIndex3D.GetLength() + pts.GetLength() + tris.GetLength() + texCord.GetLength() + texTri.GetLength() + texRgb.GetLength();
		return len;
	}
	int Pack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += landmarkIndex3D.Pack(p);
		pos += pts.Pack(p + pos);
		pos += tris.Pack(p + pos);
		pos += texCord.Pack(p + pos);
		pos += texTri.Pack(p + pos);
		pos += texRgb.Pack(p + pos);
		return pos;
	}
	int UnPack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += landmarkIndex3D.UnPack(p);
		pos += pts.UnPack(p + pos);
		pos += tris.UnPack(p + pos);
		pos += texCord.UnPack(p + pos);
		pos += texTri.UnPack(p + pos);
		pos += texRgb.UnPack(p + pos);
		return pos;

	}
};
// 3维识别数据结构
struct swRecong3DModelInfo
{
	swLandmarkIndex3D landmarkIndex3D;
	swPointsCloud pts;
	swTriangle tris;
	swRecong3DModelInfo(){}
	swRecong3DModelInfo(swLandmarkIndex3D landmark3D_, swPointsCloud pts_, swTriangle tris_)
	{
		landmarkIndex3D = landmark3D_;
		pts = pts_;
		tris = tris_;
	}
	swRecong3DModelInfo& operator =(const swRecong3DModelInfo&other)
	{
		landmarkIndex3D = other.landmarkIndex3D;
		pts = other.pts;
		tris = other.tris;
		return *this;
	}
	int GetLength()
	{
		int len = landmarkIndex3D.GetLength() + pts.GetLength() + tris.GetLength();
		return len;
	}
	int Pack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += landmarkIndex3D.Pack(buf);
		pos += pts.Pack(p + pos);
		pos += tris.Pack(p + pos);

		return pos;
	}
	int UnPack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += landmarkIndex3D.UnPack(p);
		pos += pts.UnPack(p + pos);
		pos += tris.UnPack(p + pos);
		return pos;

	}
};
// 2维数据结构
struct sw2DModelInfo
{
	swImageData imagedata;
	sw2DModelInfo(){}
	sw2DModelInfo(swImageData imagedata_)
	{
		imagedata = imagedata_;
	}
	sw2DModelInfo& operator =(const sw2DModelInfo &other)
	{
		imagedata = other.imagedata;
	}
	int GetLength()
	{
		int len = imagedata.GetLength();
		return len;
	}
	bool IsValid()
	{
		if (imagedata.data == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	int Pack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += imagedata.Pack(p + pos);

		return pos;
	}
	int UnPack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += imagedata.UnPack(p + pos);
		return pos;
	}

};

// 2/3维注册数据结构
struct swRegistMulModelInfo
{
	sw2DModelInfo info2d;
	swRegist3DModelInfo info3d;
	swRegistMulModelInfo(){}
	swRegistMulModelInfo(sw2DModelInfo info2d_, swRegist3DModelInfo info3d_)
	{
		info2d = info2d_;
		info3d = info3d_;
	}
	swRegistMulModelInfo& operator =(const swRegistMulModelInfo &other)
	{
		info2d = other.info2d;
		info3d = other.info3d;
	}
	int GetLength()
	{
		int len = info2d.GetLength() + info3d.GetLength();
		return len;
	}
	int Pack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += info2d.Pack(p);
		pos += info3d.Pack(p + pos);

		return pos;
	}
	int UnPack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += info2d.UnPack(p);
		pos += info3d.UnPack(p + pos);
		return pos;
	}
};

// 2/3维识别数据结构
struct swRecongMulModelInfo
{
	sw2DModelInfo info2d;
	swRecong3DModelInfo info3d;
	swRecongMulModelInfo(){}
	swRecongMulModelInfo(sw2DModelInfo info2d_, swRecong3DModelInfo info3d_)
	{
		info2d = info2d_;
		info3d = info3d_;
	}
	swRecongMulModelInfo& operator =(const swRecongMulModelInfo &other)
	{
		info2d = other.info2d;
		info3d = other.info3d;
	}
	int GetLength()
	{
		int len = info2d.GetLength() + info3d.GetLength();
		return len;
	}
	int Pack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += info2d.Pack(p);
		pos += info3d.Pack(p + pos);

		return pos;
	}
	int UnPack(char *buf)
	{
		char *p = buf;
		int pos = 0;
		pos += info2d.UnPack(p);
		pos += info3d.UnPack(p + pos);
		return pos;
	}
};

// 注册返回结果
struct swRegistResult
{
	int recode;
	swRegistResult & operator = (const swRegistResult& other)
	{
		recode = other.recode;
		return *this;
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};

// 识别结果数据结构
struct swMatchResult
{
	int nValid;									// 有效个数据个数
	swMatchInfo  MatchInfo[SW_MAXFACECOUNT];	//识别信息
	swMatchResult() :nValid(0){	}
	swMatchResult& operator =(const swMatchResult&other)
	{
		for (int i = 0; i < SW_MAXFACECOUNT; i++)
			MatchInfo[i] = other.MatchInfo[i];
		nValid = other.nValid;

		return*this;
	}

	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};

// 运动检测返回结果
struct swMotionResult
{
	int recode;
	swRect rect;
	swMotionResult & operator = (const swMotionResult& other)
	{
		recode = other.recode;
		rect = other.rect;
		return *this;
	}
	int Pack(char *buf)
	{
		memcpy(buf, this, sizeof(*this));
		return sizeof(*this);
	}
	int UnPack(char *buf)
	{
		memcpy(this, buf, sizeof(*this));
		return sizeof(*this);
	}
};


// 通信协议结构
/*		
命令编号	含义					数据结构
	
1		注册			swRegistMulModelInfo  _swPersonInfo
2		识别			swRecongMulModelInfo
3		注册结果		swRegistResult
4		识别结果		swMatchResult

5		人脸检测		swImageData
6		人脸检测结果	swRect

7		运动检测		swImageData
8		运动检测结果	swMotionResult
100		停止程序		无
*/
enum
{
	CMD_REGIST = 1,
	CMD_RECONG,
	CMD_REGISTRESULT,
	CMD_RECONGRESULT,

	CMD_FACEDET,
	CMD_FACEDETRESULT,

	CMD_MOTIONDET,
	CMD_MOTIONDETRESULT,

	CMD_STOPPROCESS = 100,
};
struct swCommand
{

	unsigned short cmd;
	unsigned short cmdCount;
	unsigned short nUnPack;
	char* data1;
	char* data2;
	char* data3;
	swCommand() :cmd(0), cmdCount(0), data1(0), data2(0), data3(0), nUnPack(0){}
	~swCommand()
	{
		switch (nUnPack){
		case CMD_REGIST:{
			if (data1 != NULL){
				delete (swRegistMulModelInfo*)data1;data1 = NULL;
			}
			if (data2 != NULL){
				delete (swPersonInfo*)data2;data2 = NULL;
			}
		}break;
		case CMD_RECONG:{
			if (data1 != NULL){
				delete (swRecongMulModelInfo*)data1;data1 = NULL;
			}
		}break;
		case CMD_REGISTRESULT:{
			if (data1 != NULL){
				delete (swRegistResult*)data1; data1 = NULL;
			}
		}break;
		case CMD_RECONGRESULT:{
			if (data1 != NULL){
				delete (swMatchResult*)data1; data1 = NULL;
			}
		}break;
		case CMD_FACEDET:{
			if (data1 != NULL){
				delete (swImageData*)data1; data1 = NULL;
			}
		}break;
		case CMD_FACEDETRESULT:{
			if (data1 != NULL){
				delete (swRect*)data1; data1 = NULL;
			}
		}break;
		case CMD_MOTIONDET:{
			if (data1 != NULL){
				delete (swImageData*)data1; data1 = NULL;
			}
		}break;
		case CMD_MOTIONDETRESULT : {
			if (data1 != NULL){
				delete (swMotionResult*)data1; data1 = NULL;
			}
		}break;
		default:
		{
			break;
		}
			nUnPack = 0;
		}
	}
	int GetLength()
	{
		int len = sizeof(*this);
		return len;
	}
	int Pack(char* buf)
	{
		int pos = 0;
		memcpy(buf + pos, &cmd, sizeof(cmd));
		pos += sizeof(cmd);
		memcpy(buf + pos, &cmdCount, sizeof(cmdCount));
		pos += sizeof(cmdCount);
		memcpy(buf + pos, &nUnPack, sizeof(nUnPack));
		pos += sizeof(nUnPack);

		switch (cmd)
		{
		case CMD_REGIST:
		{
			swRegistMulModelInfo* p1 = (swRegistMulModelInfo*)data1;
			p1->Pack(buf + pos);
			pos += p1->GetLength();
			swPersonInfo *p2 = (swPersonInfo*)data2;
			p2->Pack(buf + pos);

		}break;
		case CMD_RECONG:
		{
			swRecongMulModelInfo* p1 = (swRecongMulModelInfo*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_REGISTRESULT:
		{
			swRegistResult *p1 = (swRegistResult*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_RECONGRESULT:
		{
			swMatchResult *p1 = (swMatchResult*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_FACEDET:
		{
			swImageData * p1 = (swImageData*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_FACEDETRESULT:
		{
			swRect * p1 = (swRect*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_MOTIONDET:
		{
			swImageData *p1 = (swImageData*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_MOTIONDETRESULT:
		{
			swMotionResult *p1 = (swMotionResult*)data1;
			p1->Pack(buf + pos);
		}break;
		case CMD_STOPPROCESS:
		{

		}break;
		default:
			break;
		}
		return 0;
	}
	int UnPack(char *buf)
	{
		int pos = 0;
		memcpy(&cmd, buf, sizeof(cmd));
		pos += sizeof(cmd);
		memcpy(&cmdCount, buf + pos, sizeof(cmdCount));
		pos += sizeof(cmdCount);
		memcpy(&nUnPack, buf + pos, sizeof(nUnPack));
		pos += sizeof(nUnPack);
		
		switch (cmd)
		{
		case CMD_REGIST:
		{
			data1 = (char*) new swRegistMulModelInfo;
			((swRegistMulModelInfo*)data1)->UnPack(buf + pos);
			pos += ((swRegistMulModelInfo*)data1)->GetLength();

			data2 = (char*) new swPersonInfo;
			((swPersonInfo*)data2)->UnPack(buf + pos);
			nUnPack = CMD_REGIST;

		}break;
		case CMD_RECONG:
		{
			data1 = (char*) new swRecongMulModelInfo;
			((swRecongMulModelInfo*)data1)->UnPack(buf + pos);
			nUnPack = CMD_RECONG;
		}break;
		case CMD_REGISTRESULT:
		{
			data1 = (char*) new swRegistResult;
			((swRegistResult*)data1)->UnPack(buf + pos);
			nUnPack = CMD_REGISTRESULT;
		}break;
		case CMD_RECONGRESULT:
		{
			data1 = (char*) new swMatchResult;
			((swMatchResult*)data1)->UnPack(buf + pos);
			nUnPack = CMD_RECONGRESULT;
		}break;
		case CMD_FACEDET:
		{
			data1 = (char*) new swImageData;
			((swImageData*)data1)->UnPack(buf + pos);
			nUnPack = CMD_FACEDET;
		}break;
		case CMD_FACEDETRESULT:
		{
			data1 = (char*) new swRect;
			((swRect*)data1)->UnPack(buf + pos);
			nUnPack = CMD_FACEDETRESULT;
		}break;
		case CMD_MOTIONDET:
		{
			data1 = (char*) new swImageData;
			((swImageData*)data1)->UnPack(buf + pos);
			nUnPack = CMD_MOTIONDET;
		}break;
		case CMD_MOTIONDETRESULT:
		{
			data1 = (char*) new swMotionResult;
			((swMotionResult*)data1)->UnPack(buf + pos);
			nUnPack = CMD_MOTIONDETRESULT;
		}break;
		case CMD_STOPPROCESS:
		{

		}break;
		default:
			break;
		}
		

		
		return 0;
	}
};




