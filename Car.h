
#ifndef _INCLUDED_CAR_H
#define _INCLUDED_CAR_H

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "Observer.h"

class CCarTree;
class Car;
class Helmet;
class JAM;

#pragma pack(2)
typedef struct
{
   short Magic;
   short id;
   int scaleBegin;
   int scaleEnd;
   int textureBegin;
   int pointsBegin;
   int vertexBegin;
   int textureEnd;
   int vertexEnd;
   int fileEnd;
   int fileEnd2;
   int always0;
   int always1;
   short unk;
   short size;
   short size8;
} t_objheader;
#pragma pack()


#pragma pack(2)
typedef struct
{
    short X;
	short Z;
	short Y;
	short ODirection;
	short OPitch;
	short ORoll;
	short Detail1;
	short Detail2;
	short Unk1;
	short Unk2;
} t_IntCamera;
#pragma pack()


class Camera: public Observer
{
public:
	Camera():
	Observer(t_Camera)
	{
		loaded = FALSE;
	}

	void Open(CCarTree *tree);

	void Select(CCarTree *tree);

	void updateLabel(CCarTree *tree);

	void parse();

	t_IntCamera pos;

	short Direction;
	short Pitch;
	short Roll;

	BOOL loaded;
	HTREEITEM node;
	int idx;

	double cx;
	double cy;
	double cz;
};

class DatVal: public Observer
{
public:
	DatVal():
	Observer(t_DataVal)
	{
	}

	void Open(CCarTree *tree);

	int data;
	HTREEITEM node;
	int idx;
};

class JamFile: public Observer
{
    public:
		JamFile():
		  Observer(t_JamFile)
	{
		filename = NULL;
	}

	void setFileName(char *str)
	{
	   	filename = new CString(str);
	}

	CString *getFileName()
	{
		return filename;
	}

	CString *filename;
	JAM     *JamFileStructure;
};


class TextureCmd : public Observer
{
public:
	TextureCmd():
	cmd(-1),
	Observer(t_TextureCmd)
	{
		numArgs = 0;
		numPts = 0;
	}

	int numl;
	int numh;
	int cmd;
	int Args[20];
	int numArgs;


	void parse();

	void Select(CCarTree *tree);


	HTREEITEM LoadTree(CCarTree *tree,HTREEITEM node);
	


	short PtsList[20];
	int numPts;
	HTREEITEM node;
	int idx;

	double cx;
	double cy;
	double cz;
} ;

typedef struct
{
	float r;
	float g;
	float b;
} t_Color;

static t_Color RandomColors[] = {
	{1.0f,0.0f,0.0f},
    {0.0f,1.0f,0.0f},
	{0.0f,0.0f,1.0f},
	{1.0f,1.0f,0.0f},
	{1.0f,0.0f,1.0f},
	{0.0f,1.0f,1.0f},
	{0.5f,0.0f,0.0f},
    {0.0f,0.5f,0.0f},
	{0.0f,0.0f,0.5f},
	{0.5f,0.5f,0.0f},
	{0.5f,0.0f,0.5f},
	{0.0f,0.5f,0.5f},
};

static int numRandomColors = sizeof(RandomColors)/sizeof(RandomColors[0]);

class Point3D : public Observer
{
    public:

	Point3D():x(0),y(0),z(0),Observer(t_Point)
	{
		desc = NULL;
	}

	Point3D(int x,int y,int z):
	x(x),y(y),z(z),Observer(t_Point)
	{
		desc = NULL;
	}

	~Point3D()
	{
	}

	void Open(CCarTree *tree);

	void Select(CCarTree *tree);
	
	int x;
	int y;
	int z;
	//int oz;
	int ox;
	int oy;
	int u1;

	void updateLabel(CCarTree *tree);

	void patch();

	double distance(Point3D *a)
	{
		return sqrt((double)(((a->x-x)*(a->x-x))+((a->y-y)*(a->y-y))+((a->z-z)*(a->z-z))));
	}

	HTREEITEM node;
	int idx;
	int offset;

	char *desc;

	void setDesc(char *msg)
	{
		desc = _strdup(msg);
	}

	char * getDesc()
	{
		return desc;
	}


};


class Vertex : public Observer
{
    public:

	Vertex():Observer(t_Vertex)
	{}

	Vertex(int f,int t):Observer(t_Vertex)
	{
	}

	void Open(CCarTree *tree);

	void Select(CCarTree *tree);

	void updateLabel(CCarTree *tree);

	void patch();

	double Length();
	double LengthCM();
	
	int x;
	int y;
	int z;
	int u1;
	int from;
	int to;
	int unused;
	HTREEITEM node;
	int idx;
	int offset;

	double cx;
	double cy;
	double cz;
} ;



class Scale	: public Observer
{
public:

	Scale():val(0),offset(0),Observer(t_Scale)
	{
		//ptsDep = 0;
	}

	Scale(int v,int offset):
	val(v),offset(offset),Observer(t_Scale)
	{
		//ptsDep = 0;
	}

	void Open(CCarTree *tree);

	void patch();

	
	

	int val;
	int offset;
	
	//int depend[255];
	//int ptsDep;
	HTREEITEM node;
	int idx;

	void updateLabel(CCarTree *tree);
	
} ;

#define OFFSET_NOSE 31
#define OFFSET_PTS  194

	
//#define DEF_CAR_LENGTH 54536
#define DEF_CAR_LENGTH 133952
//#define DEF_CAR_START 0x14C4A8
#define GP3_CAR_START 0x76293C
#define DEF_CAR_START GP3_CAR_START
#define DEF_HELMET_START 0x1599B0
#define DEF_HELMET_LENGTH 4294

class Object3D: public CObject
{
public:
	Object3D()	
	{
		pDoc = NULL;
		selectedPoint = NULL;
		selectedVertex = NULL;
		selectedTextureCmd = NULL;
		scaleData = NULL;
		vertexData = NULL;
		pointData = NULL;
		unkData = NULL;
		wheel = NULL;
		helmet = NULL;
		cockpit = NULL;
		swheel = NULL;
		numVerticies=0;
		numScale=0;
		numPoints=0;
		textureSize=0;
		numUnks=0;
	}

	virtual ~Object3D()
	{
		//if (scaleData) delete[] scaleData;
		//if (pointData) delete[] pointData;
		if (vertexData) delete[] vertexData;
		//if (textureData) delete[] textureData;

	}

	void clearSelections()
	{
		selectedPoint = NULL;
		selectedVertex = NULL;
		selectedTextureCmd = NULL;
	}

	int GetPCWord(int offset);
	void WritePCWord(int offset,int dat);
	int GetPCByte(int offset);
	long GetPCDWord(int offset);
	void WritePCDWord(int offset,int dat);

	Vertex* SelectVertex(HDC hdc,CScrollView *scview,double scrx,double scry,int idx);

	Scale *scaleData;
	Vertex *vertexData;
	Point3D *pointData;
	TextureCmd *textureData;
	DatVal  *unkData;

	int numVerticies;
	int numScale;
	int numPoints;
	int textureSize;
	int numUnks;

	Point3D origin;
	Point3D *selectedPoint;
	Vertex  *selectedVertex;
	TextureCmd *selectedTextureCmd;

	Car *wheel;
	Car *helmet;
	Car *cockpit;
	Car *swheel;

	BOOL valid;

	BOOL isValid()
	{
		return valid;
	}

	virtual void PatchData(){}
	virtual void updateData(BOOL updateTexture){}

	CDocument *pDoc;
	unsigned char data[DEF_CAR_LENGTH];
	t_objheader *header;
	HTREEITEM carNode;
};


class Car: public Object3D
{
   public:

	int memoryOffset;

	Car()
	{
		HiNose=FALSE;
		carNode=NULL;
		CarName=NULL;
		
	}


	virtual ~Car()
	{

	}

	

	char *CarName;

	char *getCarName()
	{
		return CarName;
	}

	void setCarName(char *msg)
	{
		CarName = _strdup(msg);
	}

	void parseCar(int begin,int offset,int dver);
	void exportCar(int begin,int offset);
	void write(CString filename);

	Camera cameraData[9];
	JamFile jamData[8];
	JamFile jamCarData[20];

	
	BOOL HiNose;

	

	void LoadTree(CCarTree *tree,CDocument *pDoc);
	void PatchData();
	void updateData(BOOL updateTexture);
	

	Camera   *selectedCamera;
	
};

Car *LoadCar(LPCSTR filename);
Helmet *LoadHelmet(LPCSTR filename);
Car *ImportCar(CString exepath);
void ExportHelmet(CString exePath,Helmet *myhelmet);
Helmet *ImportHelmet(CString exepath);
void ExportCar(CString exepath,Car *mycar);
void ImportCameras(Car *mycar,CString exePath);
void ExportCameras(CString exePath,Car *mycar);
void LoadTextureCfg();
void LoadHTextureCfg();
BOOL isTextureCfgPolygon(int idx);
BOOL isTextureCfgDefined(int idx);





class Helmet: public Object3D
{
   public:

	int memoryOffset;

	Helmet()
	{
	}

	virtual ~Helmet()
	{
	}

	void parse(int begin);
	void write(CString filename);

	void LoadTree(CCarTree *tree);
	void PatchData();
	void updateData(BOOL updateTexture);
	
};

class GPOObject: public Object3D
{
   public:

	int memoryOffset;

	GPOObject()
	{
	}

	virtual ~GPOObject()
	{
	}

	void parse(int begin);
	//void write(CString filename);

	void LoadTree(CCarTree *tree);
	//void PatchData();
	void updateData(BOOL updateTexture);
	
};




#endif