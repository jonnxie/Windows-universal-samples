//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "D2DGradientMeshMain.h"

#include <DirectXColors.h>
#include "DirectXHelper.h"

using namespace D2DGradientMesh;

// Loads and initializes application assets when the application is loaded.
D2DGradientMeshMain::D2DGradientMeshMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
    // Register to be notified if the Device is lost or recreated.
    m_deviceResources->RegisterDeviceNotify(this);
    std::string programPath = "D:\\wecad\\x64_release\\VisualCad.exe";
    DX::addToHardwareAccelerationList(programPath);

    m_sceneRenderer = std::unique_ptr<D2DGradientMeshRenderer>(new D2DGradientMeshRenderer(m_deviceResources));
    m_lineRenderer = std::unique_ptr<D2DDashLineRenderer>(new D2DDashLineRenderer(
        m_deviceResources,
        0,
        {0,90},
        {240,90}
    ));
    m_circleRenderer = std::unique_ptr<D2DDashCircleRenderer>(new D2DDashCircleRenderer(
        m_deviceResources,
        { 120,90 },
        60
    ));

    m_sampleOverlay = std::unique_ptr<SampleOverlay>(new SampleOverlay(m_deviceResources, L"Direct2D Gradient Mesh Sample"));
}

D2DGradientMeshMain::~D2DGradientMeshMain()
{
    // Deregister device notification.
    m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void D2DGradientMeshMain::UpdateForWindowSizeChange()
{
    m_sceneRenderer->CreateWindowSizeDependentResources();
    m_lineRenderer->CreateWindowSizeDependentResources();
    m_circleRenderer->CreateWindowSizeDependentResources();
    m_sampleOverlay->CreateWindowSizeDependentResources();
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool D2DGradientMeshMain::Render()
{

    m_deviceResources->GetD2DDeviceContext()->BeginDraw();

    m_deviceResources->GetD2DDeviceContext()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
    // Render the scene objects.
    m_sceneRenderer->Render();
    m_lineRenderer->Render();
    m_circleRenderer->Render();

    m_sampleOverlay->Render();

    HRESULT hr = m_deviceResources->GetD2DDeviceContext()->EndDraw();
    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }
    return true;
}

// Notifies renderers that device resources need to be released.
void D2DGradientMeshMain::OnDeviceLost()
{
    m_sceneRenderer->ReleaseDeviceDependentResources();
    m_lineRenderer->ReleaseDeviceDependentResources();
    m_circleRenderer->ReleaseDeviceDependentResources();
    m_sampleOverlay->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be re-created.
void D2DGradientMeshMain::OnDeviceRestored()
{
    m_sceneRenderer->CreateDeviceDependentResources();
    m_lineRenderer->CreateDeviceDependentResources();
    m_circleRenderer->CreateDeviceDependentResources();
    m_sampleOverlay->CreateDeviceDependentResources();
    UpdateForWindowSizeChange();
}
