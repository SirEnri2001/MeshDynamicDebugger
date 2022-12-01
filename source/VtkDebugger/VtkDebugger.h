#pragma once

#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPLYReader.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkLine.h>
#include "json.hpp"

#define EVENT_UPDATE_OFFSET 101

using json = nlohmann::json;


class VtkBaseCallback {
public:
	virtual void executeCallbackBehavior() = 0;
};

class VtkVerticesCallback : public VtkBaseCallback {
public:
	VtkVerticesCallback(
		vtkPoints* points,
		vtkPolyData* verticesData,
		vtkVertexGlyphFilter* vertexGlyphFilter,
		vtkPolyDataMapper* verticesMapper)
	{
		this->points = points;
		this->verticesData = verticesData;
		this->verticesMapper = verticesMapper;
		this->vertexGlyphFilter = vertexGlyphFilter;
	}
	void executeCallbackBehavior() override {
		vertexGlyphFilter->RemoveAllInputs();
		vertexGlyphFilter->AddInputData(verticesData);
	}
private:
	vtkPoints* points;
	vtkPolyData* verticesData;
	vtkPolyDataMapper* verticesMapper;
	vtkVertexGlyphFilter* vertexGlyphFilter;
};

class VtkEdgesCallback : public VtkBaseCallback {
public:
	VtkEdgesCallback(vtkSmartPointer<vtkPolyData> linesPolyData,
		vtkSmartPointer<vtkCellArray> lines, vtkSmartPointer<vtkPoints> linePts) {
		this->linePts = linePts;
		this->lines = lines;
		this->linesPolyData = linesPolyData;
	}
	void executeCallbackBehavior() override {
		cout << linesPolyData->GetNumberOfCells() << endl;
		// Add the points to the polydata container
		linesPolyData->SetPoints(linePts);
		// Add the lines to the polydata container
		linesPolyData->SetLines(lines);
	}
	vtkSmartPointer<vtkPolyData> linesPolyData;
	vtkSmartPointer<vtkCellArray> lines;
	vtkSmartPointer<vtkPoints> linePts;
};

class VtkMeshCallback : public VtkBaseCallback {
public:
	VtkMeshCallback(vtkPLYReader* reader, vtkPolyDataMapper* mapper, bool* updatedModel)
		: m_Reader(reader), m_Mapper(mapper), m_updatedModel(updatedModel) {
		activated = false;
	}
	void executeCallbackBehavior() override {
		if (*m_updatedModel && m_Reader->GetOutput() != nullptr) {
			if (!activated) {
				m_Mapper->SetInputConnection(m_Reader->GetOutputPort());
				activated = true;
			}
			m_Mapper->Update();
			m_Reader->Update();
		}
		*m_updatedModel = false;
	}
private:
	vtkPLYReader* m_Reader;
	vtkPolyDataMapper* m_Mapper;
	bool* m_updatedModel;
	bool activated;
};

class VtkDebugger {
public:
	void HighlightVertex(double* v); // array v with 3 elements
	void HighlightEdge(double* v1, double* v2); // array v1, v2 with 3 elements each
	void SetModel(std::string fileInMem); // .ply path on sharedmem
	void Update(); // update the debugger scene
	void Start(); // start the debugger scene
	
	VtkDebugger();
private:
	// Universal Fields
	vtkSmartPointer<vtkNamedColors> namedColors = vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkWin32OpenGLRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	
	// Vertices pipeline fields
	vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPolyData> verticesData = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	vtkSmartPointer<vtkPolyDataMapper> verticesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> verticesActor = vtkSmartPointer<vtkActor>::New();

	// Edges pipeline fields
	vtkNew<vtkPolyData> linesPolyData;
	vtkNew<vtkCellArray> lines;
	vtkNew<vtkPoints> linePts;
	vtkSmartPointer<vtkPolyDataMapper> edgesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> edgesActor = vtkSmartPointer<vtkActor>::New();
	int linePtsIdx;

	// Mesh pipeline fields
	vtkSmartPointer<vtkPLYReader> meshReader = vtkSmartPointer<vtkPLYReader>::New();
	vtkSmartPointer<vtkPolyDataMapper> meshMapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> meshActor = vtkSmartPointer<vtkActor>::New();

	VtkVerticesCallback* verticesCbk;
	VtkMeshCallback* meshCbk;
	VtkEdgesCallback* edgesCbk;

	bool updatedVertices;
	bool updatedEdges;
	bool updatedModel;
	
};