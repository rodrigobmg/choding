#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"

/*
 
struct Material
{
             bool bUseTexture; // 텍스춰 사용유무
             bool bUseOpacity; // Opacity map 사용유무
             char TextureName[256]; 
             int SubMaterialCount;
             Material* SubMaterial;
             LPDIRECT3DTEXTURE8 pTexture; // 텍스쳐
             D3DMATERIAL8 d3dMaterial; // d3d 재질 객체
             CTextureContainer* m_pTextureContainer; // 텍스쳐 관리 class
 
             Material():
                           bUseTexture(false),
                           bUseOpacity(false),
                           SubMaterialCount(0),
                           SubMaterial(NULL),
                           pTexture(NULL),
                           m_pTextureContainer(NULL)
             {
             }
             ~Material()
             {
                           if(SubMaterial)
                                        delete[] SubMaterial;
                           if(m_pTextureContainer&&pTexture)
                                        m_pTextureContainer->Delete(pTexture);
             }
};
D3D는 Max의 모든 재질 정보를 사용 할 수는 없다. API자체의 한계이기도 하지만, 속도의 문제로 몇 가지 이외에는 희생 할 수밖에 없는 부분이다. 위에서도 잠깐 소개한 대로 Max의 많은 정보 중 Texture 파일의 이름과 재질값, 그리고 Alpha channel사용유무정도만을 가지고 온다. 이제 Decoding 루틴을 보자.
 
int CAse::DecodeMaterial(FILE* fp, Material* CurMaterial)
{
             CD3dUtil d3dutil;
             d3dutil.InitMaterial(CurMaterial->d3dMaterial);
             int num;
             do {
                           linecount++;
                           fgets(line, 256, fp);
                           sscanf(line,"%s",string);
                           if(_strcmpi(string,"*SUBMATERIAL")==0)
                           {
// Max는 서브재질이란 독특한 구조를 지원하므로, 잘 알아두자.
                                        sscanf(line,"%s%d",string, &num);
                                        DecodeMaterial(fp,&(CurMaterial->SubMaterial[num]));
                                        continue;
                           }
                           if(_strcmpi(string,"*NUMSUBMTLS")==0)
                           {
                                        if(CurMaterial->SubMaterial)
                                                     delete CurMaterial->SubMaterial;
                                        sscanf(line,"%s%d",string,&CurMaterial->SubMaterialCount);
// 재질은 생성시 개수를 알 수 있으므로 배열로 생성한다.
                                        CurMaterial->SubMaterial = 
                                        new Material[CurMaterial->SubMaterialCount];
                                        continue;
                           }
// 실제 재질의 색값을 알아낸다.                  
                           if(_strcmpi(string,"*MATERIAL_AMBIENT")==0)
                           {
                                        sscanf(line,"%s %f%f%f",string, 
                                                     &CurMaterial->d3dMaterial.Ambient.r, 
                                                     &CurMaterial->d3dMaterial.Ambient.g, 
                                                     &CurMaterial->d3dMaterial.Ambient.b);
                                        continue;
                           }
                           if(_strcmpi(string,"*MATERIAL_DIFFUSE")==0)
                           {
                                        sscanf(line,"%s %f%f%f",string, 
                                                     &CurMaterial->d3dMaterial.Diffuse.r,
                                                     &CurMaterial->d3dMaterial.Diffuse.g, 
                                                     &CurMaterial->d3dMaterial.Diffuse.b);
                                        continue;
                           }
                           if(_strcmpi(string, "*MATERIAL_SPECULAR")==0)
                           {
                                        sscanf(line, "%s %f%f%f",string, 
                                                     &CurMaterial->d3dMaterial.Specular.r, 
                                                     &CurMaterial->d3dMaterial.Specular.g, 
                                                     &CurMaterial->d3dMaterial.Specular.b);
                                        continue;
                           }
// 텍스쳐가 사용되었으면 이런 것이 나온다.                           
                           if(_strcmpi(string, "*MAP_DIFFUSE")==0)
                           {
// 텍스쳐 사용 플래그를 On!!!
                                        CurMaterial->bUseTexture = true;
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// 알파채널을 사용하면 나온다.
                           if(_strcmpi(string, "*MAP_OPACITY")==0)
                           {
// Opacity 맵이 사용되면, 알파채널이 있는 텍스쳐(TGA포맷등)가 사용된 것이다.
                                        CurMaterial->bUseTexture = true;
                                        CurMaterial->bUseOpacity = true;
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// 이런 것도 있는데 실시간 렌더링에선 무용지물이다.
                           if(_strcmpi(string, "*MAP_SELFILLUM")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// 만고에 쓸모없다. 디자이너에게 쓰지 말라고 충고한다.
                           if(_strcmpi(string, "*MAP_REFLECT")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// 역시 사용 안한다. 단지 Decode만 해서 몇줄 넘기면 끝이다.
                           if(_strcmpi(string, "*MAP_SPECULAR")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// ASE는 구문이 끝나면 }(괄호)로 막아버린다. 이게 나오면 해당구조체는 다 읽은거다.
                           if(_stricmp(string, "}")==0)
                           {
                                        break;
                           }
             } while (1);
             return 0;
}
텍스쳐 매핑에 관한 장황한 정보가 있는 맵부분이지만, 실제로 읽어서 사용하는 부분은 텍스쳐 파일의 이름뿐이다. 그러므로, 텍스쳐 파일 이름만을 읽어들이고나서 '}'가 나올 때까지 루프를 조용히 돌면 된다. 한가지 주의할 점이라면, 디자이너들은 대부분 파일이름이나 디렉토리이름에 공백을 많이 사용하는데, sscanf는 공백단위로 파싱하는 단점이 있다. 그러므로, 파일이름은 특별히 “”단위의 파싱을 해줘야 한다.
int CAse::DecodeMap(FILE* fp, Material* CurMaterial)
{
             bool check;
             int i,j;
             char name[200];
             do{
                           linecount++;
                           fgets(line, 256, fp);
                           sscanf(line,"%s",string);
                           if(_strcmpi(string,"*BITMAP")==0)
                           {
                                        sscanf(line,"%s %s",string, name);
                                        memset(CurMaterial->TextureName, 0, 256);
                                        j=0         ;
                                        check = false;
                                        memset(CurMaterial->TextureName, 0, 256);
// 여기가 “”단위로 파싱하는 부분, 스크립트에서 많이 다룬 부분이다.
                                        for(i=0; i<256; i++)
                                        {
                                                     if(check)
                                                     {
                                                                   if(line[i]=='"'){ check=false; break;}
                                                                   CurMaterial->TextureName[j] = line[i];
                                                                   j++;
                                                     }
                                                     if(line[i]=='"')check = true;
 
                                        }
                                        GetFileName(CurMaterial->TextureName, 256);
                                        continue;
                           }
                           if(_stricmp(string, "}")==0)
                           {
                                        break;
                           }
             }while(1);
             return 0;
}
재질은 텍스쳐를 제외하면 그리 중요한 정보가 없으므로 쉬운 편이지만, Mesh의 경우 실제로 폴리곤의 형태를 나타내는 만큼 상당히 어렵다. 여기서는 일단 메시를 읽는 부분에 대해서만 설명하고 뒤에서 애니메이션 class와 애니메이션 데이터를 읽는 것을 설명하겠다.
 
위에서 잠깐 설명한대로 메시데이터는 몇 부분으로 나누어져있다.  메시 정보는 Transform 행렬 정보, 정점 정보, 면 정보, 그리고 Animation 정보로 이루어져 있다. 이러한 정보를 저장하는 Mesh class는 다음과 같다.
 
class MESH {
public:
             char Name[256];
             bool bParents;
             char ParentName[256];
             int NumVertex;
             int NumTriangle;
             D3DXVECTOR3* VertexList;
             TRIANGLE* TriangleList;
             D3DXMATRIX TmMatrix;
             D3DXMATRIX InvTm;
 
             D3DXVECTOR3 TmPos;
             D3DXQUATERNION TmRot;
             D3DXVECTOR3 TmScale;
             D3DXQUATERNION TmScaleRot;
             TEXCOORDFLOAT* TVert;
             MESH* m_pNext;
             MESH* m_pParents;
             int MaterialRef;
             sViewList* ViewList;
             D3DXCOLOR* ColorList;
 
 
             ANIMATION Animation;
             MeshContainer* m_pChilds;
 
             D3DXMATRIX CalcMatrix;
 
             D3DXVECTOR3 vMin;
             D3DXVECTOR3 vMax;
             D3DXVECTOR3 vCenter;
             D3DXVECTOR3 vRadius;
             float fRadius;
 
             MESH()
                           :NumVertex(0),
                           NumTriangle(0),
                           VertexList(NULL),
                           ViewList(NULL),
                           TriangleList(NULL),
                           m_pNext(NULL),
                           MaterialRef(0),
                           TVert(NULL),
                           m_pChilds(NULL),
                           vMin(0,0,0),
                           vMax(0,0,0),
                           vRadius(0,0,0),
                           fRadius(0),
                           bParents(false),
                           ColorList(NULL),
                           m_pParents(NULL)
             {
             }
             ~MESH()
             {
                           if(VertexList)
                                        delete[] VertexList;
                           if(TriangleList)
                                        delete[] TriangleList;
                           if(m_pNext)delete m_pNext;
                           if(TVert)delete[] TVert;
                           if(ViewList)delete ViewList;
                           if(ColorList)delete[] ColorList;
             }
             void CalulateBoundingSphere();
             void AddChild(MESH* pMesh);
             void AnimateMatrix(D3DXMATRIX* mat, float CurFrame);
             void MakeVERTEX();
             void MakeIndex(Material* MaterialList);
};
VertexList 는 실제 메시의 각각의 정점의 위치정보를 가지고 있다. 300개의 정점으로 이루어진 메시라면 실제 VertexList에는 300개의 정점을 가지고 있다. 하지만, 이 정점정보만으로는 메시로 출력할 수 없다. 이유는 이 정점들이 어떤 식으로 면을 이루는지에 대한 정보가 없기 때문이다. 실제 면에 대한 모든 정보는 TriangleList에 저장이 된다.
 
struct TRIANGLE {
             int Vertex[3];
             DWORD VertexColor[3];
             D3DXVECTOR3 VertexNormal[3];
             D3DXVECTOR3 FaceNormal;
             TEXCOORDFLOAT VertexTexture[3];
             int MaterialID;
             bool bView;
             void Init();
};
TRIANGLE 구조체에는 면을 이루는 세 정점의 색인을 가지고 있다. 또한 이 정점의 법선값과 Color값, 그리고 Texture의 u,v좌표, 재질의 색인을 담고 있다. 화면에 실제로 출력하는 것은 이 면 정보를 가지고 출력하게 된다. 이제 실제로 파일에서 읽어들이는 부분의 소스를 훑어보도록 하자.
 
int CAse::DecodeGEOMOBJECT(FILE *fp)
{
             int num;
             int i,j;
             bool check;
             do{
                           linecount ++;
                           fgets(line, 256, fp);//*NODE_NAME
                           sscanf(line,"%s",string);
// 현재 메시의 이름을 저장한다. 메시의 이름은 계층구조를 만들 때 꼭 필요하므로, 잘 저장해야한다. 역시 공백이 들어가는 경우가 많으므로 이것을 염두에 두고 읽어들인다.
                           if(_strcmpi(string,"*NODE_NAME")==0)
                           {
                                        j=0         ;
                                        check = false;
                                        memset(CurMesh->Name, 0, 256);
                                        for(i=0; i<256; i++)
                                        {
                                                     if(check)
                                                     {
                                                                   if(line[i]=='"'){ check=false; break;}
                                                                   CurMesh->Name[j] = line[i];
                                                                   j++;
                                                     }
                                                     if(line[i]=='"')check = true;
 
                                        }
                                        continue;
                           }
// 계층구조상 어미노드(어미메시) 의 이름을 읽어서 저장한다.
                           if(_strcmpi(string, "*NODE_PARENT")==0)
                           {
                                        CurMesh->bParents = true;
                                        j=0         ;
                                        check = false;
                                        memset(CurMesh->ParentName, 0, 256);
                                        for(i=0; i<256; i++)
                                        {
                                                     if(check)
                                                     {
                                                                   if(line[i]=='"'){ check=false; break;}
                                                                   CurMesh->ParentName[j] = line[i];
                                                                   j++;
                                                     }
                                                     if(line[i]=='"')check = true;
 
                                        }
                                        continue;
                           }
                           if(_strcmpi(string,"*NODE_TM")==0)
                           {
                                        DecodeTm(fp);
                                        continue;
                           }
                           if (_stricmp(string, "*MESH") == 0)
                           {
                                        DecodeMESH(fp);
                                        continue;
                           }
                           if( _stricmp(string, "*MATERIAL_REF")==0)
                           {
                                        sscanf(line, "%s %d", string, &num);
                                        CurMesh->MaterialRef = num;
                                        continue;
                           }
                           if( _stricmp(string, "*TM_ANIMATION")==0)
                           {
                                        DecodeANIMATION(&CurMesh->Animation, fp);
                                        continue;
                           }
                           if ( _stricmp(string, "}") == 0 ) return 0;
             }while(1);
             return 0;
}
TM은 위에서 말한대로 Max좌표계를 D3D좌표계로 바꿔주면서 읽어들이면 된다. 방법은 앞서 설명한 대로 y와 z축을 서로 바꿔주면 된다.
 
int CAse::DecodeTm(FILE* fp)
{
             float fX, fY, fZ, fW;
             D3DXMatrixIdentity(&(CurMesh->TmMatrix));
             linecount +=15;
             fgets(line, 256, fp);//*NODE_NAME
             fgets(line, 256, fp);//*INHERIT_POS
             fgets(line, 256, fp);//*INHERIT_ROT
             fgets(line, 256, fp);//*INHERIT_SCL
             fgets(line, 256, fp);//*TM_ROW0
             sscanf(line,"%s%f%f%f",string, 
// 읽는 순서에 유의하기 바란다.
&(CurMesh->TmMatrix._11),&(CurMesh->TmMatrix._13),&(CurMesh->TmMatrix._12));
             fgets(line, 256, fp);//*TM_ROW1
             sscanf(line,"%s%f%f%f",string, 
&(CurMesh->TmMatrix._31),&(CurMesh->TmMatrix._33),&(CurMesh->TmMatrix._32));
             fgets(line, 256, fp);//*TM_ROW2
             sscanf(line,"%s%f%f%f",string, 
&(CurMesh->TmMatrix._21),&(CurMesh->TmMatrix._23),&(CurMesh->TmMatrix._22));
             fgets(line, 256, fp);//*TM_ROW3
             sscanf(line,"%s%f%f%f",string, 
&(CurMesh->TmMatrix._41),&(CurMesh->TmMatrix._43),&(CurMesh->TmMatrix._42));
             fgets(line, 256, fp);//*TM_POS
             sscanf(line, "%s%f%f%f",string, 
&(CurMesh->TmPos.x),&(CurMesh->TmPos.z),&(CurMesh->TmPos.y));
             fgets(line, 256, fp);//*TM_ROTAXIS
             sscanf(line, "%s%f%f%f",string, &fX,&fZ,&fY);
             fgets(line, 256, fp);//*TM_ROTANGLE 
             sscanf(line, "%s%f",string, &fW);
// 각변위를 사원수로 바꿔준다.
             CurMesh->TmRot.x = (float)sinf(fW / 2.0f) * fX;
             CurMesh->TmRot.y = (float)sinf(fW / 2.0f) * fY;
             CurMesh->TmRot.z = (float)sinf(fW / 2.0f) * fZ;
             CurMesh->TmRot.w = (float)cosf(fW / 2.0f);
             fgets(line, 256, fp);//*TM_SCALE
             sscanf(line, "%s%f%f%f",string, &(CurMesh->TmScale.x), 
                           &(CurMesh->TmScale.z), &(CurMesh->TmScale.y));
             fgets(line, 256, fp);//*TM_SCALEAXIS
             sscanf(line, "%s%f%f%f",string, &fX,&fZ,&fY);
             fgets(line, 256, fp);//*TM_SCALEAXISANG
             sscanf(line, "%s%f",string, &fW);
             CurMesh->TmScaleRot.x = (float)sin(fW / 2.0f) * fX;
             CurMesh->TmScaleRot.y = (float)sin(fW / 2.0f) * fY;
             CurMesh->TmScaleRot.z = (float)sin(fW / 2.0f) * fZ;
             CurMesh->TmScaleRot.w = (float)cos(fW / 2.0f);
             fgets(line, 256, fp);//}
             return 0;
}
정점 정보와 면 정보를 읽어들이는 부분이다. 
int CAse::DecodeMESH(FILE *fp)
{
             linecount++;
             fgets(line, 256, fp);//*TIMEVALUE
                           
             int VCOUNT;
             linecount++;
             fgets(line, 256, fp);//*MESH_NUMVERTEX
             sscanf(line, "%s%d", string, &VCOUNT);
             CurMesh->NumVertex = VCOUNT;
             ///////////
             if (CurMesh->VertexList)
                           delete[] CurMesh->VertexList;
// 친절하게도 몇 개의 정점이 있는지 알려주기 때문에 간단히 배열로 읽어들일수있다.
             CurMesh->VertexList = new D3DXVECTOR3[VCOUNT];
 
             int TCOUNT;
             linecount++;
             fgets(line, 256, fp);//*MESH_NUMFACES
             sscanf(line, "%s%d", string, &TCOUNT);
// 면의 개수도 알려준다.
             if (CurMesh->TriangleList)
                           delete[] CurMesh->TriangleList;
             CurMesh->TriangleList = new TRIANGLE[TCOUNT];
             for(int i=0; i<TCOUNT; i++)
             {
                           CurMesh->TriangleList[i].Init();
             }
             CurMesh->NumTriangle = TCOUNT;
             linecount++;
             fgets(line, 256, fp);//*MESH_VERTEX_LIST
             sscanf(line, "%s", string);
             if ( _stricmp(string, "*MESH_VERTEX_LIST") == 0 )
                           DecodeMESH_VERTEX_LIST(fp);
             linecount++;
             fgets(line, 256, fp);//*MESH_FACE_LIST
             sscanf(line, "%s", string);
             if ( _stricmp(string, "*MESH_FACE_LIST") == 0 )
                           DecodeMESH_FACE_LIST(fp);
             while (1)
             {
                           linecount++;
                           fgets(line, 256, fp);
                           sscanf(line, "%s", string);
                           if ( _stricmp(string, "*MESH_NUMTVERTEX") == 0 )
                           {
// Texture u,v좌표에 관한 부분을 읽어들인다.
                                        DecodeMESH_TVERTLIST(fp);
                                        continue;
                           }
                           if(_stricmp(string, "*MESH_NUMTVFACES")==0)
                           {
// 읽어들인 u,v좌표들이 각각 어떤 면에 적용되는지를 읽어준다.
                                        DecodeMESH_TFACELIST(fp);
                                        continue;
                           }            
                           if ( _stricmp(string, "*MESH_NUMCVERTEX") == 0)
                           {
// 정점 색상이 있는 경우 읽어준다. 실제로 잘 쓰이지는 않는다.
                                        DecodeMESH_CVERTEX(fp);
                                        continue;
                           }
                           if(_stricmp(string,"*MESH_NUMCVFACES")==0)
                           {
                                        DecodeMESH_CFACELIST(fp);
                                        continue;
                           }
                           
                           if ( _stricmp(string, "*MESH_NORMALS") == 0 )
                           {
                                        DecodeMESH_NORMALS(fp);
                                        continue;
                           }
                           if ( _stricmp(string, "}") == 0 ) return 0;
             }
             
             linecount++;
             fgets(line, 256, fp);//}
 
             return 0;
}
// 정점 정보는 순서대로 읽어주면 된다. 단, 좌표계 변환을 빼먹으면 안된다.
int CAse::DecodeMESH_VERTEX_LIST(FILE *fp)
{
 
             int Counter = 0;
             int num;
             while( Counter < CurMesh->NumVertex )
             {
                           linecount++;
                           fgets(line, 256, fp);//*MESH_VERTEX
                           sscanf(line, "%s%d%f%f%f", string, &num, 
                                        &CurMesh->VertexList[Counter].x, 
                                        &CurMesh->VertexList[Counter].z, 
                                        &CurMesh->VertexList[Counter].y);
                           Counter++;
             }
             linecount++;
             fgets(line, 256, fp);//}
 
             return 0;
}
 
int CAse::DecodeMESH_FACE_LIST(FILE *fp)
{
             int num;
             int Counter = 0;
             char Smooth[20];
             char MaterID[20];
             int ID;
             while( Counter < CurMesh->NumTriangle )
             {
                           linecount++;
 
                           fgets(line, 256, fp);
                           memset(MaterID, 0, 20);
                           memset(Smooth, 0, 20);
                           sscanf(line, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d", 
                                        string, string,
                                        string, &CurMesh->TriangleList[Counter].VertexIndex[0], 
                                        string, &CurMesh->TriangleList[Counter].VertexIndex[2], 
                                        string, &CurMesh->TriangleList[Counter].VertexIndex[1],
                                        string, &num, string, &num, string, &num, string, Smooth,
                                        MaterID, &ID);
// Material ID는 해당 면의 재질의 색인(index)인데, 이것은 보통 Submaterial의 색인이다.
// 메시자체에도 Material Ref라 하여 참조할 재질이 존재한다. ASE포맷에서는 각 면마다 다른 재질을 입힐 수 있기 때문에, 메시 참조재질 의 Submaterial의 색인를 각 면마다 지정할 수 있는 것이다. 
// 가끔 Smooth정보는 생략이 되곤 하기 때문에 어쩔 수 없이 검사를 해야한다.
                           if(_strcmpi(MaterID,"*MESH_MTLID")==0)
                           {
                                        CurMesh->TriangleList[Counter].MaterialID = ID;
                           }
                           if(_strcmpi(Smooth,"*MESH_MTLID")==0)
                           {
                                        CurMesh->TriangleList[Counter].MaterialID = atoi(MaterID);
                           }
                           Counter++;
             }
             linecount++;
             fgets(line, 256, fp);//}
 
             return 0;
}
 
int CAse::DecodeMESH_TVERTLIST(FILE *fp)
{
             int MAXTVERTEX;
             sscanf(line, "%s %d", string, &MAXTVERTEX);
             if(MAXTVERTEX<=0)return 0;
 
             CurMesh->TVert = new TEXCOORDFLOAT[MAXTVERTEX];
             linecount++;
             float u, v, w;
             fgets(line, 256, fp);//*MESH_TVERTLIST
             
             int counter = 0;
             int num;
// 텍스춰 u,v좌표를 저장한다. 
             while( counter < MAXTVERTEX )
             {
                           linecount++;
                           fgets(line, 256, fp);//*MESH_TVERT
                           sscanf(line, "%s %d%f%f%f", string, &num, 
                                        &u, &v, &w);
                           CurMesh->TVert[num].u = u;
                           CurMesh->TVert[num].v = 1.0f - v;
                           counter++;
             }
             linecount++;
             fgets(line, 256, fp);//}
 
             return 0;
}
 
int CAse::DecodeMESH_TFACELIST(FILE *fp)
{
             int TIndex[3];
 
             int TFACECOUNT;
             sscanf(line, "%s %d", string, &TFACECOUNT);
 
             if(TFACECOUNT<=0)return 0;
             linecount++;
             fgets(line, 256, fp);//*MESH_TFACELIST
             int Count = 0;
             int num;
// 위에 저장된 Texture u,v가 어떤 면에 어떤 정점에 속하는 값인지를 읽어들인다.
// 이때, CounterClock Wise로 변환하기 위해서 0, 2, 1 의 순서로 대입한다.
             while( Count < TFACECOUNT )
             {
                           linecount++;
                           fgets(line, 256, fp);//*MESH_TFACE
                           sscanf(line, "%s %d %d %d %d", string, &num, 
                                        &TIndex[0], 
                                        &TIndex[2], 
                                        &TIndex[1]);
                           if(CurMesh->TVert)
                           {
                                        CurMesh->TriangleList[num].VertexTexture[0].u = 
                                                     CurMesh->TVert[ TIndex[0] ].u; 
                                        CurMesh->TriangleList[num].VertexTexture[0].v = 
                                                     CurMesh->TVert[ TIndex[0] ].v; 
                                        CurMesh->TriangleList[num].VertexTexture[1].u = 
                                                     CurMesh->TVert[ TIndex[1] ].u; 
                                        CurMesh->TriangleList[num].VertexTexture[1].v = 
                                                     CurMesh->TVert[ TIndex[1] ].v; 
                                        CurMesh->TriangleList[num].VertexTexture[2].u = 
                                                     CurMesh->TVert[ TIndex[2] ].u; 
                                        CurMesh->TriangleList[num].VertexTexture[2].v = 
                                                     CurMesh->TVert[ TIndex[2] ].v; 
                           }
 
                           Count++;
             }
             linecount++;
             fgets(line, 256, fp);//}
 
             return 0;
}
// 정점 색상에 대한 부분으로 Texture를 읽는 부분과 거의 비슷하다.
int          CAse::DecodeMESH_CVERTEX(FILE *fp)
{
 
             int i,num;
             sscanf(line,"%s %d",string, &num);
             if(num<=0)return 0;
             CurMesh->ColorList = new D3DXCOLOR[num];
             int colnum;
             float r,g,b;
 
             linecount++;
             fgets(line, 256, fp);
             for(i=0; i<num; i++)
             {
                           linecount++;
                           fgets(line, 256, fp);
                           sscanf(line,"%s %d%f%f%f", string, &colnum, &r, &g, &b);
                           CurMesh->ColorList[colnum].r = r;
                           CurMesh->ColorList[colnum].g = g;
                           CurMesh->ColorList[colnum].b = b;
                           CurMesh->ColorList[colnum].a = 1.0f;
             }
             linecount++;
             fgets(line, 256, fp);
 
             return 0;
}
 
int          CAse::DecodeMESH_CFACELIST(FILE *fp)
{
             BYTE R0, G0, B0, A0;
             int i,num;
             sscanf(line,"%s %d",string, &num);
             if(num<=0)return 0;
             int facenum, v1, v2, v3;
             linecount++;
             fgets(line, 256, fp);//*MESH_FACENORMAL
             for(i=0; i<num; i++)
             {
                           linecount++;
                           fgets(line, 256, fp);//*MESH_FACENORMAL
                           sscanf(line,"%s %d %d %d %d",string, &facenum, &v1, &v3, &v2);
                           R0 = (BYTE)(CurMesh->ColorList[v1].r*255.0f);
                           G0 = (BYTE)(CurMesh->ColorList[v1].g*255.0f);
                           B0 = (BYTE)(CurMesh->ColorList[v1].b*255.0f);
                           A0 = (BYTE)(CurMesh->ColorList[v1].a*255.0f);
                           CurMesh->TriangleList[facenum].VertexColor[0] = 
                                        D3DCOLOR_XRGB(R0,G0,B0);
                           R0 = (BYTE)(CurMesh->ColorList[v2].r*255.0f);
                           G0 = (BYTE)(CurMesh->ColorList[v2].g*255.0f);
                           B0 = (BYTE)(CurMesh->ColorList[v2].b*255.0f);
                           A0 = (BYTE)(CurMesh->ColorList[v2].a*255.0f);
                           CurMesh->TriangleList[facenum].VertexColor[1] = 
                                        D3DCOLOR_XRGB(R0,G0,B0);
                           R0 = (BYTE)(CurMesh->ColorList[v3].r*255.0f);
                           G0 = (BYTE)(CurMesh->ColorList[v3].g*255.0f);
                           B0 = (BYTE)(CurMesh->ColorList[v3].b*255.0f);
                           A0 = (BYTE)(CurMesh->ColorList[v3].a*255.0f);
                           CurMesh->TriangleList[facenum].VertexColor[2] = 
                                        D3DCOLOR_XRGB(R0,G0,B0);
             }
             linecount++;
             fgets(line, 256, fp);//*MESH_FACENORMAL
 
             return 0;
}
// 실제 면의 법선과 각 정점의 법선을 읽어들인다.
// 같은 정점이라도 어떤 면에 속할 때인지에 따라 그 값들이 완전히 바뀌게 된다.
int CAse::DecodeMESH_NORMALS(FILE *fp)
{
             int counter = 0;     
             
             NormalFlag = TRUE;
             float x, y, z;
             int num, num2;
             NormalFlag = TRUE;
             while ( counter < CurMesh->NumTriangle )
             {
                           linecount++;
                           fgets(line, 256, fp);//*MESH_FACENORMAL
                           sscanf(line, "%s%d%f%f%f", string, &num,&x,&z,&y);
                           CurMesh->TriangleList[num].FaceNormal.x = x;
                           CurMesh->TriangleList[num].FaceNormal.y = y;
                           CurMesh->TriangleList[num].FaceNormal.z = z;
 
                           linecount++;
                           fgets(line, 256, fp);//*MESH_VERTEXNORMAL
                           sscanf(line, "%s%d%f%f%f", string, &num,&x,&z,&y);
                           CurMesh->TriangleList[num].VertexNormal[0].x = x;
                           CurMesh->TriangleList[num].VertexNormal[0].y = y;
                           CurMesh->TriangleList[num].VertexNormal[0].z = z;
 
                           linecount++;
                           fgets(line, 256, fp);//*MESH_VERTEXNORMAL
                           sscanf(line, "%s%d%f%f%f", string, &num,&x,&z,&y);
                           CurMesh->TriangleList[num].VertexNormal[2].x = x;
                           CurMesh->TriangleList[num].VertexNormal[2].y = y;
                           CurMesh->TriangleList[num].VertexNormal[2].z = z;
                           
                           linecount++;
                           fgets(line, 256, fp);//*MESH_VERTEXNORMAL
                           sscanf(line, "%s%d%f%f%f", string, &num,&x,&z,&y);
                           CurMesh->TriangleList[num].VertexNormal[1].x = x;
                           CurMesh->TriangleList[num].VertexNormal[1].y = y;
                           CurMesh->TriangleList[num].VertexNormal[1].z = z;
 
                           counter++;
             }
             linecount++;
             fgets(line, 256, fp);
             return 0;
}
이제 다 읽어들였다. 설명보다 완전히 코드분석위주로 가서 정신이 없을지도 모르지만, 이 부분은 이론 같은 부분보다는 완벽 노가다 코딩부분이라 어쩔 수 없다.
그렇다면 이렇게 열심히 읽은 모델을 어떻게 출력하는지 알아보자. 


*/


class GdsMaterialProperty : public GdsProperty
{
	D3DMATERIAL9		m_Material;
};


#endif