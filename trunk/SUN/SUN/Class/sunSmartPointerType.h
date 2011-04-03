#ifndef _SUN_CLASS_SMARTPOINTERTYPE_H_
#define _SUN_CLASS_SMARTPOINTERTYPE_H_


class sunNode;
class sunRenderNode;
class sunResource;
class sunMesh;
class sunCamera;
class sunParserXML;


typedef sunCSmartPointer<sunNode>			sunNodePtr;
typedef sunCSmartPointer<sunRenderNode>		sunRenderNodePtr;
typedef sunCSmartPointer<sunResource>		sunResourcePtr;
typedef sunCSmartPointer<sunMesh>			sunMeshPtr;
typedef sunCSmartPointer<sunCamera>			sunCameraPtr;
typedef sunCSmartPointer<sunParserXML>		sunParserXMLPtr;

#endif