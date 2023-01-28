#include "VtkDebugger.h"

#include <Windows.h>

class VtkCallback : public vtkCommand
{
public:
	VtkCallback(vtkSmartPointer<vtkRenderWindow> window)
	{
		this->window = window;
	}

	void Execute(vtkObject*, unsigned long event, void*) override
	{
		//window->Render();
	}

private:
	vtkSmartPointer<vtkRenderWindow> window;
};

VtkDebugger::VtkDebugger() {
	updatedModel = false;
	updatedVertices = false;
	updatedEdges = false;
	linePtsIdx = 0;

	// Vertices preprocessing
	verticesData->SetPoints(m_Points);
	vertexGlyphFilter->AddInputData(verticesData);
	verticesMapper->SetInputConnection(vertexGlyphFilter->GetOutputPort());
	verticesCbk = new VtkVerticesCallback(
		m_Points,
		verticesData,
		vertexGlyphFilter,
		verticesMapper
	);
	verticesActor->GetProperty()->SetPointSize(10);
	verticesActor->GetProperty()->SetColor(namedColors->GetColor3d("Orange").GetData());
	verticesActor->SetMapper(verticesMapper);
	renderer->AddActor(verticesActor);

	// Edge preprocessing
	edgesMapper->SetInputData(linesPolyData);
	edgesCbk = new VtkEdgesCallback(
		linesPolyData,
		lines,
		linePts
	);
	edgesActor->GetProperty()->SetLineWidth(5);
	edgesActor->GetProperty()->SetColor(namedColors->GetColor3d("Tomato").GetData());
	edgesActor->SetMapper(edgesMapper);
	edgesActor->GetProperty()->EdgeVisibilityOn();

	// Mesh preprocessing
	meshReader->ReadFromInputStringOn();
	meshMapper->SetInputData(meshData);
	meshCbk = new VtkMeshCallback(
		meshData, meshMapper, &updatedModel);
	meshActor->SetMapper(meshMapper);
	meshActor->GetProperty()->EdgeVisibilityOn();
	renderer->AddActor(meshActor);

	VtkCallback* cbk = new VtkCallback(renderWindow);
	interactor->AddObserver(vtkCommand::UserEvent, cbk);
	// Visualize
	renderWindow->SetWindowName("VTK Debugger by SirEnri");
	renderWindow->AddRenderer(renderer);
	interactor->SetRenderWindow(renderWindow);
	renderer->SetBackground(namedColors->GetColor3d("SlateGray").GetData());
	renderer->AddActor(edgesActor);
	renderWindow->Render();
}

void VtkDebugger::Start() {
	interactor->Start();
}

void VtkDebugger::HighlightVertex(double* v)
{
	cout << "COMMAND: HighlightVertex()" << endl;
	if (!updatedVertices) {
		m_Points->Reset();
		verticesData->Reset();
		updatedVertices = true;
	}
	m_Points->InsertNextPoint(v[0], v[1], v[2]);
}

void VtkDebugger::HighlightEdge(double* v1, double* v2) {
	cout << "COMMAND: HighlightEdge()" << endl;
	if (!updatedEdges) {
		linePts->Reset();
		lines->Reset();
		updatedEdges = true;
	}
	
	linePts->InsertNextPoint(v1);
	linePts->InsertNextPoint(v2);
	// Create the first line (between Origin and P0)
	vtkNew<vtkLine> line0;
	line0->GetPointIds()->SetId(0, linePtsIdx); // the second 0 is the index of the Origin in linesPolyData's points
	line0->GetPointIds()->SetId(1, linePtsIdx+1); // the second 1 is the index of P0 in linesPolyData's points
	linePtsIdx += 2;
	// Create a vtkCellArray container and store the lines in it
	lines->InsertNextCell(line0);
}

void VtkDebugger::SetModel(std::string fileInMem)
{
	//meshReader->RemoveAllInputs();
	meshReader = vtkSmartPointer<vtkPLYReader>::New();
	meshReader->ReadFromInputStringOn();
	meshReader->SetInputString(fileInMem);
	meshReader->Update();
	meshMapper->SetInputData(meshReader->GetOutput());
	cout << "COMMAND: SetModel()" << endl;
	updatedModel = true;
}

void VtkDebugger::Update() {
	//interactor->InvokeEvent(vtkCommand::UserEvent + EVENT_UPDATE_OFFSET);
	cout << "COMMAND: Update()" << endl;
	if (!updatedVertices) {
		m_Points->Reset();
		verticesData->Reset();
		updatedVertices = true;
	}
	if (!updatedEdges) {
		linePts->Reset();
		lines->Reset();
		updatedEdges = true;
	}
	verticesCbk->executeCallbackBehavior();
	edgesCbk->executeCallbackBehavior();
	meshCbk->executeCallbackBehavior();
	// DO NOT CALL Render() HERE! WILL FREEZE OTHERWISE
	//renderWindow->Render();
	interactor->InvokeEvent(vtkCommand::UserEvent);
	updatedModel = false;
	updatedVertices = false;
	updatedEdges = false;
	linePtsIdx = 0;
	
}

