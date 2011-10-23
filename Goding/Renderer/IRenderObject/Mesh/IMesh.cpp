#include "stdafx.h"
#include "IMesh.h"

IMesh::IMesh()
{
	vertexBuffer = NULL;
	indexBuffer = NULL;
}

IMesh::~IMesh()
{

}

void IMesh::onDeviceReset()
{

}

void IMesh::onDeviceLost()
{

}

void IMesh::Draw( D3DDevice* pDevice , D3DXEffect* pEffect )
{

}