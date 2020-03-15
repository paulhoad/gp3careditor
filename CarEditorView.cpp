// CarEditorView.cpp : implementation of the CCarEditorView class
//

#include "stdafx.h"
#include "CarEditor.h"

#include "CarEditorDoc.h"
#include "CarEditorView.h"

#include "JAM.h"
#include "JamSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <gl\gl.h>
#include <gl\glu.h>

#include "Texture.h"



#define WR_PROFILE(x) pApp->WriteProfileInt(strSection, #x,x);
#define WR_PROFILES(x) pApp->WriteProfileString(strSection, #x,x);
#define PROFILE(x,b) x =  pApp->GetProfileInt(strSection, #x,b) == 1 ? TRUE : FALSE;
#define PROFILES(x,b) x =  pApp->GetProfileString(strSection, #x,b);

/////////////////////////////////////////////////////////////////////////////
// CCarEditorView

IMPLEMENT_DYNCREATE(CCarEditorView, COpenGLView)

BEGIN_MESSAGE_MAP(CCarEditorView, COpenGLView)
	//{{AFX_MSG_MAP(CCarEditorView)
	ON_COMMAND(ID_FILLOBJECTS, OnFillobjects)
	ON_UPDATE_COMMAND_UI(ID_FILLOBJECTS, OnUpdateFillobjects)
	ON_COMMAND(ID_SHOW_POINTS, OnShowPoints)
	ON_UPDATE_COMMAND_UI(ID_SHOW_POINTS, OnUpdateShowPoints)
	ON_COMMAND(ID_SHOWTYRES, OnShowtyres)
	ON_UPDATE_COMMAND_UI(ID_SHOWTYRES, OnUpdateShowtyres)
	ON_COMMAND(ID_HELMET, OnHelmet)
	ON_COMMAND(ID_DRAWAXIS, OnDrawaxis)
	ON_UPDATE_COMMAND_UI(ID_DRAWAXIS, OnUpdateDrawaxis)
	ON_COMMAND(ID_SHOWCAMERAS, OnShowcameras)
	ON_UPDATE_COMMAND_UI(ID_SHOWCAMERAS, OnUpdateShowcameras)
	ON_COMMAND(ID_TEXTUREMAPS, OnTexturemaps)
	ON_UPDATE_COMMAND_UI(ID_TEXTUREMAPS, OnUpdateTexturemaps)
	ON_UPDATE_COMMAND_UI(ID_HELMET, OnUpdateHelmet)
	ON_COMMAND(ID_SHOW_SHADOW, OnShowShadow)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHADOW, OnUpdateShowShadow)
	ON_COMMAND(ID_SHOW_LINES, OnShowLines)
	ON_UPDATE_COMMAND_UI(ID_SHOW_LINES, OnUpdateShowLines)
	ON_COMMAND(ID_SHOW_CULLING, OnShowCulling)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CULLING, OnUpdateShowCulling)
	ON_COMMAND(ID_SHOW_NORMALS, OnShowNormals)
	ON_UPDATE_COMMAND_UI(ID_SHOW_NORMALS, OnUpdateShowNormals)
	ON_COMMAND(ID_SHOW_SINGLE_COLOR, OnShowSingleColor)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SINGLE_COLOR, OnUpdateShowSingleColor)
	ON_COMMAND(ID_LIGHTING, OnLighting)
	ON_UPDATE_COMMAND_UI(ID_LIGHTING, OnUpdateLighting)
	ON_COMMAND(ID_SHOW_CAR, OnShowCar)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CAR, OnUpdateShowCar)
	ON_COMMAND(ID_SHOW_POLYGON_NUMBERS, OnShowPolygonNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_POLYGON_NUMBERS, OnUpdateShowPolygonNumbers)
	ON_COMMAND(ID_SHOW_POINTNUMBERS, OnShowPointnumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_POINTNUMBERS, OnUpdateShowPointnumbers)
	ON_COMMAND(ID_SHOW_VERTEX_NUMBERS, OnShowVertexNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_VERTEX_NUMBERS, OnUpdateShowVertexNumbers)
	ON_COMMAND(ID_SHOW_CAMERA_NUMBERS, OnShowCameraNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CAMERA_NUMBERS, OnUpdateShowCameraNumbers)
	ON_COMMAND(ID_JAM_SET, OnJamSet)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA1, OnCameraViewfromcamera1)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA2, OnCameraViewfromcamera2)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA3, OnCameraViewfromcamera3)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA4, OnCameraViewfromcamera4)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA5, OnCameraViewfromcamera5)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA6, OnCameraViewfromcamera6)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA7, OnCameraViewfromcamera7)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA8, OnCameraViewfromcamera8)
	ON_COMMAND(ID_CAMERA_VIEWFROMCAMERA9, OnCameraViewfromcamera9)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA1, OnUpdateCameraViewfromcamera1)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA2, OnUpdateCameraViewfromcamera2)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA3, OnUpdateCameraViewfromcamera3)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA4, OnUpdateCameraViewfromcamera4)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA5, OnUpdateCameraViewfromcamera5)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA6, OnUpdateCameraViewfromcamera6)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA7, OnUpdateCameraViewfromcamera7)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA8, OnUpdateCameraViewfromcamera8)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEWFROMCAMERA9, OnUpdateCameraViewfromcamera9)
	ON_COMMAND(ID_NORMAL_VIEW, OnNormalView)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_VIEW, OnUpdateNormalView)
	ON_COMMAND(ID_CAMERA_VIEWRIGHTSIDE, OnCameraViewrightside)
	ON_COMMAND(ID_CAMERA_VIEWLEFTSIDE, OnCameraViewleftside)
	ON_COMMAND(ID_CAMERA_VIEWFRONT, OnCameraViewfront)
	ON_COMMAND(ID_CAMERA_VIEWTOP, OnCameraViewtop)
	ON_COMMAND(ID_CAMERA_VIEWBACK, OnCameraViewback)
	ON_COMMAND(ID_CAMERA_VIEWBOTTOM, OnCameraViewbottom)
	ON_COMMAND(ID_CAMERA_VIEWFRONTLEFT, OnCameraViewfrontleft)
	ON_COMMAND(ID_CAMERA_VIEWFRONTRIGHT, OnCameraViewfrontright)
	ON_COMMAND(ID_CAMERA_VIEWBACKLEFT, OnCameraViewbackleft)
	ON_COMMAND(ID_CAMERA_VIEWBACKRIGHT, OnCameraViewbackright)
	ON_COMMAND(ID_START_ANIMATION, OnStartAnimation)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_START_ANIMATION, OnUpdateStartAnimation)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_WM_CREATE()
	ON_COMMAND(ID_SHOW_NOSE_CONE, OnShowNoseCone)
	ON_UPDATE_COMMAND_UI(ID_SHOW_NOSE_CONE, OnUpdateShowNoseCone)
	ON_COMMAND(ID_JAM_EXPORTRCRJAM, OnJamExportrcrjam)
	ON_COMMAND(ID_SHOW_OBJECT, OnShowObject)
	ON_UPDATE_COMMAND_UI(ID_SHOW_OBJECT, OnUpdateShowObject)
	ON_COMMAND(ID_JAM_DRAWRCRJAM, OnJamDrawrcrjam)
	ON_UPDATE_COMMAND_UI(ID_JAM_DRAWRCRJAM, OnUpdateJamDrawrcrjam)
	ON_COMMAND(ID_JAM_PRODUCERCRIMAGES, OnJamProducercrimages)
	ON_COMMAND(ID_FILE_EXPORTWAVEFRONTOBJ, OnFileExportwavefrontobj)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_RCRDISTANCES, OnEditRcrdistances)
	ON_COMMAND(ID_SHOW_COCKPIT, OnShowCockpit)
	ON_UPDATE_COMMAND_UI(ID_SHOW_COCKPIT, OnUpdateShowCockpit)
	ON_COMMAND(ID_SHOW_SWHEEL, OnShowSwheel)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SWHEEL, OnUpdateShowSwheel)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarEditorView construction/destruction

static int matrixCount=0;

extern int myGLPushMatrix();
extern int myGLPpopMatrix();

int myGLPushMatrix()
{
		matrixCount++;
		glPushMatrix();
		return matrixCount;
}

int myGLPpopMatrix()
{
		matrixCount--;
		glPopMatrix();
		return matrixCount;
}

char * getFullPathToJamFileName(char *filename)
{
		  static char buffer[512];
		  CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();

		  wsprintf(buffer,"%s\\%s",gp2Location,filename);
		  return buffer;
}

void Mak256SquareImage(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight)
{
       CDC memdcA,memdcB;
	   CBitmap *poldbmps,*poldbmpd;
	   // Create a compatible memory DC
       memdcA.CreateCompatibleDC( NULL );
	   memdcB.CreateCompatibleDC( NULL );
	   
 
	   // Select the bitmap into the DC
       poldbmps = memdcA.SelectObject(&jamBitmap);	   
	   poldbmpd = memdcB.SelectObject(&currentImg);

	   //memdcB.BitBlt( 0, 0, 256, 256, &memdcA, 0,0, SRCCOPY );
	   memdcB.StretchBlt( 0,0, 256,256, &memdcA, 0,0, nWidth, nHeight,SRCCOPY );

	   memdcA.SelectObject( &poldbmps );
	   memdcB.SelectObject( &poldbmpd );
}

void FlipVertically(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight)
{
       CDC memdcA,memdcB;
	   CBitmap *poldbmps,*poldbmpd;
	   // Create a compatible memory DC
       memdcA.CreateCompatibleDC( NULL );
	   memdcB.CreateCompatibleDC( NULL );
	   
 
	   // Select the bitmap into the DC
       poldbmps = memdcA.SelectObject(&jamBitmap);	   
	   poldbmpd = memdcB.SelectObject(&currentImg);

	   //memdcB.BitBlt( 0, 0, 256, 256, &memdcA, 0,0, SRCCOPY );
	   //memdcB.StretchBlt( 0,0, 256,256, &memdcA, 0,0, nWidth, nHeight,SRCCOPY );

	   memdcA.SelectObject( &poldbmps );
	   memdcB.SelectObject( &poldbmpd );
}


CCarEditorView::CCarEditorView()
{
	CString strSection       = "Preferences";
	CWinApp* pApp = AfxGetApp();
	
	PROFILE(showFilled,TRUE)
	PROFILE(showPoints,FALSE)
	PROFILE(showTyres,TRUE)
	PROFILE(showHelmet,TRUE)
	PROFILE(showAxis,TRUE)
	PROFILE(showCameras,TRUE)
	PROFILE(showTexture,FALSE)
	PROFILE(showShadowBox,TRUE)
	PROFILE(showLines,TRUE)
	PROFILE(showCulling,FALSE)
	PROFILE(showNormals,FALSE)
	PROFILE(showSingleColor,FALSE)
	PROFILE(showLighting,FALSE)
	PROFILE(showCar,TRUE)
	PROFILE(showPointNumbers,FALSE)
	PROFILE(showVertexNumbers,FALSE)
	PROFILE(showPolygonNumbers,FALSE)
	PROFILE(showCameraNumbers,FALSE)
	PROFILE(showNoseDetail,TRUE)
	PROFILE(showCockpit,TRUE)
	PROFILE(showSWheel,TRUE)
	
	
	PROFILES(DefaultJamFileName,"ferrari.jam")
	showObject = FALSE;

	currentView = t_Air;

	Animation = FALSE;
}

void LoadJamBmp(CBitmap &output,int idx,char *filename)
{
	CBitmap temp;
	output.LoadBitmap(IDB_TEST);
	char msg[256];
  	  wsprintf(msg,"gamejams/%s",filename);
	  JAM *JamFile = new JAM(getFullPathToJamFileName(msg),FALSE);
	  JamFile->OpenToRead(TRUE);
	  JamFile->LoadSingleBMPImage(temp,
		  JamFile->getImageWidth(idx), 
		  JamFile->getImageHeight(idx),idx);
	  Mak256SquareImage(temp,output,JamFile->getImageWidth(idx),JamFile->getImageHeight(idx));
}

void CCarEditorView::ReReadJams()
{
	return;
	CCarEditorDoc *pDoc = (CCarEditorDoc *)GetDocument();
	pDoc->currentJamBitmap.LoadBitmap(IDB_COLORSCAN);
	
	
	if (((CCarEditorApp*)AfxGetApp())->useExternalJams)
	{
	  LoadJamBmp(tyre,0,"B1ftr.jam");
	  //LoadJamBmp(tyreface,12,"B2ftr.jam");
	  LoadJamBmp(shadow,0,"Cshad1.jam");
	  //LoadJamBmp(helmets,0,"Helmets1.jam");
	}
	else
	{
	  tyre.LoadBitmap(IDB_TYRE);
	  //tyreface.LoadBitmap(IDB_TYREFACE);
	  shadow.LoadBitmap(IDB_SHADOW);	  
	}
	helmets.LoadBitmap(IDB_HELMETS);

	//tyre.LoadBitmap(IDB_TYRE);
	tyreface.LoadBitmap(IDB_TYREFACE);
	//shadow.LoadBitmap(IDB_SHADOW);
}



void CCarEditorView::UpdateJamFile()
{
	return;
	CCarEditorDoc *pDoc = GetDocument();
	char msg[256];
	wsprintf(msg,"gamejams/%s",DefaultJamFileName);
	JAM *JamFileStructure = new JAM(getFullPathToJamFileName(msg),FALSE);
	JamFileStructure->OpenToRead(TRUE);

	int subimages = JamFileStructure->getNumberOfImages();

	for(int j=0;j<subimages;j++)
	{
		CBitmap jamBitmap;
	   //int idx = JamFileStructure->getImageId(j);
	   if (((CCarEditorApp*)AfxGetApp())->drawRCRJam)
	   {
		  jamBitmap.LoadBitmap(IDB_COLORSCAN);
	   }
	   else if (((CCarEditorApp*)AfxGetApp())->useExternalJams)
	   {
		  JamFileStructure->LoadSingleBMPImage(jamBitmap,JamFileStructure->getImageWidth(j), JamFileStructure->getImageHeight(j),j);
	   }
	   else
	   {
		  //jamBitmap.LoadBitmap(IDB_TEST);
		  jamBitmap.LoadBitmap(IDB_COLORSCAN);
	   }

	   

	   TRACE("Jam Size = %d %d\n",JamFileStructure->getImageWidth(j),JamFileStructure->getImageHeight(j));
	   

	   CDC memdcA,memdcB;
	   CBitmap *poldbmps,*poldbmpd;
	   // Create a compatible memory DC
       memdcA.CreateCompatibleDC( NULL );
	   memdcB.CreateCompatibleDC( NULL );
 
	   // Select the bitmap into the DC
       poldbmps = memdcA.SelectObject(&jamBitmap);
	   poldbmpd = memdcB.SelectObject(&pDoc->currentJamBitmap);

	   memdcB.BitBlt( 0, 0, 256, 164, &memdcA, 0,0, SRCCOPY );

	   memdcA.SelectObject( &poldbmps );
	   memdcB.SelectObject( &poldbmpd );
	}

}

CCarEditorView::~CCarEditorView()
{
	CString strSection       = "Preferences";
	CWinApp* pApp = AfxGetApp();

	WR_PROFILE(showFilled);
	WR_PROFILE(showPoints);
	WR_PROFILE(showTyres);
	WR_PROFILE(showHelmet);
	WR_PROFILE(showAxis);
	WR_PROFILE(showCameras)
	WR_PROFILE(showTexture)
	WR_PROFILE(showShadowBox)
	WR_PROFILE(showLines)
	WR_PROFILE(showCulling)
	WR_PROFILE(showNormals)
	WR_PROFILE(showSingleColor)
	WR_PROFILE(showLighting)
	WR_PROFILE(showCar)
	WR_PROFILE(showPointNumbers)
	WR_PROFILE(showVertexNumbers)
	WR_PROFILE(showPolygonNumbers)
	WR_PROFILE(showCameraNumbers)
	WR_PROFILES(DefaultJamFileName)
	WR_PROFILE(showNoseDetail)
	WR_PROFILE(showCockpit)
	WR_PROFILE(showSWheel)
}

BOOL CCarEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorView drawing

void CCarEditorView::OnDraw(CDC* pDC)
{
	CCarEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorView printing

BOOL CCarEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCarEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCarEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorView diagnostics

#ifdef _DEBUG
void CCarEditorView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CCarEditorView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

void DrawText2D(HDC hdc,CScrollView *scview,GLdouble wx,GLdouble wy,GLdouble wz,int idx)
{
        GLdouble sx;
		GLdouble sy;
		GLdouble sz;
		GLdouble model[16];
		GLdouble projection[16];
		GLint view[4];

		myGLPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,view);
		gluProject(wx,wy,wz,model,projection,view,&sx,&sy,&sz);

		char buffer[10];
		wsprintf(buffer,"%d",idx);
		//dc->SelectStockObject(ANSI_VAR_FONT);
		if (!((CCarEditorView*)(scview))->showFilled)
		{
		  //dc->SetBkMode(TRANSPARENT);
		}
		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,GetStockObject(ANSI_VAR_FONT));

		RECT rect;
		scview->GetClientRect(&rect);

		int midy = (rect.top+rect.bottom)/2;
		int midx = (rect.left+rect.right)/2;

		int winx = (int)(sx-midx);
		int winy = (int)(sy-midy);
		
		::TextOut(hdc,(int)(midx+winx),(int)(midy-winy),buffer,strlen(buffer));
		myGLPpopMatrix();
}

CCarEditorDoc* CCarEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarEditorDoc)));
	return (CCarEditorDoc*)m_pDocument;
}

GLvoid CalculateVectorNormal(GLdouble fVert1[], GLdouble fVert2[],
                             GLdouble fVert3[], GLdouble *fNormalX,
                             GLdouble *fNormalY, GLdouble *fNormalZ)
{
    GLdouble Qx, Qy, Qz, Px, Py, Pz;
 
    Qx = fVert2[0]-fVert1[0];
    Qy = fVert2[1]-fVert1[1];
    Qz = fVert2[2]-fVert1[2];
    Px = fVert3[0]-fVert1[0];
    Py = fVert3[1]-fVert1[1];
    Pz = fVert3[2]-fVert1[2];
 
    *fNormalX = Py*Qz - Pz*Qy;
    *fNormalY = Pz*Qx - Px*Qz;
    *fNormalZ = Px*Qy - Py*Qx;
}




void DrawCamera(Camera *camera,double wx,double wy,double wz,BOOL showAxis)
{
	myGLPushMatrix();
    GLUquadricObj* tyre         =  gluNewQuadric();
	glTranslated(wx,wy,wz);
	gluQuadricDrawStyle(tyre,GLU_SILHOUETTE);
	gluSphere(tyre,100,50,50);

	glRotated(camera->Direction,0.0f,0.0f,1.0f);
	glRotated(camera->Roll,0.0f,1.0f,0.0f);
	glRotated(camera->Pitch,1.0f,0.0f,0.0f);

	if (showAxis)
	{
	    float v = 1350.0f;
	// draw carthesian axes
	    glBegin(GL_LINES);
		// red x axis
		glColor3f(1.0f,0.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(v,0.0f,0.0f);
		
		// green y axis
		glColor3f(0.f,1.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,v,0.0f);
		// blue z axis
		glColor3f(0.f,0.f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,v);
		glEnd();
	}

	myGLPpopMatrix();


}



typedef struct
{
	BOOL  enabled;
	BOOL  polygon;
	int x[7];
	int y[7];
} t_TexturePos;

/*
t_TexturePos textures[] =
{
	{FALSE,0,0,0,0,0,0,0,0},
	{FALSE,0,0,0,0,0,0,0,0},    
	{TRUE,188,114,134,114,134,97,188,97},
	{TRUE,0,164,256,164,256,0,0,0},
	{TRUE,0,164,256,164,256,0,0,0},
	//{TRUE,134,97,188,97,188,114,134,114,},
	{FALSE,0,0,0,0,0,0,0,0},
};
*/

/*
t_TexturePos htextures[] =
{
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   {TRUE,TRUE,4,0,0,58,0,58,15,0,15},
   
};
*/

t_TexturePos textures[390];
t_TexturePos htextures[390];

//static int numberOfTextures = sizeof(textures)/sizeof(textures[0]);

BOOL isTextureCfgPolygon(int idx)
{
   return textures[idx].polygon;
}

BOOL isTextureCfgDefined(int idx)
{
   return textures[idx].enabled;
}


t_TexturePos *getTexture(int idx,BOOL selected,BOOL showTexture)
{
	if (idx >= 390 && showTexture)
	{
		glDisable(GL_TEXTURE_2D);
		if (!selected) glColor3f (1.0f,1.0f,0.0f);
		return &textures[0];
	}
	else
	{
		if (!textures[idx].enabled)
		{
		   glDisable(GL_TEXTURE_2D);
		   if (!selected && showTexture) glColor3f (1.0f,1.0f,0.0f);
		   return  &textures[0];
		}
		
	}
	if (showTexture) glEnable(GL_TEXTURE_2D);
	return &textures[idx];
}

t_TexturePos *getHelmetTexture(int idx,BOOL selected,BOOL showTexture)
{
	/*
	if (idx >= 390 && showTexture)
	{
		glDisable(GL_TEXTURE_2D);
		if (!selected) glColor3f (1.0f,1.0f,0.0f);
		return &htextures[0];
	}
	else
	{
		if (!textures[idx].enabled)
		{
		   glDisable(GL_TEXTURE_2D);
		   if (!selected && showTexture) glColor3f (1.0f,1.0f,0.0f);
		   return  &htextures[0];
		}
		
	}
	*/
	if (showTexture) glEnable(GL_TEXTURE_2D);
	return &htextures[idx];
}


double getTX(t_TexturePos *text,int idx)
{
	double x=0;
	x=(double)text->x[idx];
	return (double)(x/256.0);
}

double getTY(t_TexturePos *text,int idx)
{
	double x=0;
	x=(double)text->y[idx];
	return 1.0-((double)(x/256.0));
}

static int hidx=0;
static int hidy=4; 

double getHTX(t_TexturePos *text,int idx)
{
	double x=0;
	x=(double)text->x[idx];
	x+=(hidx*58);
	return (double)(x/256.0);
}

double getHTY(t_TexturePos *text,int idx)
{
	double x=0;
	x=(double)text->y[idx];
	x+=(hidy*15);
	return 1.0-((double)(x/256.0));
}


char *getline(FILE *fp)
{
	static char line[512];
	fscanf(fp,"%[^\n]\n",line);
	return line;
}

void LoadTextureCfg()
{
	FILE *fp = fopen("texture.cfg","r");

	for(int i=0;i<390;i++)
	{
		textures[i].enabled = FALSE;
		textures[i].polygon = FALSE;
	}

	if (fp==NULL)
	{
		//AfxMessageBox("Cannot Open Texture Cfg");
		int numTextures = sizeof(AllTextures)/sizeof(AllTextures[0]);
		for(int i=0;i<numTextures;i++)
		{
			int idx = AllTextures[i].id;
			int num = AllTextures[i].num;
			
			textures[idx].enabled = TRUE;
			int count = 0;
			for(int j=0;j<num;j++)
			{		   
			    int x = AllTextures[i].xy[count];
				if (x == -1) {
				  textures[idx].polygon = FALSE;
				  break;
			    }
				textures[idx].x[j] = x;
				count++;
			    textures[idx].y[j] = AllTextures[i].xy[count];				
				count++;
			}
		}
		return;
	}

	while(!feof(fp))
	{
		int idx,num,pos;
		char *line = getline(fp);
		if (line && strlen(line))
		{
		 if (line[0] == '/' && line[1] == '/') continue;
		 if (line[0] == '#') continue;

		 sscanf(line,"%d,%d%n",&idx,&num,&pos);
		 line+=pos;
		 textures[idx].enabled = TRUE;
		 for(int i=0;i<num;i++)
		 {
			int x,y;
			sscanf(line,",%d,%d%n",&x,&y,&pos);

			if (x==-1) 	{
				textures[idx].polygon = FALSE;
				break;
			}
			line+=pos;
			textures[idx].x[i] = x;
			textures[idx].y[i] = y;
		 }
		}
	}

	fclose(fp);
}

void LoadHTextureCfg()
{
	FILE *fp = fopen("helmet.cfg","r");

	for(int i=0;i<390;i++)
	{
		htextures[i].enabled = FALSE;
		htextures[i].polygon = FALSE;
	}

	if (fp==NULL)
	{
		//AfxMessageBox("Cannot Open helmet.Cfg");
		int numTextures = sizeof(AllHelmets)/sizeof(AllHelmets[0]);
		for(int i=0;i<numTextures;i++)
		{
			int idx = AllHelmets[i].id;
			int num = AllHelmets[i].num;
			
			htextures[idx].enabled = TRUE;
			int count = 0;
			for(int j=0;j<num;j++)
			{		   
			    int x = AllHelmets[i].xy[count];
				if (x == -1) {
				  htextures[idx].polygon = FALSE;
				  break;
			    }
				htextures[idx].x[j] = x;
				count++;
			    htextures[idx].y[j] = AllHelmets[i].xy[count];				
				count++;
			}
		}
		return;
	}

	while(!feof(fp))
	{
		int idx,num,pos;
		char *line = getline(fp);
		if (line && strlen(line))
		{
		 if (line[0] == '/' && line[1] == '/') continue;
		 if (line[0] == '#') continue;

		 sscanf(line,"%d,%d%n",&idx,&num,&pos);
		 line+=pos;
		 htextures[idx].enabled = TRUE;
		 for(int i=0;i<num;i++)
		 {
			int x,y;
			sscanf(line,",%d,%d%n",&x,&y,&pos);

			if (x==-1) 	{
				htextures[idx].polygon = FALSE;
				break;
			}
			line+=pos;
			htextures[idx].x[i] = x;
			htextures[idx].y[i] = y;
		 }
		}
	}

	fclose(fp);
}



void CCarEditorView::DrawObjectTextures(Object3D *object,GLdouble x,GLdouble y,GLdouble z)
{
	myGLPushMatrix();

	glTranslated(x,y,z);
	for(int i=0;i<object->textureSize;i++)
	{
		  if (showTexture)
		  {
			 //glEnable(GL_TEXTURE_2D);
			 //TextureMap(shadow);
		  }

		  int colorIdx = (i % numRandomColors);
		  t_Color currentColor = RandomColors[colorIdx];

		  TextureCmd *cmd =&(object->textureData[i]);
		  BOOL selected=FALSE;

		  if (cmd==NULL) continue;

		  if (cmd->numPts > 0)
		  {
			  //glColor3d(1.0f,0.0f,0.0f);
			  if (object->selectedTextureCmd!=NULL)
			  {
				if (object->selectedTextureCmd == &object->textureData[i])
				{
					//AfxMessageBox("dr Selected Texture\n");
					glColor3f (1.0f,1.0f,0.0f);
					selected=TRUE;
				}
				else
			    {
				  if (showSingleColor)
			        glColor3f (1.0f, 0.0f,0.0f);
				  else
					glColor3f (currentColor.r, currentColor.g, currentColor.b);
			    }
			  }
			  else
			  {
				if (showSingleColor)
			        glColor3f (1.0f, 0.0f,0.0f);
				  else
			        glColor3f (currentColor.r, currentColor.g, currentColor.b);
			  }

			  if (showFilled || selected)
		      {
			  t_TexturePos *coordinates = getTexture(i,selected,showTexture);
			  GLdouble norm[6][3];
			  int ptcount=0;
			  GLdouble nx,ny,nz;
			  GLdouble cx,cy,cz;
			  GLdouble x,y,z;

			  if (cmd)
			  {
			  // calculate normal first
			  for(int j=0;j<cmd->numPts;j++)
			  {
				  int val = cmd->PtsList[j];
				  int aval = abs(val);
				  if (aval > 0 && aval <= object->numPoints)
				  {
					int F,T;
					if (val > 0)
					{
				     F = object->pointData[val-1].x;
		             T = object->pointData[val-1].y;					
					}
				    else
					{				   
				     F = object->pointData[aval-1].y;
		             T = object->pointData[aval-1].x;
					}
					GLdouble wx,wy,wz;
					if (F >=0 && F <= object->numVerticies){
						wx = object->vertexData[F].x;
						wy = object->vertexData[F].y;
						wz = object->vertexData[F].z;
						if (ptcount < 6){
							norm[ptcount][0] = wx;
							norm[ptcount][1] = wy;
							norm[ptcount][2] = wz;
							ptcount++;
						}
						
					}
				  }
			  }
			  if (cmd->numPts >=3)
			  {			    	
				CalculateVectorNormal(norm[0],norm[1],norm[2],&nx,&ny,&nz);
				nx = nx/100;
			    ny = ny/100;
			    nz = nz/100;
				glNormal3d(nx,ny,ny);
				x = (double)object->pointData[2].x;
		        y = (double)object->pointData[2].y;
		        z = (double)object->pointData[2].z;
			  }

			  glBegin(GL_POLYGON);			  
			  cx = 0.0;
			  cy = 0.0;
			  cz = 0.0;
			  ptcount = 0;
			  for(int j=0;j<cmd->numPts;j++)
			  {
				  int val = cmd->PtsList[j];
				  int aval = abs(val);

				  if (aval > 0 && aval <= object->numPoints)
				  {
					int F,T;
					if (val > 0)
					{
				     F = object->pointData[val-1].x;
		             T = object->pointData[val-1].y;					
					}
				    else
					{				   
				     F = object->pointData[aval-1].y;
		             T = object->pointData[aval-1].x;
					}
					GLdouble wx,wy,wz;
					if (F >=0 && F <= object->numVerticies){
						wx = object->vertexData[F].x;
						wy = object->vertexData[F].y;
						wz = object->vertexData[F].z;
						glVertex3d(wx,wy,wz);
						if (ptcount < 6){
							norm[ptcount][0] = wx;
							norm[ptcount][1] = wy;
							norm[ptcount][2] = wz;
							ptcount++;
						}
						cx += wx;
					    cy += wy;
					    cz += wz;
					}					
				  }

				   /*
				 	   double xpos = getTX(coordinates,ptcount);
					   double ypos = getTY(coordinates,ptcount);
					   glTexCoord2d(xpos, ypos);
					   norm[ptcount][0] = x;
					   norm[ptcount][1] = y;
					   norm[ptcount][2] = z;
					   ptcount++;
				  */
				}
			  }
			  cx = cx/ptcount;
			  cy = cy/ptcount;
			  cz = cz/ptcount;

			  cmd->cx = cx;
			  cmd->cy = cy;
			  cmd->cz = cz;

			  
			  
			
			  glEnd();

			  if (showNormals)
			  {
			   glColor3d(0.0f,0.0f,0.0f);
			   glBegin(GL_LINES);
			   glVertex3d(cx,cy,cz);
			   glVertex3d(cx+(1*nx),cy+(1*ny),cz+(1*nz));
			   glEnd();
			  }


			  if (selected)
			  {
				 glLineWidth(4.0f);
				 glBegin(GL_LINES);
				 for(int j=0;j<2;j++)
			     {
				  //int val = cmd->PtsList[j];
				  //int idxF,idxT;
				  //double x,y,z;
				  //int aval = abs(val);

				  if (j==0) glColor3f(1.0f,0.0f,0.0f);
				  else glColor3f(0.0f,0.0f,1.0f);
				  
				  int val = cmd->PtsList[j];
				  int aval = abs(val);

				  if (aval > 0 && aval <= object->numPoints)
				  {
					int F,T;
					if (val > 0)
					{
				     F = object->pointData[val-1].x;
		             T = object->pointData[val-1].y;					
					}
				    else
					{				   
				     F = object->pointData[aval-1].y;
		             T = object->pointData[aval-1].x;
					}
					GLdouble wx,wy,wz;
					if (F >=0 && F <= object->numVerticies){
						wx = object->vertexData[F].x;
						wy = object->vertexData[F].y;
						wz = object->vertexData[F].z;
						glVertex3d(wx,wy,wz);
					}
					/*
				  if (aval > 0 && aval <= object->numVerticies)
				  {
				   if (val > 0)
				   {
				    idxF = object->vertexData[val-1].from;
		            idxT = object->vertexData[val-1].to;					
				   }
				   else
				   {				   
				    idxF = object->vertexData[aval-1].to;
		            idxT = object->vertexData[aval-1].from;
				   }
				   x = (double)object->pointData[idxF].x;
		           y = (double)object->pointData[idxF].y;
		           z = (double)object->pointData[idxF].z;
				   glVertex3d(x,y,z);
				  }
				  */
				  }
			    }
				glEnd();
				glLineWidth(1.0f);
			  }

			  }
			  }



	}
	
	myGLPpopMatrix();
}


//void CCarEditorView::OpenGLDrawScene(CPaintDC *dc)
void CCarEditorView::OpenGLDrawScene(HDC hdc)
{
	    CCarEditorDoc *pDoc = GetDocument();
		if (pDoc == NULL)      return;
		if (pDoc->car == NULL) return;
		//glTranslated(-pDoc->car->origin.x,-pDoc->car->origin.y,-pDoc->car->origin.z);
        

		//COLORREF color = ::GetSysColor(COLOR_WINDOW);
	    //glClearColor((float)GetRValue(color)/255.0f,
		//		 (float)GetGValue(color)/255.0f,
		//		 (float)GetBValue(color)/255.0f,
		//		 (float)1.0);


        glColor3f (0.0f, 0.0f, 0.0f);
		if (showTexture )glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);

		if (showLighting)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}

		if (showCulling)
		{
		 glCullFace(GL_FRONT);
		 glEnable(GL_CULL_FACE);
		}
		else
		{
		 glDisable(GL_CULL_FACE);
		}

		if (showAxis)
		{
		float v = 10.0f;

		// draw carthesian axes
	    glBegin(GL_LINES);
		// red x axis
		glColor3f(1.0f,0.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(v,0.0f,0.0f);
		
		// green y axis
		glColor3f(0.f,1.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,v,0.0f);
		// blue z axis
		glColor3f(0.f,0.f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,v);
		glEnd();
		}

		if (pDoc->car == NULL) return;

		int size = pDoc->car->numPoints;
		int vsize = pDoc->car->numVerticies;

		int i;

		
		//TRACE("Textures = %d\n",pDoc->car->textureSize);
		if (showTexture)
		{
		  glEnable(GL_TEXTURE_2D);
		  //CBitmap bmp;
		  //bmp.LoadBitmap(IDB_TEST);
		  //TextureMap(pDoc->currentJamBitmap);

		  //currentJam
		}
		else glDisable(GL_TEXTURE_2D);

	
		if (showCar && pDoc->car)
		{
			DrawObjectTextures(pDoc->car,0,0,0);
		//if (showTexture) glDisable(GL_TEXTURE_2D);


		glColor3f (0.0f,0.0f,0.0f);

		for(int i=0;i<vsize-1;i++)
		{
		   BOOL lineSelected=FALSE;

		   if (pDoc->car->selectedVertex!=NULL)
		   {
			   if (pDoc->car->selectedVertex == &pDoc->car->vertexData[i])
			   {
				   glLineWidth(4.0f);
				   glColor3f (1.0f,0.0f,0.0f);
				   lineSelected=TRUE;
				   
			   }
			   else
			   {
				   glColor3f (0.0f,0.0f,0.0f);
				   glLineWidth(1.0f);
			   }
		   }		   
		   if (lineSelected || showLines) glBegin(GL_LINES);
		   int idxF = pDoc->car->vertexData[i].from;
		   int idxT = pDoc->car->vertexData[i].to; 

		   /*
		   x = (double)pDoc->car->pointData[idxF].x;
		   y = (double)pDoc->car->pointData[idxF].y;
		   z = (double)pDoc->car->pointData[idxF].z;
		   pDoc->car->vertexData[i].cx = x;
		   pDoc->car->vertexData[i].cy = y;
		   pDoc->car->vertexData[i].cz = z;
		   if (lineSelected || showLines) glVertex3d(x,y,z);
		   x = (double)pDoc->car->pointData[idxT].x;
		   y = (double)pDoc->car->pointData[idxT].y;
		   z = (double)pDoc->car->pointData[idxT].z;
		   */
		   //pDoc->car->vertexData[i].cx += x;
		   //pDoc->car->vertexData[i].cy += y;
		   //pDoc->car->vertexData[i].cz += z;
		   //pDoc->car->vertexData[i].cx /= 2;
		   //pDoc->car->vertexData[i].cy /= 2;
		   //pDoc->car->vertexData[i].cz /= 2;
		   
		   //if (lineSelected || showLines) glVertex3d(x,y,z);
		   if (lineSelected || showLines) glEnd();


		 }

		if (pDoc->car->selectedVertex!=NULL)
		{
		   //AfxMessageBox("Selected");
		   double x,y,z;
		   x = (double)pDoc->car->selectedVertex->x;
		   y = (double)pDoc->car->selectedVertex->y;
		   z = (double)pDoc->car->selectedVertex->z;
		   glColor3f (1.0f,0.0f,0.0f);
		   int size = 60;
		   glPointSize(5.0f);
		   glBegin(GL_POINTS);
		   glVertex3d(x,y,z);
		   glEnd();
		  
		   glColor3f (0.0f,0.0f,1.0f);
		   glBegin(GL_LINES);		   
		   glVertex3d(x-size,y-size,z);
		   glVertex3d(x+size,y+size,z);
		   glEnd();
		   glBegin(GL_LINES);		   
		   glVertex3d(x+size,y-size,z);
		   glVertex3d(x-size,y+size,z);
		   glEnd();
		}

		if (showPoints)
		{
		 glPointSize(2.0f);
		 
		 glBegin(GL_POINTS);

		 for(int i=0;i<pDoc->car->numVerticies;i++)
		 {	    	
		  glColor3f(1.0f,0.0f,0.0f);
		  GLdouble wx,wy,wz;
		  wx = pDoc->car->vertexData[i].x;
		  wy = pDoc->car->vertexData[i].y;
		  wz = pDoc->car->vertexData[i].z;
		  glVertex3d(wx,wy,wz);
		 }
		 glEnd();
        }

		if (showLines)
		{
		 for(int i=0;i<pDoc->car->numPoints;i++)
		 {	  
		  glBegin(GL_LINES);
		  glColor3f(0.0f,0.0f,0.0f);
		  int F = pDoc->car->pointData[i].x;
		  int T = pDoc->car->pointData[i].y;
		  GLdouble wx,wy,wz;
		  if (F >=0 && F <= pDoc->car->numVerticies){
			  wx = pDoc->car->vertexData[F].x;
			  wy = pDoc->car->vertexData[F].y;
			  wz = pDoc->car->vertexData[F].z;
			  glVertex3d(wx,wy,wz);
		  }
		  if (T >=0 && T <= pDoc->car->numVerticies){
			wx = pDoc->car->vertexData[T].x;
			wy = pDoc->car->vertexData[T].y;
			wz = pDoc->car->vertexData[T].z;
		  }
		  glVertex3d(wx,wy,wz);
		  glEnd();
		 }
		}


		if (pDoc->car->selectedVertex!=NULL)
		{
		   double x,y,z;
		   x = (double)pDoc->car->selectedVertex->x;
		   y = (double)pDoc->car->selectedVertex->y;
		   z = (double)pDoc->car->selectedVertex->z;
		   glColor3f (1.0f,0.0f,0.0f);
		   int size = 60;
		   glPointSize(5.0f);
		   glBegin(GL_POINTS);
		   glVertex3d(x,y,z);
		   glEnd();
		  
		   glColor3f (0.0f,0.0f,1.0f);
		   glBegin(GL_LINES);		   
		   glVertex3d(x-size,y-size,z);
		   glVertex3d(x+size,y+size,z);
		   glEnd();
		   glBegin(GL_LINES);		   
		   glVertex3d(x+size,y-size,z);
		   glVertex3d(x-size,y+size,z);
		   glEnd();
		}
		}
		

		//if (showHelmet) DrawHelmetModel();
		//if (showObject) DrawObjectModel();

		if (showCameras)
		{
		  for(i=0;i<9;i++)
		  {	    	
		  if (pDoc->car->selectedCamera)
		  {			  
			 if (pDoc->car->selectedCamera == &(pDoc->car->cameraData[i]))
		     {		  
		        glColor3f(1.0f,0.0f,0.0f);
			 }
			 else
			 {
				 glColor3f(0.0f,1.0f,0.0f);
			 }
		  }
		  else
		  {
		    glColor3f(0.0f,1.0f,0.0f);
		  }
		  GLdouble wx,wy,wz;
		  wx = pDoc->car->cameraData[i].pos.X*8;
		  wy = pDoc->car->cameraData[i].pos.Y*8;
		  wz = pDoc->car->cameraData[i].pos.Z*8;
		  DrawCamera(&pDoc->car->cameraData[i],wx,wz,wy,showAxis);
		  }
		}


		if (showTyres)
		{
		 for(int j=37;j<41;j++)
		 {
			 GLdouble ox,oy,oz;
			 ox = pDoc->car->vertexData[j].x;
			 oy = pDoc->car->vertexData[j].y;
		     oz = pDoc->car->vertexData[j].z;

			 if (pDoc->car->wheel)
				DrawObjectTextures(pDoc->car->wheel,ox,oy,oz);

			 if (pDoc->car->wheel)
			 for(i=0;i<pDoc->car->wheel->numPoints;i++)
			 {	  
			  glBegin(GL_LINES);
			  glColor3f(0.0f,0.0f,0.0f);
			  int F = pDoc->car->wheel->pointData[i].x;
			  int T = pDoc->car->wheel->pointData[i].y;
			  GLdouble wx,wy,wz;
			  if (F >=0 && F <= pDoc->car->wheel->numVerticies){
				  wx = pDoc->car->wheel->vertexData[F].x;
				  wy = pDoc->car->wheel->vertexData[F].y;
				  wz = pDoc->car->wheel->vertexData[F].z;
				  glVertex3d(ox+wx,oy+wy,oz+wz);
			  }
			  if (T >=0 && T <= pDoc->car->wheel->numVerticies){
				wx = pDoc->car->wheel->vertexData[T].x;
				wy = pDoc->car->wheel->vertexData[T].y;
				wz = pDoc->car->wheel->vertexData[T].z;
			  }
			  glVertex3d(ox+wx,oy+wy,oz+wz);
			  glEnd();
			 }
		 }
		}

		if (showHelmet)
		{
		 for(int j=0;j<1;j++)
		 {
			 GLdouble ox,oy,oz;
			 ox = pDoc->car->vertexData[j].x;
			 oy = pDoc->car->vertexData[j].y;
		     oz = pDoc->car->vertexData[j].z;

			 if (pDoc->car->helmet)
				DrawObjectTextures(pDoc->car->helmet,ox,oy,oz);

			 if (pDoc->car->helmet)
			 for(i=0;i<pDoc->car->helmet->numPoints;i++)
			 {	  
			  glBegin(GL_LINES);
			  glColor3f(0.0f,0.0f,0.0f);
			  int F = pDoc->car->helmet->pointData[i].x;
			  int T = pDoc->car->helmet->pointData[i].y;
			  GLdouble wx,wy,wz;
			  if (F >=0 && F <= pDoc->car->helmet->numVerticies){
				  wx = pDoc->car->helmet->vertexData[F].x;
				  wy = pDoc->car->helmet->vertexData[F].y;
				  wz = pDoc->car->helmet->vertexData[F].z;
				  glVertex3d(ox+wx,oy+wy,oz+wz);
			  }
			  if (T >=0 && T <= pDoc->car->helmet->numVerticies){
				wx = pDoc->car->helmet->vertexData[T].x;
				wy = pDoc->car->helmet->vertexData[T].y;
				wz = pDoc->car->helmet->vertexData[T].z;
			  }
			  glVertex3d(ox+wx,oy+wy,oz+wz);
			  glEnd();
			 }
		 }
		}

		if (showHelmet)
		{
		 for(int j=0;j<1;j++)
		 {
			 GLdouble ox,oy,oz;
			 ox = 0;
			 oy = 0;
			 oz = 0;

			 if (pDoc->car->cockpit)
				DrawObjectTextures(pDoc->car->cockpit,ox,oy,oz);
	
			 if (showCockpit && pDoc->car->cockpit)
			 for(i=0;i<pDoc->car->cockpit->numPoints;i++)
			 {	  
			  glBegin(GL_LINES);
			  glColor3f(0.0f,0.0f,0.0f);
			  int F = pDoc->car->cockpit->pointData[i].x;
			  int T = pDoc->car->cockpit->pointData[i].y;
			  GLdouble wx,wy,wz;
			  if (F >=0 && F <= pDoc->car->cockpit->numVerticies){
				  wx = pDoc->car->cockpit->vertexData[F].x;
				  wy = pDoc->car->cockpit->vertexData[F].y;
				  wz = pDoc->car->cockpit->vertexData[F].z;
				  glVertex3d(ox+wx,oy+wy,oz+wz);
			  }
			  if (T >=0 && T <= pDoc->car->cockpit->numVerticies){
				wx = pDoc->car->cockpit->vertexData[T].x;
				wy = pDoc->car->cockpit->vertexData[T].y;
				wz = pDoc->car->cockpit->vertexData[T].z;
			  }
			  glVertex3d(ox+wx,oy+wy,oz+wz);
			  glEnd();
			 }
		 }
		}

		if (showHelmet)
		{
		 for(int j=0;j<1;j++)
		 {
			 GLdouble ox,oy,oz;
			 ox = 0;
			 oy = 0;
			 oz = 0;
			 if (pDoc->car->cockpit){
				ox = pDoc->car->cockpit->vertexData[84].x;
				oy = pDoc->car->cockpit->vertexData[84].y;
				oz = pDoc->car->cockpit->vertexData[84].z;
			 }

			 if (pDoc->car->swheel)
				DrawObjectTextures(pDoc->car->swheel,ox,oy,oz);

			 if (showSWheel && pDoc->car->swheel)
			 for(i=0;i<pDoc->car->swheel->numPoints;i++)
			 {	  
			  glBegin(GL_LINES);
			  glColor3f(0.0f,0.0f,0.0f);
			  int F = pDoc->car->swheel->pointData[i].x;
			  int T = pDoc->car->swheel->pointData[i].y;
			  GLdouble wx,wy,wz;
			  if (F >=0 && F <= pDoc->car->swheel->numVerticies){
				  wx = pDoc->car->swheel->vertexData[F].x;
				  wy = pDoc->car->swheel->vertexData[F].y;
				  wz = pDoc->car->swheel->vertexData[F].z;
				  glVertex3d(ox+wx,oy+wy,oz+wz);
			  }
			  if (T >=0 && T <= pDoc->car->swheel->numVerticies){
				wx = pDoc->car->swheel->vertexData[T].x;
				wy = pDoc->car->swheel->vertexData[T].y;
				wz = pDoc->car->swheel->vertexData[T].z;
			  }
			  glVertex3d(ox+wx,oy+wy,oz+wz);
			  glEnd();
			 }
		 }
		}

		
		SwapAnyBuffers(hdc);

		if (pDoc->car != NULL) {
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			Vertex * vert = pDoc->car->SelectVertex(::GetDC(m_hWnd),this,pt.x,pt.y,0);
			if (vert){
				if (pDoc->car->selectedVertex != vert)
				{
					pDoc->car->selectedVertex = vert;
					InvalidateRect(NULL,FALSE);
				}
				else {
					pDoc->car->selectedVertex = vert;
				}
			}	
		}	

		if (showPointNumbers && showCar && pDoc->car)
		{
			for(i=0;i<pDoc->car->numVerticies;i++)
			{
			GLdouble wx = pDoc->car->vertexData[i].x;
			GLdouble wy = pDoc->car->vertexData[i].y;
			GLdouble wz = pDoc->car->vertexData[i].z;

			DrawText2D(hdc,this,wx,wy,wz,i);		
			}
		}

		if (showPointNumbers && showHelmet && pDoc->car->cockpit)
		{
			for(i=0;i<pDoc->car->cockpit->numVerticies;i++)
			{
			GLdouble wx = pDoc->car->cockpit->vertexData[i].x;
			GLdouble wy = pDoc->car->cockpit->vertexData[i].y;
			GLdouble wz = pDoc->car->cockpit->vertexData[i].z;

			DrawText2D(hdc,this,wx,wy,wz,i);		
			}
		}
		

		if (showVertexNumbers && showCar )
		{
		 for(i=0;i<pDoc->car->numPoints-1;i++)
		 {
			 int F = pDoc->car->pointData[i].x;
			 int T = pDoc->car->pointData[i].y;

			 if (F < pDoc->car->numVerticies && T < pDoc->car->numVerticies)
			 {
			 double cx = (pDoc->car->vertexData[F].x + pDoc->car->vertexData[T].x)/2;
			 double cy = (pDoc->car->vertexData[F].y + pDoc->car->vertexData[T].y)/2;
			 double cz = (pDoc->car->vertexData[F].z + pDoc->car->vertexData[T].z)/2;
			 GLdouble wx = cx;
		     GLdouble wy = cy;
		     GLdouble wz = cz;
		     DrawText2D(hdc,this,wx,wy,wz,i+1);
			 }
		 }
		}


		/*
		if (showPolygonNumbers && showCar)
		{
		 for(i=0;i<pDoc->car->textureSize;i++)
		 {
			 TextureCmd *cmd =&(pDoc->car->textureData[i]);

			 GLdouble wx = cmd->cx;
		     GLdouble wy = cmd->cy;
		     GLdouble wz = cmd->cz;
		     DrawText2D(hdc,this,wx,wy,wz,i);
		 }
		}

		if (showCameraNumbers)
		{
		  for(i=0;i<9;i++)
		  {	    	
		  GLdouble wx,wy,wz;
		  wx = pDoc->car->cameraData[i].pos.X;
		  wy = pDoc->car->cameraData[i].pos.Y;
		  wz = pDoc->car->cameraData[i].pos.Z;
		  DrawText2D(hdc,this,wx,wy,wz,i+1);
		  }
		}

		
		*/

		//m_xRotate
		//DrawText2D(hdc,this,wx,wy,wz,m_xRotate);

		/*
		SetBkMode(hdc,TRANSPARENT);
		char buffer[20];
		sprintf(buffer,"xr=%f",m_xRotate);
		::TextOut(hdc,10,10,buffer,strlen(buffer));
		sprintf(buffer,"xy=%f",m_yRotate);
		::TextOut(hdc,10,30,buffer,strlen(buffer));
		sprintf(buffer,"xz=%f",m_zRotate);
		::TextOut(hdc,10,50,buffer,strlen(buffer));
		sprintf(buffer,"dist=%f",dist);
		::TextOut(hdc,10,70,buffer,strlen(buffer));
		*/
		



	    
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CCarEditorView message handlers

void CCarEditorView::OnFillobjects() 
{
	showFilled = !showFilled;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateFillobjects(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showFilled);	
}

void CCarEditorView::OnShowPoints() 
{
	showPoints = !showPoints;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowPoints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showPoints);
}

void CCarEditorView::OnShowtyres() 
{
	showTyres = !showTyres;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowtyres(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showTyres);
}

void CCarEditorView::OnHelmet() 
{
	showHelmet = !showHelmet;	
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateHelmet(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showHelmet);
}

void CCarEditorView::OnDrawaxis() 
{
	showAxis = !showAxis;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateDrawaxis(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showAxis);
}

void CCarEditorView::OnShowcameras() 
{
	showCameras = !showCameras;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowcameras(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showCameras);
	
}

void CCarEditorView::TextureMap(CBitmap &cbmp)
{
  BITMAP bmp;
  cbmp.GetBitmap( &bmp );

  CDC*          bitmapDc = new CDC;
  bitmapDc -> CreateCompatibleDC(  GetDC() );

  CBitmap* oldBitMap = (CBitmap*) bitmapDc -> SelectObject( &cbmp );

  unsigned char *lpvBits = new unsigned char[bmp.bmWidth * bmp.bmHeight *3];
  BITMAPINFO *bmpinfo = new BITMAPINFO;

  bmpinfo->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  bmpinfo->bmiHeader.biWidth         = bmp.bmWidth;
  bmpinfo->bmiHeader.biHeight        = bmp.bmHeight;
  bmpinfo->bmiHeader.biPlanes        = 1;
  bmpinfo->bmiHeader.biBitCount      = 24;
  bmpinfo->bmiHeader.biCompression   = BI_RGB;
  bmpinfo->bmiHeader.biSizeImage     = bmp.bmWidth * bmp.bmHeight * 3;
  bmpinfo->bmiHeader.biXPelsPerMeter = 0;
  bmpinfo->bmiHeader.biYPelsPerMeter = 0;
  bmpinfo->bmiHeader.biClrImportant  = 0;
  bmpinfo->bmiHeader.biClrUsed       = 0;

  if (!::GetDIBits( bitmapDc -> GetSafeHdc(), HBITMAP( cbmp ), 0,bmp.bmHeight, lpvBits, bmpinfo, DIB_RGB_COLORS ))
  {
   AfxMessageBox("GetDIBits error");
   return ;
  }

  unsigned int size = (bmpinfo->bmiHeader.biSizeImage);

  for(unsigned int i=0;i<size;i+=3)
  {
	  unsigned char *ptr =lpvBits+i;
	  unsigned char *r = ptr;
	  unsigned char *g = ptr+1;
	  unsigned char *b = ptr+2;

	  unsigned char red = *r;
	  unsigned char blue = *b;
	  *b = red;
	  *r = blue;
  }


  TRACE("%d %d\n",bmp.bmWidth, bmp.bmHeight);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, lpvBits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  bitmapDc -> SelectObject( oldBitMap );
  delete bitmapDc;
  delete[] lpvBits;
  delete bmpinfo;
}

void CCarEditorView::OnTexturemaps() 
{
	showTexture = !showTexture;
	//showGrey = !showTexture;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateTexturemaps(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showTexture);
}


void CCarEditorView::OnShowShadow() 
{
	showShadowBox = !showShadowBox;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowShadow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showShadowBox);
	
}

void CCarEditorView::OnShowLines() 
{
	showLines = !showLines;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowLines(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showLines);
}

void CCarEditorView::OnShowCulling() 
{
	showCulling = !showCulling;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowCulling(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showCulling);

}

void CCarEditorView::OnShowNormals() 
{
	showNormals = !showNormals;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateShowNormals(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showNormals);
}

void CCarEditorView::OnShowSingleColor() 
{
	showSingleColor = !showSingleColor;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);

}

void CCarEditorView::OnUpdateShowSingleColor(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showSingleColor);
}

void CCarEditorView::OnLighting() 
{
	showLighting = !showLighting;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnUpdateLighting(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showLighting);
}




#define CHECK_OBJECT(x) x = !x;CCarEditorDoc *pDoc = GetDocument();if (pDoc) pDoc->UpdateAllViews(NULL);
#define UPDATE_OBJECT(x) pCmdUI->SetCheck(x);



void CCarEditorView::OnShowCar() 
{
	CHECK_OBJECT(showCar);
}

void CCarEditorView::OnUpdateShowCar(CCmdUI* pCmdUI) 
{
	UPDATE_OBJECT(showCar);
}

void CCarEditorView::OnShowPolygonNumbers() 
{
	CHECK_OBJECT(showPolygonNumbers);
}

void CCarEditorView::OnUpdateShowPolygonNumbers(CCmdUI* pCmdUI) 
{
	UPDATE_OBJECT(showPolygonNumbers);
}

void CCarEditorView::OnShowPointnumbers() 
{
	CHECK_OBJECT(showPointNumbers);
}

void CCarEditorView::OnUpdateShowPointnumbers(CCmdUI* pCmdUI) 
{
	UPDATE_OBJECT(showPointNumbers);
}

void CCarEditorView::OnShowVertexNumbers() 
{
	CHECK_OBJECT(showVertexNumbers);
}

void CCarEditorView::OnUpdateShowVertexNumbers(CCmdUI* pCmdUI) 
{
	UPDATE_OBJECT(showVertexNumbers);
}

void CCarEditorView::OnShowCameraNumbers() 
{
	CHECK_OBJECT(showCameraNumbers);
}

void CCarEditorView::OnUpdateShowCameraNumbers(CCmdUI* pCmdUI) 
{
	UPDATE_OBJECT(showCameraNumbers);
}

void CCarEditorView::OnJamSet() 
{
	CJamSelect *dlg = new CJamSelect();

	int result = dlg->DoModal();

	if (result==IDOK)
	{
		DefaultJamFileName = dlg->m_JamFileName;
		UpdateJamFile();
		CCarEditorDoc *pDoc = GetDocument();
		if (pDoc) pDoc->UpdateAllViews(NULL);
	}


	delete dlg;	
}

#define PI 3.1415927

void CCarEditorView::getLookPoint()
{
	CCarEditorDoc *pDoc = GetDocument();
   if (currentView == t_Air)
   {
	
	if (pDoc->car){
		glTranslated(0,0,-dist);
	}
	else {
		glTranslated(0.0,0.0,-dist);
	}
	glRotated(m_xRotate, 1.0, 0.0, 0.0);
	glRotated(m_yRotate, 0.0, 1.0, 0.0);
	glRotated(m_zRotate, 0.0, 0.0, 1.0);	
   }
   else
   {
   CCarEditorDoc *pDoc = GetDocument();
   int idx;

   if (currentView >= t_Cam1 && currentView <= t_Cam9)
   {
     idx = currentView - t_Cam1;
   }
   else
   {
	   COpenGLView::getLookPoint();
	   return;
   }
   Camera *cam = &(pDoc->car->cameraData[idx]);

   if (cam == NULL)
   {
	   COpenGLView::getLookPoint();
	   AfxMessageBox("No Camera");
	   return;
   }
   //double eyeX = cam->pos.X;
   //double eyeY = cam->pos.Y;
   //double eyeZ = cam->pos.Z;
   double eyeX = cam->pos.X/25;
   double eyeY = cam->pos.Y/25;
   double eyeZ = cam->pos.Z/25;

   double ydiff = 100;
   double lookYDiff = ydiff*cos((cam->Direction*PI/180.0));
   double lookXDiff = ydiff*sin((cam->Direction*PI/180.0));

   TRACE("look %f %f\n",lookXDiff,lookYDiff);

   double lookX = (cam->pos.X+lookXDiff)/25;
   double lookY = (cam->pos.Y+lookYDiff)/25;
   double lookZ = cam->pos.Z/25;



   gluLookAt(eyeX,eyeY,(eyeZ),
			  lookX,lookY,(lookZ),
			  0.0f,0.0f,1.0f);

   //Rotated(cam->Direction,0.0f,0.0f,1.0f);
   //Rotated(cam->Roll,0.0f,1.0f,0.0f);
   //Rotated(cam->Pitch,1.0f,0.0f,0.0f);

   }
   
}

#define CHECK_VIEW(x) currentView = x;CCarEditorDoc *pDoc = GetDocument();if (pDoc) pDoc->UpdateAllViews(NULL);
#define UPDATE_VIEW(x) pCmdUI->SetCheck((currentView == x) ? TRUE : FALSE);

void CCarEditorView::OnCameraViewfromcamera1() 
{
	CHECK_VIEW(t_Cam1)
}

void CCarEditorView::OnCameraViewfromcamera2() 
{
	CHECK_VIEW(t_Cam2)
}

void CCarEditorView::OnCameraViewfromcamera3() 
{
	CHECK_VIEW(t_Cam3)
}

void CCarEditorView::OnCameraViewfromcamera4() 
{
	CHECK_VIEW(t_Cam4)
}

void CCarEditorView::OnCameraViewfromcamera5() 
{
	CHECK_VIEW(t_Cam5)
}

void CCarEditorView::OnCameraViewfromcamera6() 
{
	CHECK_VIEW(t_Cam6)
}

void CCarEditorView::OnCameraViewfromcamera7() 
{
	CHECK_VIEW(t_Cam7)
}

void CCarEditorView::OnCameraViewfromcamera8() 
{
	CHECK_VIEW(t_Cam8)
}

void CCarEditorView::OnCameraViewfromcamera9() 
{
	CHECK_VIEW(t_Cam9)
}

void CCarEditorView::OnUpdateCameraViewfromcamera1(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam1)
}

void CCarEditorView::OnUpdateCameraViewfromcamera2(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam2)
}

void CCarEditorView::OnUpdateCameraViewfromcamera3(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam3)
}

void CCarEditorView::OnUpdateCameraViewfromcamera4(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam4)
}

void CCarEditorView::OnUpdateCameraViewfromcamera5(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam5)

}

void CCarEditorView::OnUpdateCameraViewfromcamera6(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam6)
}

void CCarEditorView::OnUpdateCameraViewfromcamera7(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam7)
}

void CCarEditorView::OnUpdateCameraViewfromcamera8(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam8)
}

void CCarEditorView::OnUpdateCameraViewfromcamera9(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Cam9)
}

void CCarEditorView::OnNormalView() 
{
	CHECK_VIEW(t_Air)
}

void CCarEditorView::OnUpdateNormalView(CCmdUI* pCmdUI) 
{
	UPDATE_VIEW(t_Air)
}

void CCarEditorView::OnCameraViewrightside() 
{
	// top view
	//m_xRotate = -70;
	//m_yRotate = 0;
	//m_zRotate = 140;
	//CCarEditorDoc *pDoc = GetDocument();
	//if (pDoc) pDoc->UpdateAllViews(NULL);
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 270;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewleftside() 
{
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 90;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewfront() 
{
	m_xRotate = -90;
	m_yRotate = 0;
	m_zRotate = 180;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewtop() 
{
	// top view
	m_xRotate=0;
	m_yRotate=0;
	m_zRotate=-90;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewback() 
{
	m_xRotate = -90;
	m_yRotate = 0;
	m_zRotate = 0;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewbottom() 
{
	m_xRotate=180;
	m_yRotate=0;
	m_zRotate=90;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewfrontleft() 
{
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 135;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewfrontright() 
{
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 225;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewbackleft() 
{
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 45;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnCameraViewbackright() 
{
	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 315;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
}

void CCarEditorView::OnStartAnimation() 
{
	// TODO: Add your command handler code here
	if (!Animation)
	{
		SetTimer(0,((CCarEditorApp*)AfxGetApp())->m_AnimationSpeed,NULL);
		Animation = TRUE;
	}
	else
	{
		Animation = FALSE;
		KillTimer(0);
	}
}

void CCarEditorView::OnTimer(UINT nIDEvent) 
{
	if (Animation == FALSE)
	{
		
	}
	else
	{
		m_yRotate -= 1;
	    InvalidateRect(NULL,FALSE);
	}
	COpenGLView::OnTimer(nIDEvent);
}

void CCarEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CCarEditorDoc *pDoc = GetDocument();

	RedrawWindow();
	KillTimer(0);
	Animation = FALSE;
	COpenGLView::OnLButtonDown(nFlags, point);
}

void CCarEditorView::OnUpdateStartAnimation(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Animation);
}

void CCarEditorView::OnZoomOut() 
{
	dist = dist*1.2;
    InvalidateRect(NULL,FALSE);
}

void CCarEditorView::OnZoomIn() 
{
	dist = dist*0.8;
	InvalidateRect(NULL,FALSE);
}

int CCarEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ReReadJams();

	UpdateJamFile();
	return 0;
}


void CCarEditorView::OnShowNoseCone() 
{
	showNoseDetail = !showNoseDetail;

	InvalidateRect(NULL,FALSE);
}

void CCarEditorView::OnUpdateShowNoseCone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showNoseDetail);
	
}

void CCarEditorView::OnJamExportrcrjam() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("Current Not Implemented");
}

void CCarEditorView::OnShowObject() 
{
	// TODO: Add your command handler code here
	showObject = !showObject;
	InvalidateRect(NULL,FALSE);
}

void CCarEditorView::OnUpdateShowObject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showObject);
}



void CCarEditorView::OnJamDrawrcrjam() 
{
	((CCarEditorApp*)AfxGetApp())->drawRCRJam = !((CCarEditorApp*)AfxGetApp())->drawRCRJam;
	//ReReadJams();
	UpdateJamFile();
}

void CCarEditorView::OnUpdateJamDrawrcrjam(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(((CCarEditorApp*)AfxGetApp())->drawRCRJam);
}

void CCarEditorView::OnJamProducercrimages() 
{
	m_xRotate = -90;
	m_yRotate = 0;
	m_zRotate = 0;
	CCarEditorDoc *pDoc = GetDocument();
	if (pDoc) pDoc->UpdateAllViews(NULL);
	renderToDIB = TRUE;
	doubleBuffered = FALSE;
	//if (SetWindowPixelFormat(hDC)==FALSE)
	//{
	//	AfxMessageBox("OpenGL Error setting WindowPixel Format");
	//}
	//setupDIB(hDC);
	CRect rc;
	GetClientRect(&rc);
	
	
	if (pDoc) pDoc->UpdateAllViews(NULL);
	if (hBitmap != NULL)
	{
	CBitmap *cbmp = CBitmap::FromHandle(hBitmap);
	BMP *image = new BMP();	
	BITMAP bmp;
    cbmp->GetBitmap( &bmp );

	double aspect = bmp.bmWidth/bmp.bmHeight;
    double nHeight = 100;
	double nWidth = aspect*nHeight;
			 
	//HDC hdc = ::GetDC(m_hWnd);

	//StretchBlt(hdc, 0, 0, nWidth,nHeight, hDC, 0, 0, bmp.bmWidth,bmp.bmHeight,SRCCOPY);

	//image->Create(bmp.bmWidth,bmp.bmHeight,(unsigned char*)bmp.bmBits);
	//image->Save("testRCR.bmp");
	//renderToDIB = FALSE;	
	}
	else
	{
		AfxMessageBox("Cannot Grab Screen");
	}
}

char lpszFileOBJFilter[] = "WaveFront Object Files (*.obj)|*.obj|All Files (*.*)|*.*|";

//#define SCALEVAL(x) x/180.0

void CCarEditorView::OnFileExportwavefrontobj() 
{
	CCarEditorDoc *pDoc = GetDocument();

	 char fileName[256];

	 strcpy(fileName,"*.obj");
	 CFileDialog *fdlg = new CFileDialog(FALSE,"obj",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFileOBJFilter);

	 int result = fdlg->DoModal();

	 if (result==IDOK)
	 {
	   CString filename = fdlg->GetFileName();
	   FILE *fp = fopen(filename,"w");

	   //for(int k=0;k<pDoc->car->numVerticies;k++)
	   //{		   
		   //fprintf(fp,"v %f %f %f\n",SCALEVAL((float)pDoc->car->pointData[k].y),
			//   SCALEVAL((float)pDoc->car->pointData[k].z),
			//   SCALEVAL((float)pDoc->car->pointData[k].x));
	   //}

	   for(int i=0;i<pDoc->car->numVerticies;i++)
	   {
		 fprintf(fp,"v %d %d %d\n",
		 pDoc->car->vertexData[i].x,
		 pDoc->car->vertexData[i].y,
		 pDoc->car->vertexData[i].z);
	   }
	   for(int i=0;i<pDoc->car->numPoints/2;i++)
	   {
		fprintf(fp,"f %d %d %d\n",pDoc->car->pointData[i].x,
			pDoc->car->pointData[i].y,pDoc->car->pointData[i].x);
		}
	 }
}

	   /*
	   fprintf(fp,"usemtl R128G128B128\n");

	   for(int i=0;i<pDoc->car->textureSize;i++)
	   {
		  //int colorIdx = (i % numRandomColors);
		  //t_Color currentColor = RandomColors[colorIdx];
		  if (i == 186) continue;

		  TextureCmd *cmd =&(pDoc->car->textureData[i]);
		  if (cmd==NULL) continue;

		  if (cmd->numPts > 0)
		  {
			  fprintf(fp,"f ");
			  for(int j=0;j<cmd->numPts;j++)
			  {
				  int val = cmd->PtsList[j];
				  int idxF,idxT;
				  //double x,y,z;
				  int aval = abs(val);
				  if (aval > 0 && aval <= pDoc->car->numVerticies)
				  {
				   if (val > 0)
				   {
				    idxF = pDoc->car->vertexData[val-1].from;
		            idxT = pDoc->car->vertexData[val-1].to;					
				   }
				   else
				   {				   
				    idxF = pDoc->car->vertexData[aval-1].to;
		            idxT = pDoc->car->vertexData[aval-1].from;
				   }
				   fprintf(fp," %d",idxF+1);
				   //x = (double)pDoc->car->pointData[idxF].x;
		           //y = (double)pDoc->car->pointData[idxF].y;
		           //z = (double)pDoc->car->pointData[idxF].z;
				   // output x,y,z
				  }
			  }
			  fprintf(fp,"\n");
		}
	}
	   fclose(fp);
	}
	 
	
}
*/

void CCarEditorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CCarEditorDoc *pDoc = GetDocument();

	if (pDoc)
	{
		if (pDoc->car)
		{
			if (pDoc->car->selectedVertex){
				pDoc->car->selectedVertex->Open(((CCarEditorApp*)AfxGetApp())->m_MainTree);
				InvalidateRect(NULL,FALSE);
			}
		}
	}
	
	COpenGLView::OnLButtonDblClk(nFlags, point);
}

#include "RCREditor.h"

void CCarEditorView::OnEditRcrdistances() 
{
	CRCREditor *dlg = new CRCREditor();
	int result = dlg->DoModal();
	delete dlg;
}



void CCarEditorView::OnShowCockpit() 
{
	showCockpit = !showCockpit;

	InvalidateRect(NULL,FALSE);
}

void CCarEditorView::OnUpdateShowCockpit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showCockpit);
}

void CCarEditorView::OnShowSwheel() 
{
	showSWheel = !showSWheel;

	InvalidateRect(NULL,FALSE);
}

void CCarEditorView::OnUpdateShowSwheel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showSWheel);
}
