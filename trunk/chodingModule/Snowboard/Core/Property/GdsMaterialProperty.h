#ifndef _GDS_CORE_PROPERTY_MATERIAL_H_
#define _GDS_CORE_PROPERTY_MATERIAL_H_

#include "GdsProperty.h"

/*
 
struct Material
{
             bool bUseTexture; // �ؽ��� �������
             bool bUseOpacity; // Opacity map �������
             char TextureName[256]; 
             int SubMaterialCount;
             Material* SubMaterial;
             LPDIRECT3DTEXTURE8 pTexture; // �ؽ���
             D3DMATERIAL8 d3dMaterial; // d3d ���� ��ü
             CTextureContainer* m_pTextureContainer; // �ؽ��� ���� class
 
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
D3D�� Max�� ��� ���� ������ ��� �� ���� ����. API��ü�� �Ѱ��̱⵵ ������, �ӵ��� ������ �� ���� �̿ܿ��� ��� �� ���ۿ� ���� �κ��̴�. �������� ��� �Ұ��� ��� Max�� ���� ���� �� Texture ������ �̸��� ������, �׸��� Alpha channel��������������� ������ �´�. ���� Decoding ��ƾ�� ����.
 
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
// Max�� ���������̶� ��Ư�� ������ �����ϹǷ�, �� �˾Ƶ���.
                                        sscanf(line,"%s%d",string, &num);
                                        DecodeMaterial(fp,&(CurMaterial->SubMaterial[num]));
                                        continue;
                           }
                           if(_strcmpi(string,"*NUMSUBMTLS")==0)
                           {
                                        if(CurMaterial->SubMaterial)
                                                     delete CurMaterial->SubMaterial;
                                        sscanf(line,"%s%d",string,&CurMaterial->SubMaterialCount);
// ������ ������ ������ �� �� �����Ƿ� �迭�� �����Ѵ�.
                                        CurMaterial->SubMaterial = 
                                        new Material[CurMaterial->SubMaterialCount];
                                        continue;
                           }
// ���� ������ ������ �˾Ƴ���.                  
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
// �ؽ��İ� ���Ǿ����� �̷� ���� ���´�.                           
                           if(_strcmpi(string, "*MAP_DIFFUSE")==0)
                           {
// �ؽ��� ��� �÷��׸� On!!!
                                        CurMaterial->bUseTexture = true;
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// ����ä���� ����ϸ� ���´�.
                           if(_strcmpi(string, "*MAP_OPACITY")==0)
                           {
// Opacity ���� ���Ǹ�, ����ä���� �ִ� �ؽ���(TGA���˵�)�� ���� ���̴�.
                                        CurMaterial->bUseTexture = true;
                                        CurMaterial->bUseOpacity = true;
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// �̷� �͵� �ִµ� �ǽð� ���������� ���������̴�.
                           if(_strcmpi(string, "*MAP_SELFILLUM")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// ���� �������. �����̳ʿ��� ���� ����� ����Ѵ�.
                           if(_strcmpi(string, "*MAP_REFLECT")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// ���� ��� ���Ѵ�. ���� Decode�� �ؼ� ���� �ѱ�� ���̴�.
                           if(_strcmpi(string, "*MAP_SPECULAR")==0)
                           {
                                        DecodeMap(fp, CurMaterial);
                                        continue;
                           }
// ASE�� ������ ������ }(��ȣ)�� ���ƹ�����. �̰� ������ �ش籸��ü�� �� �����Ŵ�.
                           if(_stricmp(string, "}")==0)
                           {
                                        break;
                           }
             } while (1);
             return 0;
}
�ؽ��� ���ο� ���� ��Ȳ�� ������ �ִ� �ʺκ�������, ������ �о ����ϴ� �κ��� �ؽ��� ������ �̸����̴�. �׷��Ƿ�, �ؽ��� ���� �̸����� �о���̰��� '}'�� ���� ������ ������ ������ ���� �ȴ�. �Ѱ��� ������ ���̶��, �����̳ʵ��� ��κ� �����̸��̳� ���丮�̸��� ������ ���� ����ϴµ�, sscanf�� ��������� �Ľ��ϴ� ������ �ִ�. �׷��Ƿ�, �����̸��� Ư���� ���������� �Ľ��� ����� �Ѵ�.
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
// ���Ⱑ ���������� �Ľ��ϴ� �κ�, ��ũ��Ʈ���� ���� �ٷ� �κ��̴�.
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
������ �ؽ��ĸ� �����ϸ� �׸� �߿��� ������ �����Ƿ� ���� ��������, Mesh�� ��� ������ �������� ���¸� ��Ÿ���� ��ŭ ����� ��ƴ�. ���⼭�� �ϴ� �޽ø� �д� �κп� ���ؼ��� �����ϰ� �ڿ��� �ִϸ��̼� class�� �ִϸ��̼� �����͸� �д� ���� �����ϰڴ�.
 
������ ��� �����Ѵ�� �޽õ����ʹ� �� �κ����� ���������ִ�.  �޽� ������ Transform ��� ����, ���� ����, �� ����, �׸��� Animation ������ �̷���� �ִ�. �̷��� ������ �����ϴ� Mesh class�� ������ ����.
 
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
VertexList �� ���� �޽��� ������ ������ ��ġ������ ������ �ִ�. 300���� �������� �̷���� �޽ö�� ���� VertexList���� 300���� ������ ������ �ִ�. ������, �� �������������δ� �޽÷� ����� �� ����. ������ �� �������� � ������ ���� �̷������ ���� ������ ���� �����̴�. ���� �鿡 ���� ��� ������ TriangleList�� ������ �ȴ�.
 
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
TRIANGLE ����ü���� ���� �̷�� �� ������ ������ ������ �ִ�. ���� �� ������ �������� Color��, �׸��� Texture�� u,v��ǥ, ������ ������ ��� �ִ�. ȭ�鿡 ������ ����ϴ� ���� �� �� ������ ������ ����ϰ� �ȴ�. ���� ������ ���Ͽ��� �о���̴� �κ��� �ҽ��� �Ⱦ���� ����.
 
int CAse::DecodeGEOMOBJECT(FILE *fp)
{
             int num;
             int i,j;
             bool check;
             do{
                           linecount ++;
                           fgets(line, 256, fp);//*NODE_NAME
                           sscanf(line,"%s",string);
// ���� �޽��� �̸��� �����Ѵ�. �޽��� �̸��� ���������� ���� �� �� �ʿ��ϹǷ�, �� �����ؾ��Ѵ�. ���� ������ ���� ��찡 �����Ƿ� �̰��� ���ο� �ΰ� �о���δ�.
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
// ���������� ��̳��(��̸޽�) �� �̸��� �о �����Ѵ�.
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
TM�� ������ ���Ѵ�� Max��ǥ�踦 D3D��ǥ��� �ٲ��ָ鼭 �о���̸� �ȴ�. ����� �ռ� ������ ��� y�� z���� ���� �ٲ��ָ� �ȴ�.
 
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
// �д� ������ �����ϱ� �ٶ���.
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
// �������� ������� �ٲ��ش�.
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
���� ������ �� ������ �о���̴� �κ��̴�. 
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
// ģ���ϰԵ� �� ���� ������ �ִ��� �˷��ֱ� ������ ������ �迭�� �о���ϼ��ִ�.
             CurMesh->VertexList = new D3DXVECTOR3[VCOUNT];
 
             int TCOUNT;
             linecount++;
             fgets(line, 256, fp);//*MESH_NUMFACES
             sscanf(line, "%s%d", string, &TCOUNT);
// ���� ������ �˷��ش�.
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
// Texture u,v��ǥ�� ���� �κ��� �о���δ�.
                                        DecodeMESH_TVERTLIST(fp);
                                        continue;
                           }
                           if(_stricmp(string, "*MESH_NUMTVFACES")==0)
                           {
// �о���� u,v��ǥ���� ���� � �鿡 ����Ǵ����� �о��ش�.
                                        DecodeMESH_TFACELIST(fp);
                                        continue;
                           }            
                           if ( _stricmp(string, "*MESH_NUMCVERTEX") == 0)
                           {
// ���� ������ �ִ� ��� �о��ش�. ������ �� �������� �ʴ´�.
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
// ���� ������ ������� �о��ָ� �ȴ�. ��, ��ǥ�� ��ȯ�� �������� �ȵȴ�.
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
// Material ID�� �ش� ���� ������ ����(index)�ε�, �̰��� ���� Submaterial�� �����̴�.
// �޽���ü���� Material Ref�� �Ͽ� ������ ������ �����Ѵ�. ASE���˿����� �� �鸶�� �ٸ� ������ ���� �� �ֱ� ������, �޽� �������� �� Submaterial�� ���θ� �� �鸶�� ������ �� �ִ� ���̴�. 
// ���� Smooth������ ������ �ǰ� �ϱ� ������ ��¿ �� ���� �˻縦 �ؾ��Ѵ�.
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
// �ؽ��� u,v��ǥ�� �����Ѵ�. 
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
// ���� ����� Texture u,v�� � �鿡 � ������ ���ϴ� �������� �о���δ�.
// �̶�, CounterClock Wise�� ��ȯ�ϱ� ���ؼ� 0, 2, 1 �� ������ �����Ѵ�.
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
// ���� ���� ���� �κ����� Texture�� �д� �κа� ���� ����ϴ�.
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
// ���� ���� ������ �� ������ ������ �о���δ�.
// ���� �����̶� � �鿡 ���� �������� ���� �� ������ ������ �ٲ�� �ȴ�.
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
���� �� �о�鿴��. ������ ������ �ڵ�м����ַ� ���� ������ �������� ������, �� �κ��� �̷� ���� �κк��ٴ� �Ϻ� �밡�� �ڵ��κ��̶� ��¿ �� ����.
�׷��ٸ� �̷��� ������ ���� ���� ��� ����ϴ��� �˾ƺ���. 


*/


class GdsMaterialProperty : public GdsProperty
{
	D3DMATERIAL9		m_Material;
};


#endif