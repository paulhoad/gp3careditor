
#include "stdafx.h"

#include "Car.h"
#include "CarTree.h"

#include "resource.h"

#include "DataValue.h"
#include "VertexDataValue.h"
#include "Point3DDataValue.h"
#include "CamEdit.h"
#include "JAM.h"

extern char * getFullPathToJamFileName(char *filename);

static long GetLengthofFile(const char *name)
{
  FILE *fp;
  long i=0;
  unsigned char da[1];

  fp = fopen(name,"rb");
  if (fp == NULL) return -1;
  while(fread(&da,sizeof(unsigned char),1,fp)>0){
     i++;
  }
  fclose(fp);
  return i;
}

Car *ImportCar(CString exePath)
{
	FILE *fp = fopen(exePath,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return NULL;
	}

	int length = DEF_CAR_LENGTH;

	

	Car *mycar = new Car();
	mycar->setCarName("Import From Exe");

	mycar->wheel = new Car();
	mycar->wheel->setCarName("Wheel");

	mycar->helmet = new Car();
	mycar->helmet->setCarName("Helmet");

	mycar->cockpit = new Car();
	mycar->cockpit->setCarName("Cockpit");

	mycar->swheel = new Car();
	mycar->swheel->setCarName("Steering Wheel");

	//0x1599B0
	//0x14C4A8  - Car
	//0x15B160  -
	//0x15BD94  -

	//0x3A8CE   -
	
	//int start = DEF_CAR_START;
	/*
	start = 0x76293C; // car
	start = 0x78347C; // tyre 72
	start = 0x78A0F0; // safey car? 72
	start = 0x78B5F0; // advert 72
	start = 0x78B720; // ?
	start = 0x78B9D8; // ?
	start = 0x78FA8C; // ?
	
	start = 0x77965C; // cockpit 88
	start = 0x77F2CC; // advert?
	start = 0x77F4BC; // helment 78
	start = 0x781310; // box sign 66
	start = 0x789FC4; // advert 72
	
	*/

	int start = GP3_CAR_START; // car 94
	int tmp_offset = 94;

	if (!fseek(fp,start,0))
	{
	  fread(mycar->data,length,sizeof(char),fp);
	  mycar->parseCar(start,tmp_offset,0);
	}
	else
	{
	  AfxMessageBox("Failed to find Car Data",MB_OK);
	}

	// Load Wheel also
	start = 0x78347C; // car 94
	tmp_offset = 72;
	//start = 0x77F4BC; // car 94
	//tmp_offset = 78;

	if (!fseek(fp,start,0))
	{
	  fread(mycar->wheel->data,length,sizeof(char),fp);
	  mycar->wheel->parseCar(start,tmp_offset,0);
	}
	else
	{
	  AfxMessageBox("Failed to find Wheel Data",MB_OK);
	}

	// Load Wheel also
	start = 0x77F4BC; // car 94
	tmp_offset = 78;
	//start = 0x77F4BC; // car 94
	//tmp_offset = 78;

	if (!fseek(fp,start,0))
	{
	  fread(mycar->helmet->data,length,sizeof(char),fp);
	  mycar->helmet->parseCar(start,tmp_offset,0);
	}
	else
	{
	  AfxMessageBox("Failed to find Wheel Data",MB_OK);
	}

	// Load Wheel also
	start = 0x77965C; // car 94
	tmp_offset = 88;

	if (!fseek(fp,start,0))
	{
	  fread(mycar->cockpit->data,length,sizeof(char),fp);
	  mycar->cockpit->parseCar(start,tmp_offset,0);
	}
	else
	{
	  AfxMessageBox("Failed to find Wheel Data",MB_OK);
	}

	start = 0x77F2CC; // car 94
	tmp_offset = 72;

	if (!fseek(fp,start,0))
	{
	  fread(mycar->swheel->data,length,sizeof(char),fp);
	  mycar->swheel->parseCar(start,tmp_offset,0);
	}
	else
	{
	  AfxMessageBox("Failed to find Wheel Data",MB_OK);
	}

	/*
	Helmet *myhelmet = new Helmet();
	if (!fseek(fp,start,0))
	{
	  fread(mycar->data,length,sizeof(char),fp);
	  mycar->parseCar(start);
	}
	else
	{
	  AfxMessageBox("Failed to find Car Data",MB_OK);
	}

	LoadTextureCfg();
	LoadHTextureCfg();
	*/
	fclose(fp);
	return mycar;
}

void ImportCameras(Car *mycar,CString exePath)
{
	FILE *fp = fopen(exePath,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return;
	}

	//int length = 10*sizeof(t_IntCamera);

	//0x1599B0
	//0x14C4A8

	//if (!fseek(fp,0x13735A,0))	 
	// 0x0079e59e 
	if (!fseek(fp,0x79e59e,SEEK_SET))
	{
	  for(int i=0;i<9;i++)
	  {
	    fread(&(mycar->cameraData[i].pos),sizeof(t_IntCamera),sizeof(char),fp);
		mycar->cameraData[i].object = mycar;
		mycar->cameraData[i].parse();
		mycar->cameraData[i].loaded = TRUE;
	  }	  
	}
	else
	{
	  AfxMessageBox("Failed to find Camera Data",MB_OK);
	}

	fclose(fp);
}


void ExportCar(CString exePath,Car *mycar)
{
	FILE *fp = fopen(exePath,"rb+w");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return;
	}

	int length = DEF_CAR_LENGTH;

	int start = GP3_CAR_START; // car 94
	int tmp_offset = 94;

	mycar->exportCar(start,tmp_offset);

	unsigned char tempData[DEF_CAR_LENGTH];
	if (!fseek(fp,DEF_CAR_START,0))
	{
		fread(tempData,length,sizeof(char),fp);
	}

	if (!fseek(fp,DEF_CAR_START,0))
	{
	  for(int i=0;i<length;i++)
	  {
		  int data = tempData[i];

		  if (mycar->data[i] != data)
		  {
		  	  fseek(fp,DEF_CAR_START+i,0);
			  fputc(mycar->data[i],fp);
		  }
	  }
	}
	else
	{
	  AfxMessageBox("Failed to find Car Data",MB_OK);
	  return;
	}
	AfxMessageBox("Finished Patching GP3.EXE",MB_OK);


	fclose(fp);
}


Car *LoadCar(LPCSTR filename)
{
	int length = GetLengthofFile(filename);

	if (length!=-1)
	{
	FILE *fp = fopen(filename,"rb");

	if (fp== NULL)
	{
		AfxMessageBox("Canot open Car file",MB_OK);
		return NULL;
	}
	Car *mycar = new Car();
	mycar->setCarName((char*)filename);

	fread(mycar->data,length,sizeof(char),fp);

	mycar->parseCar(DEF_CAR_START,94,0);

	//LoadTextureCfg();
	//LoadHTextureCfg();

	fclose(fp);
	return mycar;
	}
	return NULL;
}

int Object3D::GetPCByte(int offset)
{
        int buffer = data[offset];
		return buffer;
}


int Object3D::GetPCWord(int offset)
{
        unsigned char buffer[2];

		buffer[0] = 0x000000FF & data[offset];
		buffer[1] = 0x000000FF & data[offset+1];
        int  PCword;
        PCword = buffer[1]*256 +
                 buffer[0];
        return 0x0000FFFF & PCword;
}

long Object3D::GetPCDWord(int offset)
{
        unsigned char tdata[4];
        long  PCword;

		tdata[0] = data[offset];
		tdata[1] = data[offset+1];
		tdata[2] = data[offset+2];
		tdata[3] = data[offset+3];

		//return (MAKELONG(MAKEWORD(data[2],tdata[3]),MAKEWORD(data[0],tdata[1])));
		PCword = tdata[3]*256*256*256 +
                 tdata[2]*256*256 +
                 tdata[1]*256 +
                 tdata[0];
        return (signed long) PCword;

}

void Object3D::WritePCDWord(int offset,int value)
{
	    char c1   = (value &  0x000000FF);
		char c2= ((value & 0x0000FF00)>>8);
		char c3 = ((value & 0x00FF0000)>>16);
		char c4 = ((value & 0xFF000000)>>24);

		data[offset] = c1;
		data[offset+1] = c2;
		data[offset+2] = c3;
		data[offset+3] = c4;
}


void Object3D::WritePCWord(int offset,int value)
{
	    data[offset]   = (value & 0x00FF);
		data[offset+1] = ((value &0xFF00)>>8);
}

void Car::write(CString filename)
{
	FILE *fp = fopen(filename,"wb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot open file for writing",MB_OK);
		return;
	}

	int length = DEF_CAR_LENGTH;

	fwrite(data,length,sizeof(char),fp);

	fclose(fp);
}

void Car::parseCar(int begin,int tmpOff,int dver)
{
	int offset = 0;
	header = (t_objheader*)malloc(sizeof(t_objheader));
	header->Magic         = GetPCWord(offset);offset+=2;
	header->id         = GetPCWord(offset);offset+=2;
	header->scaleBegin    = GetPCDWord(offset);offset+=4;
	header->scaleEnd      = GetPCDWord(offset);offset+=4;
	header->textureBegin  = GetPCDWord(offset);offset+=4;
	header->pointsBegin   = GetPCDWord(offset);offset+=4;
	header->vertexBegin   = GetPCDWord(offset);offset+=4;
	header->textureEnd    = GetPCDWord(offset);offset+=4;
	header->vertexEnd     = GetPCDWord(offset);offset+=4;
	header->fileEnd       = GetPCDWord(offset);offset+=4;
	header->fileEnd2      = GetPCDWord(offset);offset+=4;
	header->always0      = GetPCDWord(offset);offset+=4;
	header->always1      = GetPCDWord(offset);offset+=4;
	header->unk      = GetPCWord(offset);offset+=2;
	header->size      = GetPCWord(offset);offset+=2;
	header->size8      = GetPCWord(offset);offset+=2;
	//int fileEnd;
    //int fileEnd2;
    //int always0;
    //int always1;
    //short unk;
    //short size;
    //short size8;
	//header = (t_objheader*)&data;
	//memoryOffset = begin;

	if (header->Magic!= 0xffff8002 && header->Magic!= 0xffff8003)
	{
		char msg[256];
		wsprintf(msg,"Bad Magic Number in exe 0x%x\n\nThis means you probably have not yet decrypted the gp3.exe you need to do this before the car editor will work!, please download gp3decrypt.zip from a GP3 support site.",header->Magic);
		AfxMessageBox(msg,MB_OK);
		valid = FALSE;
		return;
	}

	valid = TRUE;

	//int start=106;
	int start=72;

	TRACE("Magic %x\n",header->Magic);
	TRACE("scaleBegin %x\n",header->scaleBegin);
	TRACE("scaleEnd %x\n",header->scaleEnd);
	TRACE("textureBegin %x\n",header->textureBegin);
	TRACE("textureEnd %x\n",header->textureEnd);
	TRACE("pointsBegin %x\n",header->pointsBegin);
	TRACE("fileEnd %x\n",header->fileEnd);
	TRACE("vertexEnd %x\n",header->vertexEnd);
	int diff = header->vertexBegin-header->pointsBegin;
	TRACE("pointsBegin %x %d\n#######\n",diff,diff);

	TRACE("scaleBegin=%x\n",start+(header->scaleBegin-header->scaleBegin));
	TRACE("scaleEnd=%x\n",start+(header->scaleEnd-header->scaleBegin));
	TRACE("textureBegin=%x\n",start+(header->textureBegin-header->scaleBegin));
	TRACE("textureEnd=%x\n",start+(header->textureEnd-header->scaleBegin));
	TRACE("pointsBegin=%x\n",start+(header->pointsBegin-header->scaleBegin));

	numVerticies = header->size8/8;
	//numScale  = (header->scaleEnd - header->scaleBegin)/2;
	//numPoints =	(header->vertexBegin - header->pointsBegin)/8;
	numUnks   = (header->fileEnd -header->vertexEnd)/2;

	TRACE("numVerticies=%d\n",numVerticies);

	//numVerticies = (header->vertexEnd - header->vertexBegin)/4;

	TRACE("numVerticies=%d\n",numVerticies);
	//TRACE("numScale=%d\n",numScale);
	TRACE("numPoints=%d\n",numPoints);

	numVerticies = 3000;
	//scaleData  = new Scale[numScale];
	//pointData  = new Point3D[numPoints];
	
	textureData = new TextureCmd[1000];
	//unkData = new DatVal[30000];

	//offset = header->pointsBegin-header->scaleBegin+72;

	offset = header->pointsBegin-header->scaleBegin;
	if (dver){
		numVerticies = (header->vertexBegin - header->pointsBegin)/8;
	}
	else {
		numVerticies = (header->vertexBegin - header->pointsBegin)/16;
	}
	//numPoints = (header->vertexEnd - header->vertexBegin)/8;
	//numPoints = (header->vertexEnd - header->vertexBegin)/16;
	numPoints = (header->vertexEnd - header->vertexBegin)/8;
	vertexData = new Vertex[numVerticies];
	pointData  = new Point3D[numPoints];
	// 78 for helmet
	int posP = begin+offset+tmpOff;
	offset += tmpOff;
	TRACE("Offset1 = %d 0x%x\n",offset,offset);
	for(int i=0;i<numVerticies;i++)
	{
		long X = -GetPCDWord(offset);offset+=4;
		long Y = GetPCDWord(offset);offset+=4;
		long Z = GetPCDWord(offset);offset+=4;
		long U = GetPCDWord(offset);offset+=4;

		vertexData[i].x = X;
		vertexData[i].y = Y;
		vertexData[i].z = Z;
		vertexData[i].u1 = U;
		vertexData[i].object = this;
	}
	int pos = begin+offset;
	offset+=8;
	//offset += 312;
	TRACE("Offset2 = %d 0x%x\n",offset,offset);
	for(int i=0;i<numPoints;i++)
	{
		long F = GetPCWord(offset);offset+=2;
		long T = GetPCWord(offset);offset+=2;
		long U1 = GetPCWord(offset);offset+=2;
		long U2 = GetPCWord(offset);offset+=2;

		pointData[i].x = F;
		pointData[i].y = T;
		pointData[i].z = U1;
		pointData[i].u1 = U2;
	}

	offset = header->textureBegin-header->scaleBegin;
	offset += tmpOff;
	int end   = (header->textureEnd-header->scaleBegin);
	end += tmpOff;
	//offset;
	TRACE("Offset3 = %d 0x%x\n",offset,offset);
	//int count = start+(header->textureBegin-header->scaleBegin);
	//int end   = start+(header->textureEnd-header->scaleBegin);
	int count = offset;
	TRACE("count = %d end=%d\n",count,end);
	int idx=0;
	int lastcmd = -1;
	int cmd;
	while(count < end)
	{
		if (idx > 1000){
			break;
		}
		int numl = GetPCWord(count);count+=2;
		if (numl != lastcmd+1){
			TRACE("Gone wrong on previous cmd 0x%x 0x%x\n",cmd,count);
			break;
		}
		lastcmd = numl;
		//int numh = GetPCByte(count++);
		cmd  = GetPCWord(count);count+=2;
		textureData[idx].numl = numl;
		//textureData[idx].numh = numh;
		textureData[idx].cmd = cmd;
		textureData[idx].object = this;

		int nArgs=0;

		switch(cmd)
		{
		   case 0x90:
		   case 0x80:
			   textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			   textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			   textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			   textureData[idx].numArgs = nArgs;
			   break;
			   /*
		  case 0x16:
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  //textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  // should be zero to detect end of polygon
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].numArgs = nArgs;
			  break;
			  */
		  case 0x10:
		  case 0x11:
		  case 0x12:
		  case 0x13:
		  case 0x16:
		  case 0x17:
          case 0x1a:
		  case 0x1b:
		  case 0x0:
		  case 0x1:
		  case 0x2:
		  case 0x3:
		  case 0x0a:
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  while(!textureData[idx].Args[nArgs-1]==0)
			  {
			   textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  }
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0x4:
			  textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;//1
			  while(!textureData[idx].Args[nArgs-1]==0)
			  {
			   textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  }
			  //textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;//2
			  //textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;//3
			  //textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;//4
			  // should be zero to detect end of polygon
			  //textureData[idx].Args[nArgs++] = GetPCWord(count);count+=2;
			  textureData[idx].numArgs = nArgs;
			  break;
		  /*
		  case 0x90:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  
		  
		  case 0x13:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  if (textureData[idx].Args[3] == 0x80)
			  {
				textureData[idx].Args[nArgs++] = GetPCByte(count++);
			    textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  }
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0x18:
		  case 0x11:
		  case 0x1a:
		  case 0x16:
		  case 0x17:		 
		  
		  case 0x15:
		  //case 0xa:
		  case 0x10:
		  case 0x0:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//1
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//2
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//3
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//4
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//5
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);//6
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  */
		  default:
			  TRACE("Unknown Command 0x%x\n",cmd);break;
		  /*
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
			  */
		}

		textureData[idx].parse();
		idx++;
		textureSize++;
	}
	TRACE("%d %d\n",count,end);
	updateData(TRUE);
}

void Car::exportCar(int begin,int tmpOff)
{
	int offset = 0;
	int start=72;

	int diff = header->vertexBegin-header->pointsBegin;
	numVerticies = header->size8/16;
	offset = header->pointsBegin-header->scaleBegin;
	numVerticies = (header->vertexBegin - header->pointsBegin)/16;
	numPoints = (header->vertexEnd - header->vertexBegin)/16;

	int posP = offset+tmpOff;
	offset += tmpOff;

	TRACE("Offset1 = %d\n",offset);
	for(int i=0;i<numVerticies;i++)
	{
		//long X = GetPCDWord(offset);offset+=4;
		//long Y = GetPCDWord(offset);offset+=4;
		//long Z = GetPCDWord(offset);offset+=4;
		//long U = GetPCDWord(offset);offset+=4;

		//vertexData[i].x = X;
		//vertexData[i].y = Y;
		//vertexData[i].z = Z;
		WritePCDWord(offset,-vertexData[i].x);offset+=4;
		WritePCDWord(offset,vertexData[i].y);offset+=4;
		WritePCDWord(offset,vertexData[i].z);offset+=4;
		WritePCDWord(offset,vertexData[i].u1);offset+=4;
	}
	int pos = begin+offset;
	offset+=8;
	TRACE("Offset2 = %d\n",offset);
	for(int i=0;i<numPoints;i++)
	{
		//long F = GetPCWord(offset);offset+=2;
		//long T = GetPCWord(offset);offset+=2;
		//long U1 = GetPCWord(offset);offset+=2;
		//long U2 = GetPCWord(offset);offset+=2;

		//pointData[i].x = F;
		//pointData[i].y = T;
		//pointData[i].z = 0;
		WritePCWord(offset,pointData[i].x);offset+=2;
		WritePCWord(offset,pointData[i].y);offset+=2;
		WritePCWord(offset,pointData[i].z);offset+=2;
		WritePCWord(offset,pointData[i].u1);offset+=2;
	}

	//updateData(TRUE);
}



void Car::updateData(BOOL updateTexture)
{
	for(int i=0;i<numVerticies;i++)
	{
		//int offset = start+(header->vertexBegin-header->scaleBegin)+(i*4);
		//if (memoryOffset == DEF_CAR_START)
	    //{
		//  offset = 0x1d42+(i*4);
		//}
		//vertexData[i].x = 10;
		//vertexData[i].y = 10;
		//vertexData[i].z = 10;
		//vertexData[i].offset = offset;
		//vertexData[i].object = this;
		//int from = GetPCByte(offset);
		//int to   = GetPCByte(offset+1);
		//int A    = GetPCByte(offset+2);
		//int B    = GetPCByte(offset+3);
		//TRACE("Vertex %x %d = %d %d (%d %d)\n",offset,i,from,to,A,B);
		//vertexData[i].from = from;
		//vertexData[i].to = to;
	}
	/*
	int start=78;

	if (memoryOffset == DEF_CAR_START)
	{
		start = 106;
	}

	for(int i=0;i<numScale;i++)
	{
		int offset = start+(i*2);
		int val = GetPCWord(offset);
		//TRACE("Scale %d = %d\n",i,val);

		if (val > 0x8000) val = -(0xFFFF-val);
		scaleData[i].val    = val;
		scaleData[i].offset = offset;
		scaleData[i].object = this;
	}



	for(int i=0;i<numVerticies;i++)
	{
		int offset = start+(header->vertexBegin-header->scaleBegin)+(i*4);
		if (memoryOffset == DEF_CAR_START)
	    {
		  offset = 0x1d42+(i*4);
		}
		vertexData[i].offset = offset;
		vertexData[i].object = this;
		int from = GetPCByte(offset);
		int to   = GetPCByte(offset+1);
		int A    = GetPCByte(offset+2);
		int B    = GetPCByte(offset+3);
		//TRACE("Vertex %x %d = %d %d (%d %d)\n",offset,i,from,to,A,B);
		vertexData[i].from = from;
		vertexData[i].to = to;
	}

	for(int i=0;i<numUnks;i++)
	{
	   int offset = start+(header->vertexEnd-header->scaleBegin)+(i*2);
	   int data   = (int)GetPCWord(offset);	
	   unkData[i].data = data;
	}

	 for(int j=0;j<2;j++)
	 {
	 for(int i=0;i<numPoints;i++)
	 {
		int offset = start+(header->pointsBegin-header->scaleBegin)+(i*8);
		//int offset = 0x1126+(i*8);
		pointData[i].offset = offset;
		pointData[i].object = this;
		int x   = (int)GetPCWord(offset);
		int y   = (int)GetPCWord(offset+2);
		int z   = (int)GetPCWord(offset+4);
		int u   = (int)GetPCWord(offset+6);

		pointData[i].ox = x;
		pointData[i].oy = y;

		if (z > 0x8000) z = -(0x10000-z);
		//if (z > 0x8000) z = -(0xFFFF-z);

	

		if (x < 0x8000)
		{	
		 if (x > 0x80 && x < 0xFF)
		 {
			int idx = (x-0x84)/4;
			if (HiNose) idx+=OFFSET_NOSE;
			//scaleData[idx].depend[scaleData[idx].ptsDep++] = i;
			x = scaleData[idx].val;
			x = -x;
		 }
		 else if (x > 0)
		 {
		   int idx = (x-0x4)/4;
		   if (HiNose) idx+=OFFSET_NOSE;
		   //scaleData[idx].depend[scaleData[idx].ptsDep++] = i;
		   x = scaleData[idx].val;
		 }
		
		 if (y > 0x80 && y < 0xFF)
		 {
			int idx = (y-0x84)/4;
			if (HiNose) idx+=OFFSET_NOSE;
			//scaleData[idx].depend[scaleData[idx].ptsDep++] = i;
			y = scaleData[idx].val;
			y = -y;
		 }
		 else if (y > 0)
		 {
			int idx = (y-0x04)/4;
			if (HiNose) idx+=OFFSET_NOSE;
			y = scaleData[idx].val;
		
		 }
		}
		else
		{
		   int pidx = x - 0x8000;
		   x = pointData[pidx].x;
		   y = pointData[pidx].y;
		}

		pointData[i].x = x;
		pointData[i].y = y;
		pointData[i].z = z;

		switch(i)
		{
		   case 63 :  pointData[i].setDesc("Helmet Position");
					  break;
		   case 42 :  pointData[i].setDesc("Tyre LF Position");
					  break;
		   case 43 :  pointData[i].setDesc("Tyre LR Position");
					  break;
		   case 44 :  pointData[i].setDesc("Tyre RF Position");
					  break;
		   case 45 :  pointData[i].setDesc("Tyre RR Position");
					  break;
		}
	  }
	 }


	if (updateTexture)
	{
	int count = start+(header->textureBegin-header->scaleBegin);
	int end   = start+(header->textureEnd-header->scaleBegin);

	//textureSize = end-count;

	int idx=0;
	

	while(count < end)
	{
		int numl = GetPCByte(count++);
		int numh = GetPCByte(count++);
		int cmd  = GetPCByte(count++);
		textureData[idx].numl = numl;
		textureData[idx].numh = numh;
		textureData[idx].cmd = cmd;
		textureData[idx].object = this;

		int nArgs=0;

		switch(cmd)
		{
		   case 0x80:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  case 0x90:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  
		  
		  case 0x13:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  if (textureData[idx].Args[3] == 0x80)
			  {
				textureData[idx].Args[nArgs++] = GetPCByte(count++);
			    textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  }
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0x18:
		  case 0x11:
		  case 0x1a:
		  case 0x16:
		  case 0x17:		 
		  case 0x12:
		  case 0x15:
		  //case 0xa:
		  case 0x10:
		  case 0x0:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  /*
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		}

		textureData[idx].parse();
		idx++;
		textureSize++;
	  }
    }
	*/
}




void Car::LoadTree(CCarTree *tree,CDocument *pDoc)
{
	char buffer[256];
	wsprintf(buffer,"Car:%s",getCarName());
	carNode = tree->insertInfoNode(tree->m_CarTree.GetRootItem(),buffer,TO_ID(IDB_CAR));

	wsprintf(buffer,"Verticies %d",numVerticies);
	HTREEITEM vertex = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(int i=0;i<numVerticies;i++)
	{
	  
	   sprintf(buffer,"p[%d] %d %d %d",i,vertexData[i].x,vertexData[i].y,vertexData[i].z);
	   HTREEITEM verItem = tree->insertInfoNode(vertex,buffer,TO_ID(IDB_VERTICIES));
	   tree->m_CarTree.SetItemData(verItem,(DWORD)&vertexData[i]);
	   vertexData[i].node = verItem;
	   vertexData[i].idx = i;
	}

	wsprintf(buffer,"Connections %d",numPoints);
	HTREEITEM pts = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POINT));
	for(int i=0;i<numPoints;i++)
	{
	   wsprintf(buffer,"v[%d] %d-> %d ",i+1,pointData[i].x,pointData[i].y);
	   HTREEITEM ptItem = tree->insertInfoNode(pts,buffer,TO_ID(IDB_POINT));
	   tree->m_CarTree.SetItemData(ptItem,(DWORD)&pointData[i]);
	   pointData[i].node = ptItem;
	   pointData[i].idx = i;
	}

	
	/*
	wsprintf(buffer,"ScaleData %d",numScale);
	HTREEITEM scalep = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_SCALE));
	for(int i=0;i<numScale;i++)
	{
	  
	   if (i<31)
	   {
	     wsprintf(buffer,"s[%d] %d Low Nose Car",i,scaleData[i].val);
	   }
	   else
	   {
		 wsprintf(buffer,"s[%d] %d High Nose Car",i,scaleData[i].val);
	   }
	   HTREEITEM scaleItem = tree->insertInfoNode(scalep,buffer,TO_ID(IDB_SCALE));
	   scaleData[i].node = scaleItem;
	   scaleData[i].idx = i;
	   tree->m_CarTree.SetItemData(scaleItem,(DWORD)&scaleData[i]);
	}
	*/

	wsprintf(buffer,"TextureData %d",textureSize);
	HTREEITEM textureP = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POLYGONS));

	for(int i=0; i< textureSize;i++)
	{
		char argsStr[1024];

		strcpy(argsStr,"Args=");

		for(int j=0;j<textureData[i].numArgs;j++)
		{
			char msg[20];
			signed short da =  textureData[i].Args[j];
			wsprintf(msg,"%d ",da);
			strcat(argsStr,msg);
		}

		wsprintf(buffer,"[%d] %d (0x%x) %s",i,textureData[i].numl,textureData[i].cmd,argsStr);

		int icon = TO_ID(IDB_POLYGONS);

		if (isTextureCfgPolygon(i))
		{
		   icon = TO_ID(IDB_NO_DEF);
		}
		else if (isTextureCfgDefined(i))
		{
		  icon = TO_ID(IDB_POLYGON_DEF);
		}
		
		HTREEITEM textItem = tree->insertInfoNode(textureP,buffer,icon);
		HTREEITEM textureItem = textureData[i].LoadTree(tree,textItem);
		textureData[i].node = textItem;
		textureData[i].idx = i;
		tree->m_CarTree.SetItemData(textItem,(DWORD)&textureData[i]);
	}
	
	/*

	wsprintf(buffer,"UnkData %d",numUnks);
	HTREEITEM unkp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(int i=0;i<numUnks;i+=8)
	{
	  
	   wsprintf(buffer,"u[%d] %d %d %d %d %d %d %d %d",i,
				unkData[i].data,
				unkData[i+1].data,
				unkData[i+2].data,
				unkData[i+3].data,
				unkData[i+4].data,
				unkData[i+5].data,
				unkData[i+6].data,
				unkData[i+7].data);
	   HTREEITEM verItem = tree->insertInfoNode(unkp,buffer,TO_ID(IDB_VERTICIES));
	   //tree->m_CarTree.SetItemData(verItem,(DWORD)&vertexData[i]);
	   unkData[i].node = verItem;
	   unkData[i].idx = i;
	}
	*/

	
	wsprintf(buffer,"Cameras %d",9);
	HTREEITEM camp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_CAMERA));
	for(int i=0;i<9;i++)
	{
	  
	   wsprintf(buffer,"c[%d] (%d,%d,%d) (D=%d P=%d R=%d) (%d %d) (%d %d)",i+1,cameraData[i].pos.X,
		   cameraData[i].pos.Y,cameraData[i].pos.Z,cameraData[i].Direction,cameraData[i].Pitch,cameraData[i].Roll,
		   cameraData[i].pos.Detail1,cameraData[i].pos.Detail2,cameraData[i].pos.Unk1,cameraData[i].pos.Unk2);
	   HTREEITEM verItem = tree->insertInfoNode(camp,buffer,TO_ID(IDB_CAMERA));
	   tree->m_CarTree.SetItemData(verItem,(DWORD)&cameraData[i]);
	   cameraData[i].node = verItem;
	   cameraData[i].idx = i;
	}

	/*
	wsprintf(buffer,"RCR Jams 1-6");
	HTREEITEM jamp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_JAM));
	for(int i=0;i<7;i++)
	{
	   switch(i)
	   {
	      case 0:wsprintf(buffer,"gamejams\\rcr1.jam");	break;
		  case 1:wsprintf(buffer,"gamejams\\rcr2.jam");break;
		  case 2:wsprintf(buffer,"gamejams\\rcr2a.jam");break;
		  case 3:wsprintf(buffer,"gamejams\\rcr2b.jam");break;
		  case 4:wsprintf(buffer,"gamejams\\rcr3.jam");break;
		  case 5:wsprintf(buffer,"gamejams\\rcr4.jam");break;
		  case 6:wsprintf(buffer,"gamejams\\rcr5.jam");break;
	   }
	   jamData[i].setFileName(buffer);
	   jamData[i].pDoc = pDoc;
	   jamData[i].JamFileStructure = new JAM(getFullPathToJamFileName(buffer),TRUE);
	   jamData[i].JamFileStructure->OpenToRead(TRUE);
	   HTREEITEM verItem = tree->insertInfoNode(jamp,buffer,TO_ID(IDB_JAM));	   
	   tree->m_CarTree.SetItemData(verItem,(DWORD)&jamData[i]);
	   jamData[i].JamFileStructure->LoadTree(tree,verItem,FALSE);
	}
	*/
	/*
	wsprintf(buffer,"Car Jams");
	HTREEITEM jamcp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_JAM));
	for(int i=0;i<18;i++)
	{
	   switch(i)
	   {
		  case 0:wsprintf(buffer,"gamejams\\b1ftr.jam");	break;		  
		  case 1:wsprintf(buffer,"gamejams\\b2ftr.jam");	break;		  
		  case 2:wsprintf(buffer,"gamejams\\b1rtr.jam");	break;		  
		  case 3:wsprintf(buffer,"gamejams\\b2rtr.jam");	break;		  
	      case 4:wsprintf(buffer,"gamejams\\ferrari.jam");	break;		  
		  case 5:wsprintf(buffer,"gamejams\\benetton.jam");	break;		  
		  case 6:wsprintf(buffer,"gamejams\\williams.jam");	break;		  
		  case 7:wsprintf(buffer,"gamejams\\jordan.jam");	break;		  
		  case 8:wsprintf(buffer,"gamejams\\mclaren.jam");	break;		  
		  case 9:wsprintf(buffer,"gamejams\\ligier.jam");	break;		  
		  case 10:wsprintf(buffer,"gamejams\\sauber.jam");	break;		  
		  case 11:wsprintf(buffer,"gamejams\\simtek.jam");	break;		  
		  case 12:wsprintf(buffer,"gamejams\\minardi.jam");	break;		  
		  case 13:wsprintf(buffer,"gamejams\\footwork.jam");	break;		  
		  case 14:wsprintf(buffer,"gamejams\\tyrrell.jam");	break;		  
		  case 15:wsprintf(buffer,"gamejams\\larrous.jam");	break;		  
		  case 16:wsprintf(buffer,"gamejams\\pacific.jam");	break;		  
		  case 17:wsprintf(buffer,"gamejams\\lotus.jam");	break;		  

	
	   }
	   jamCarData[i].setFileName(buffer);
	   jamCarData[i].pDoc = pDoc;
	   jamCarData[i].JamFileStructure = new JAM(getFullPathToJamFileName(buffer),FALSE);
	   jamCarData[i].JamFileStructure->OpenToRead(TRUE);
	   HTREEITEM verItem = tree->insertInfoNode(jamcp,buffer,TO_ID(IDB_JAM));	   
	   tree->m_CarTree.SetItemData(verItem,(DWORD)&jamCarData[i]);
	   jamCarData[i].JamFileStructure->LoadTree(tree,verItem,FALSE);
	}
	*/
}

void Observer::Open(CCarTree *tree)
{
	//AfxMessageBox("Default Observer Open");
}

void Scale::Open(CCarTree *tree)
{
	CDataValue *dlg = new CDataValue();

	dlg->m_DataVal = val;
	int result = dlg->DoModal();

	if (result!=IDCANCEL)
	{
		//update model
		val = dlg->m_DataVal;
		setChanged();
		updateLabel(tree);
	}
}

void Car::PatchData()
{
	for(int i=0;i<numScale;i++)
	{
		if (scaleData[i].isChanged())
		{
		  scaleData[i].patch();
		}
	}
	for(int i=0;i<numVerticies;i++)
	{
		if (vertexData[i].isChanged())
		{
		  vertexData[i].patch();
		}
	}
	for(int i=0;i<numPoints;i++)
	{
		if (pointData[i].isChanged())
		{
		  pointData[i].patch();
		}
	}
}

void Scale::patch()
{
	    int high = val/256;
		int low  = val%256;

		object->data[offset]=low;
		object->data[offset+1]=high;
}


void Scale::updateLabel(CCarTree *tree)
{
	   char buffer[256];

	   if (idx<31)
	   {
	     wsprintf(buffer,"s[%d] %d First Car",idx,val);
	   }
	   else
	   {
		 wsprintf(buffer,"s[%d] %d Second Car",idx,val);
	   }

	   tree->m_CarTree.SetItemText(node,buffer);
}

HTREEITEM TextureCmd::LoadTree(CCarTree *tree,HTREEITEM node)
{
	    char argsStr[256];
		char buffer[256];

		strcpy(argsStr,"Verticies=");

		for(int j=0;j<numPts;j++)
		{
			if (numPts > 20) break;
			char msg[20];
			wsprintf(msg,"%d ",PtsList[j]);
			strcat(argsStr,msg);
		}

		wsprintf(buffer,"%s",argsStr);
		HTREEITEM scaleItem = tree->insertInfoNode(node,buffer,TO_ID(IDB_POLYGONS));
		return scaleItem;
}

void TextureCmd::parse()
{
	if (cmd >= 0x0 && cmd < 0x7F)
	{
		int startArg = 2;

		switch(cmd)
		{
		   case 0x4:
		   case 0x3:
		   case 0x2:
		   case 0x1:
		   case 0xa:
		   
		   case 0x0: startArg = 0;break;

		   case 0x17:
		   case 0x16:
		   case 0x13:
		   case 0x10: if (Args[1] == 0x80){
						 startArg = 4;break;
					  }
					  else {
						 startArg = 2;break;
					  }
		   case 0x11: startArg = 2;break;
		   case 0x12: startArg = 2;break;
		   default:
				TRACE("Unknown Cmd 2 0x%x\n",cmd);break;
		}
		//make pts list
		for(int i=startArg;i<numArgs;i+=1)
		{
			int val=Args[i];
			if (val!=0)
			{
			 PtsList[numPts++] = val;
			}
		}
	}
}

void Vertex::Open(CCarTree *tree)
{
	CPoint3DDataValue *dlg = new CPoint3DDataValue(object);

	dlg->m_X = x;
	dlg->m_Y = y;
	dlg->m_Z = z;
	int result = dlg->DoModal();

	if (result!=IDCANCEL)
	{
		//update model
		//from = dlg->m_From;
		//to = dlg->m_To;
		x = dlg->m_X;
		y = dlg->m_Y;
		z = dlg->m_Z;

		setChanged();
		updateLabel(tree);
	}
}

void Vertex::updateLabel(CCarTree *tree)
{
	   char buffer[256];

	   wsprintf(buffer,"v[%d] %d %d %d (u1=%d)",idx,x,y,z,u1);
	   tree->m_CarTree.SetItemText(node,buffer);
}

void Vertex::patch()
{
	    object->data[offset]=from;
		object->data[offset+1]=to;
}


void Point3D::Open(CCarTree *tree)
{
	CPoint3DDataValue *dlg = new CPoint3DDataValue(object);

	dlg->m_X = x;
	dlg->m_Y = y;
	dlg->m_Z = z;
	
	int result = dlg->DoModal();

	if (result!=IDCANCEL)
	{
		//update model
		x = dlg->m_X;
		y = dlg->m_Y;
		z = dlg->m_Z;
		setChanged();
		updateLabel(tree);
	}
}

void Point3D::updateLabel(CCarTree *tree)
{
	   char buffer[256];

	   wsprintf(buffer,"v[%d] %d %d  (u1=%d u2=%d)",idx+1,x,y,z,u1);
	   tree->m_CarTree.SetItemText(node,buffer);
}

void Point3D::patch()
{
	    object->data[offset]=LOBYTE(ox);
		object->data[offset+1]=HIBYTE(ox);
		object->data[offset+2]=LOBYTE(oy);
		object->data[offset+3]=HIBYTE(oy);
		object->data[offset+4]=LOBYTE(z);
		object->data[offset+5]=HIBYTE(z);
}

void Observer::Select(CCarTree *tree)
{
	//AfxMessageBox("No Select Call",MB_OK);
}


void Point3D::Select(CCarTree *tree)
{
	object->selectedPoint = this;
}

void Camera::Select(CCarTree *tree)
{
	((Car*)(object))->selectedCamera = this;
}

void Vertex::Select(CCarTree *tree)
{
	object->selectedVertex = this;
}

void TextureCmd::Select(CCarTree *tree)
{
	//AfxMessageBox("Selcted Texture");
	object->selectedTextureCmd = this;
}

#include <gl\gl.h>
#include <gl\glu.h>

extern int myGLPushMatrix();
extern int myGLPpopMatrix();

Vertex* Object3D::SelectVertex(HDC hdc,CScrollView *scview,double scrx,double scry,int idx)
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
		

		RECT rect;
		scview->GetClientRect(&rect);

		

		double minDist = 10E8;
		int minidx=-1;
		for(int i=0;i<numVerticies;i++)
		{
			GLdouble wx = vertexData[i].x;
			GLdouble wy = vertexData[i].y;
			GLdouble wz = vertexData[i].z;
			gluProject(wx,wy,wz,model,projection,view,&sx,&sy,&sz);
			double midy = (rect.top+rect.bottom)/2;
			double midx = (rect.left+rect.right)/2;

			double winx = (int)(sx-midx);
			double winy = (int)(sy-midy);
		
			double xpos = midx+winx;
			double ypos = midy-winy;
			//double xpos = sx;
			//double ypos = -sy;

			double dx = scrx-xpos;
			double dy = scry-ypos;

			/*
			::MoveToEx(hdc,xpos-5,ypos-5,NULL);
			::LineTo(hdc,xpos+5,ypos+5);
			::MoveToEx(hdc,xpos-5,ypos+5,NULL);
			::LineTo(hdc,xpos+5,ypos-5);
			*/

			double scrDist = sqrt((dx*dx)+(dy*dy));

			if (scrDist < 20 && scrDist < minDist)
			{
				minDist = scrDist;
				minidx= i;
			}
		}
		
		myGLPpopMatrix();

		if (minidx==-1) return NULL;
		return &vertexData[minidx];
}


void Camera::parse()
{
	Direction = (short)(pos.ODirection/182);
	Pitch = (short)(pos.OPitch/182);
	Roll = (short)(pos.ORoll/182);
}

void Camera::Open(CCarTree *tree)
{
	CCamEdit *dlg = new CCamEdit();

	dlg->m_X = pos.X;
	dlg->m_Y = pos.Y;
	dlg->m_Z = pos.Z;
	dlg->m_Dir = Direction;
	dlg->m_Pitch = Pitch;
	dlg->m_Roll = Roll;
	dlg->m_Detail1 = pos.Detail1;
	dlg->m_Detail2 = pos.Detail2;
	dlg->m_Unk1 = pos.Unk1;
	dlg->m_Unk2 = pos.Unk2;
	int result = dlg->DoModal();

	if (result!=IDCANCEL)
	{
		//update model
		pos.X = dlg->m_X;
		pos.Y = dlg->m_Y;
		pos.Z = dlg->m_Z;
		pos.ODirection = dlg->m_Dir*182;
		pos.OPitch     = dlg->m_Pitch*182;
		pos.ORoll      = dlg->m_Roll*182;
		pos.Detail1    = dlg->m_Detail1;
		pos.Detail2    = dlg->m_Detail2;
		pos.Unk1 = dlg->m_Unk1;
		pos.Unk2 = dlg->m_Unk2;
		setChanged();
		parse();
		updateLabel(tree);
	}
}

void Camera::updateLabel(CCarTree *tree)
{
	   char buffer[256];

	   wsprintf(buffer,"c[%d] (%d,%d,%d) (D=%d P=%d R=%d) (%d %d)",idx+1,pos.X,
		   pos.Y,pos.Z,Direction,Pitch,Roll,pos.Detail1,pos.Detail2);
	   tree->m_CarTree.SetItemText(node,buffer);
}

void ExportCameras(CString exePath,Car *mycar)
{
	FILE *fp = fopen(exePath,"rb+w");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return;
	}

	//if (!fseek(fp,0x13735A,0))	
	if (!fseek(fp,0x79e59e,0))
	{
	  for(int i=0;i<9;i++)
	  {
		if (mycar->cameraData[i].loaded == TRUE)
		{
	    fwrite(&(mycar->cameraData[i].pos),sizeof(t_IntCamera),sizeof(char),fp);
		}
	  }	  
	}
	else
	{
	  AfxMessageBox("Failed to find Camera Data",MB_OK);
	  return;
	}
	fclose(fp);
}

void DatVal::Open(CCarTree *tree)
{
	CDataValue *dlg = new CDataValue();

	dlg->m_DataVal = data;
	int result = dlg->DoModal();

	if (result!=IDCANCEL)
	{
		//update model
		data = dlg->m_DataVal;
		setChanged();
		//updateLabel(tree);
	}
}

double Vertex::Length()
{
	    if (from < object->numPoints && to < object->numPoints)
		{
		Point3D *ptA = &object->pointData[from];
		Point3D *ptB = &object->pointData[to];
		return ptA->distance(ptB);
		}
		return -1;
}

double Vertex::LengthCM()
{
		return Length()*0.48076;
}

void ExportHelmet(CString exePath,Helmet *myhelmet)
{
	FILE *fp = fopen(exePath,"rb+w");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return;
	}

	int length = DEF_HELMET_LENGTH;

	unsigned char tempData[DEF_HELMET_LENGTH];
	if (!fseek(fp,DEF_HELMET_START,0))
	{
		fread(tempData,length,sizeof(char),fp);
	}

	if (!fseek(fp,DEF_HELMET_START,0))
	{
	  //fread(mycar->cardata,length,sizeof(char),fp);
	  //mycar->parseCar();
	  for(int i=0;i<length;i++)
	  {
		  //fseek(fp,DEF_CAR_START+i,0);
		  //
		  //int data = fgetc(fp);
		  int data = tempData[i];

		  if (myhelmet->data[i] != data)
		  {
		  	  TRACE("%d Patching Data from %d to %d\n",i,myhelmet->data[i],data);
			  fseek(fp,DEF_HELMET_START+i,0);
			  fputc(myhelmet->data[i],fp);
		  }
	  }
	  //fwrite(mycar->cardata,length,sizeof(char),fp);
	}
	else
	{
	  AfxMessageBox("Failed to find Car Data",MB_OK);
	  return;
	}

	AfxMessageBox("Finished Patching GP3.EXE",MB_OK);

	fclose(fp);
}

Helmet *ImportHelmet(CString exePath)
{
	FILE *fp = fopen(exePath,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot find GP3.EXE",MB_OK);
		return NULL;
	}

	int length = DEF_HELMET_LENGTH;

	//0x1599B0  - Helmet
	//0x14C4A8  - Car
	//0x15B160  -
	//0x15BD94  -

	
	Helmet *myhelmet = new Helmet();
	int start = DEF_HELMET_START;

	if (!fseek(fp,start,0))
	{
	  fread(myhelmet->data,length,sizeof(char),fp);
	  myhelmet->parse(start);
	}
	else
	{
	  AfxMessageBox("Failed to find Helmet Data",MB_OK);
	}


	fclose(fp);
	return myhelmet;
}

void Helmet::parse(int begin)
{
	header = (t_objheader*)&data;
	memoryOffset = begin;

	if (header->Magic!= 0xffff8002)
	{
		char msg[256];
		wsprintf(msg,"Bad Magic Number in exe 0x%x",header->Magic);
		AfxMessageBox(msg,MB_OK);
		valid = FALSE;
		return;
	}

	valid = TRUE;

	int start=106;

	TRACE("Magic %x\n",header->Magic);
	//TRACE("Id %x\n",header->id);
	TRACE("scaleBegin %x\n",header->scaleBegin);
	TRACE("scaleEnd %x\n",header->scaleEnd);
	TRACE("textureBegin %x\n",header->textureBegin);
	TRACE("textureEnd %x\n",header->textureEnd);
	TRACE("pointsBegin %x\n",header->pointsBegin);
	TRACE("fileEnd %x\n",header->fileEnd);
	TRACE("vertexEnd %x\n",header->vertexEnd);
	int diff = header->vertexBegin-header->pointsBegin;
	TRACE("pointsBegin %x %d\n#######",diff,diff);

	TRACE("scaleBegin=%x\n",start+(header->scaleBegin-header->scaleBegin));
	TRACE("scaleEnd=%x\n",start+(header->scaleEnd-header->scaleBegin));
	TRACE("textureBegin=%x\n",start+(header->textureBegin-header->scaleBegin));
	TRACE("textureEnd=%x\n",start+(header->textureEnd-header->scaleBegin));
	TRACE("pointsBegin=%x\n",start+(header->pointsBegin-header->scaleBegin));

	numVerticies = header->size8/8;
	numScale  = (header->scaleEnd - header->scaleBegin)/2;
	numPoints =	(header->vertexBegin - header->pointsBegin)/8;
	numUnks   = (header->fileEnd -header->vertexEnd)/2;

	TRACE("numVerticies=%d\n",numVerticies);

	numVerticies = (header->vertexEnd - header->vertexBegin)/4;

	TRACE("numVerticies=%d\n",numVerticies);
	TRACE("numScale=%d\n",numScale);
	TRACE("numPoints=%d\n",numPoints);

	scaleData  = new Scale[numScale];
	pointData  = new Point3D[numPoints];
	vertexData = new Vertex[numVerticies];
	textureData = new TextureCmd[1000];
	unkData = new DatVal[30000];
	updateData(TRUE);
}

void Helmet::LoadTree(CCarTree *tree)
{
	char buffer[256];
	carNode = tree->insertInfoNode(tree->m_CarTree.GetRootItem(),"Helmet",TO_ID(IDB_HELMET));

	wsprintf(buffer,"Points %d",numPoints);
	HTREEITEM pts = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POINT));
	for(int i=0;i<numPoints;i++)
	{
	   if (pointData[i].getDesc()!=NULL)
	   {
		 wsprintf(buffer,"p[%d] %d %d %d %s",i,pointData[i].x,pointData[i].y,pointData[i].z,pointData[i].getDesc());
	   }
	   else
	   {
	     wsprintf(buffer,"p[%d] %d %d %d",i,pointData[i].x,pointData[i].y,pointData[i].z);
	   }
	   HTREEITEM ptItem = tree->insertInfoNode(pts,buffer,TO_ID(IDB_POINT));
	   tree->m_CarTree.SetItemData(ptItem,(DWORD)&pointData[i]);
	   pointData[i].node = ptItem;
	   pointData[i].idx = i;
	   pointData[i].object = this;
	}

	wsprintf(buffer,"Verticies %d",numVerticies);
	HTREEITEM vertex = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(int i=0;i<numVerticies;i++)
	{
	  
	   sprintf(buffer,"v[%d] %d -> %d =%2.2fcm",i+1,vertexData[i].from,vertexData[i].to,vertexData[i].LengthCM());
	   HTREEITEM verItem = tree->insertInfoNode(vertex,buffer,TO_ID(IDB_VERTICIES));
	   tree->m_CarTree.SetItemData(verItem,(DWORD)&vertexData[i]);
	   vertexData[i].node = verItem;
	   vertexData[i].idx = i;
	   vertexData[i].object = this;
	}

	wsprintf(buffer,"ScaleData %d",numScale);
	HTREEITEM scalep = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_SCALE));
	for(int i=0;i<numScale;i++)
	{
	  
	   if (i<31)
	   {
	     wsprintf(buffer,"s[%d] %d",i,scaleData[i].val);
	   }
	   else
	   {
		 wsprintf(buffer,"s[%d] %d",i,scaleData[i].val);
	   }
	   HTREEITEM scaleItem = tree->insertInfoNode(scalep,buffer,TO_ID(IDB_SCALE));
	   scaleData[i].node = scaleItem;
	   scaleData[i].idx = i;
	   scaleData[i].object = this;
	   tree->m_CarTree.SetItemData(scaleItem,(DWORD)&scaleData[i]);
	}


	wsprintf(buffer,"TextureData %d",textureSize);
	HTREEITEM textureP = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POLYGONS));

	for(int i=0; i< textureSize;i++)
	{
		char argsStr[1024];

		strcpy(argsStr,"Args=");

		for(int j=0;j<textureData[i].numArgs;j++)
		{
			char msg[20];
			signed short da =  textureData[i].Args[j];
			wsprintf(msg,"%x ",da);
			strcat(argsStr,msg);
		}

		wsprintf(buffer,"[%d] %d (cmd = 0x%x) %s",i,textureData[i].numl,textureData[i].cmd,argsStr);
		HTREEITEM textItem = tree->insertInfoNode(textureP,buffer,TO_ID(IDB_POLYGONS));
		HTREEITEM textureItem = textureData[i].LoadTree(tree,textItem);
		textureData[i].node = textItem;
		textureData[i].idx = i;
		tree->m_CarTree.SetItemData(textItem,(DWORD)&textureData[i]);


	}


	wsprintf(buffer,"UnkData %d",numUnks);
	HTREEITEM unkp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(int i=0;i<numUnks;i+=8)
	{
	  
	   wsprintf(buffer,"u[%d] %d %d %d %d %d %d %d %d",i,
				unkData[i].data,
				unkData[i+1].data,
				unkData[i+2].data,
				unkData[i+3].data,
				unkData[i+4].data,
				unkData[i+5].data,
				unkData[i+6].data,
				unkData[i+7].data);
	   HTREEITEM verItem = tree->insertInfoNode(unkp,buffer,TO_ID(IDB_VERTICIES));
	   //tree->m_CarTree.SetItemData(verItem,(DWORD)&vertexData[i]);
	   unkData[i].node = verItem;
	   unkData[i].idx = i;
	}
}


void Helmet::updateData(BOOL updateTexture)
{
	int start=78;

	if (memoryOffset == DEF_CAR_START)
	{
		start = 106;
	}

	for(int i=0;i<numScale;i++)
	{
		int offset = start+(i*2);
		int val = GetPCWord(offset);
		//TRACE("Scale %d = %d\n",i,val);

		if (val > 0x8000) val = -(0xFFFF-val);
		scaleData[i].val    = val;
		scaleData[i].offset = offset;
		scaleData[i].object = this;
	}



	for(int i=0;i<numVerticies;i++)
	{
		int offset = 4+start+(header->vertexBegin-header->scaleBegin)+(i*4);
		//if (memoryOffset == DEF_CAR_START)
	    //{
		//  offset = 0x1d42+(i*4);
		//}
		vertexData[i].offset = offset;
		vertexData[i].object = this;
		int from = GetPCByte(offset);
		int to   = GetPCByte(offset+1);
		int A    = GetPCByte(offset+2);
		int B    = GetPCByte(offset+3);
		//TRACE("Vertex %x %d = %d %d (%d %d)\n",offset,i,from,to,A,B);
		vertexData[i].from = from;
		vertexData[i].to = to;
	}

	for(int i=0;i<numUnks;i++)
	{
	   int offset = start+(header->vertexEnd-header->scaleBegin)+(i*2);
	   int data   = (int)GetPCByte(offset);	
	   unkData[i].data = data;
	}

	for(int i=0;i<numPoints;i++)
	{
		int offset = start+(header->pointsBegin-header->scaleBegin)+(i*8);
		//int offset = 0x1126+(i*8);
		pointData[i].offset = offset;
		pointData[i].object = this;
		int x   = (int)GetPCWord(offset);
		int y   = (int)GetPCWord(offset+2);
		int z   = (int)GetPCWord(offset+4);
		int u   = (int)GetPCWord(offset+6);

		pointData[i].ox = x;
		pointData[i].oy = y;

		//TRACE("%x (0x%x 0x%x 0x%x)\n",offset,x,y,z);

		if (z > 0x8000) z = -(0xFFFF-z);

		if (x < 0x8000)
		{	
		 if (x > 0x80 && x < 0xFF)
		 {
			int idx = (x-0x84)/4;
			x = scaleData[idx].val;
			x = -x;
		 }
		 else if (x > 0)
		 {
		   int idx = (x-0x4)/4;
		   x = scaleData[idx].val;
		 }
		
		 if (y > 0x80 && y < 0xFF)
		 {
			int idx = (y-0x84)/4;
			y = scaleData[idx].val;
			y = -y;
		 }
		 else if (y > 0)
		 {
			int idx = (y-0x04)/4;
			y = scaleData[idx].val;
		
		 }
		}
		else
		{
		   int pidx = x - 0x8000;
		   x = pointData[pidx].x;
		   y = pointData[pidx].y;
		}

		pointData[i].x = x;
		pointData[i].y = y;
		pointData[i].z = z;
	}


	if (updateTexture)
	{
	int count = start+(header->textureBegin-header->scaleBegin);
	int end   = start+(header->textureEnd-header->scaleBegin);

	//textureSize = end-count;

	int idx=0;
	textureSize = 0;

	while(count < end)
	{
		int numl = GetPCByte(count++);
		int numh = GetPCByte(count++);
		int cmd  = GetPCByte(count++);
		textureData[idx].numl = numl;
		textureData[idx].numh = numh;
		textureData[idx].cmd = cmd;
		textureData[idx].object = this;

		int nArgs=0;

		switch(cmd)
		{
		   case 0x80:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  case 0x90:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  
		  
		  case 0x13:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  if (textureData[idx].Args[3] == 0x80)
			  {
				textureData[idx].Args[nArgs++] = GetPCByte(count++);
			    textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  }
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0x18:
		  case 0x11:
		  case 0x1a:
		  case 0x16:
		  case 0x17:		 
		  case 0x12:
		  case 0x15:
		  //case 0xa:
		  case 0x10:
		  case 0x0:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  
			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  
			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		}

		textureData[idx].parse();
		idx++;
		textureSize++;
	  }
    }
}

void Helmet::PatchData()
{
	for(int i=0;i<numScale;i++)
	{
		if (scaleData[i].isChanged())
		{
		  scaleData[i].patch();
		}
	}
	for(int i=0;i<numVerticies;i++)
	{
		if (vertexData[i].isChanged())
		{
		  vertexData[i].patch();
		}
	}
	for(int i=0;i<numPoints;i++)
	{
		if (pointData[i].isChanged())
		{
		  pointData[i].patch();
		}
	}
}

void Helmet::write(CString filename)
{
	FILE *fp = fopen(filename,"wb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot open file for writing",MB_OK);
		return;
	}

	int length = DEF_HELMET_LENGTH;

	fwrite(data,length,sizeof(char),fp);

	fclose(fp);
}

Helmet *LoadHelmet(LPCSTR filename)
{
	int length = GetLengthofFile(filename);

	if (length!=-1)
	{
	FILE *fp = fopen(filename,"rb");

	if (fp== NULL)
	{
		AfxMessageBox("Canot open Car file",MB_OK);
		return NULL;
	}
	Helmet *myhelmet = new Helmet();
	//myhelmet->setCarName((char*)filename);

	fread(myhelmet->data,length,sizeof(char),fp);

	myhelmet->parse(DEF_HELMET_START);

	LoadHTextureCfg();

	fclose(fp);
	return myhelmet;
	}
	return NULL;
}












