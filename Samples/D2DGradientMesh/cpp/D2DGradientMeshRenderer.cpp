//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "D2DGradientMeshRenderer.h"

#include "DirectXHelper.h"

using namespace D2DGradientMesh;

using namespace DirectX;
using namespace Windows::Foundation;

// Creates a DWriteTextFormat with our text properties. This includes setting our flow and reading direction.
D2DGradientMeshRenderer::D2DGradientMeshRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

// Initialization.
void D2DGradientMeshRenderer::CreateDeviceDependentResources()
{
    // Create an array that will hold the patch data. In this case, we have two patches.
    D2D1_GRADIENT_MESH_PATCH patchList[2];

    // Create the first patch data item using the helper function.
    patchList[0] = D2D1::GradientMeshPatch(
        D2D1::Point2F(-210, 90),            // Point00 (bottom left corner)
        D2D1::Point2F(-150, 30),            // Point01
        D2D1::Point2F(-90, 150),            // Point02
        D2D1::Point2F(-30, 90),             // Point03 (bottom right corner)
        D2D1::Point2F(-190, 30),            // Point10
        D2D1::Point2F(-130, 30),            // Point11
        D2D1::Point2F(-70, 30),             // Point12
        D2D1::Point2F(-10, 30),             // Point13
        D2D1::Point2F(-170, -30),           // Point20
        D2D1::Point2F(-110, -30),           // Point21
        D2D1::Point2F(-50, -30),            // Point22
        D2D1::Point2F(10, -30),             // Point23
        D2D1::Point2F(-150, -90),           // Point30 (upper left corner)
        D2D1::Point2F(-90, -150),           // Point31
        D2D1::Point2F(-30, -30),            // Point32
        D2D1::Point2F(30, -90),             // Point33 (upper right corner)
        D2D1::ColorF(D2D1::ColorF::Black),  // Color00 (bottom left corner)
        D2D1::ColorF(D2D1::ColorF::Blue),   // Color03 (bottom right corner)
        D2D1::ColorF(D2D1::ColorF::Purple), // Color30 (upper left corner)
        D2D1::ColorF(D2D1::ColorF::Red),    // Color33 (upper right corner)
        D2D1_PATCH_EDGE_MODE_ANTIALIASED,   // Top edge is a boundary of the mesh
        D2D1_PATCH_EDGE_MODE_ANTIALIASED,   // Left edge is a boundary of the mesh
        D2D1_PATCH_EDGE_MODE_ANTIALIASED,   // Bottom edge is a boundary of the mesh
        D2D1_PATCH_EDGE_MODE_ALIASED        // Right edge is internal to the mesh
        );

    // The second patch is the same as the first but translated by x.
    const int x = 180;

    // Create the second patch data item using the helper function.
    patchList[1] = D2D1::GradientMeshPatch(
        D2D1::Point2F(-210 + x, 90),        // Point00 (bottom left corner)
        D2D1::Point2F(-150 + x, 30),        // Point01
        D2D1::Point2F(-90 + x, 150),        // Point02
        D2D1::Point2F(-30 + x, 90),         // Point03 (bottom right corner)
        D2D1::Point2F(-190 + x, 30),        // Point10
        D2D1::Point2F(-130 + x, 30),        // Point11
        D2D1::Point2F(-70 + x, 30),         // Point12
        D2D1::Point2F(-10 + x, 30),         // Point13
        D2D1::Point2F(-170 + x, -30),       // Point20
        D2D1::Point2F(-110 + x, -30),       // Point21
        D2D1::Point2F(-50 + x, -30),        // Point22
        D2D1::Point2F(10 + x, -30),         // Point23
        D2D1::Point2F(-150 + x, -90),       // Point30 (upper left corner)
        D2D1::Point2F(-90 + x, -150),       // Point31
        D2D1::Point2F(-30 + x, -30),        // Point32
        D2D1::Point2F(30 + x, -90),         // Point33 (upper right corner)
        D2D1::ColorF(D2D1::ColorF::Blue),   // Color00 (bottom left corner)
        D2D1::ColorF(D2D1::ColorF::Yellow), // Color03 (bottom right corner)
        D2D1::ColorF(D2D1::ColorF::Red),    // Color30 (upper left corner)
        D2D1::ColorF(D2D1::ColorF::Green),  // Color33 (upper right corner)
        D2D1_PATCH_EDGE_MODE_ANTIALIASED,   // Top edge is a boundary of the mesh
        D2D1_PATCH_EDGE_MODE_ALIASED,       // Left edge is internal to the mesh
        D2D1_PATCH_EDGE_MODE_ANTIALIASED,   // Bottom edge is a boundary of the mesh
        D2D1_PATCH_EDGE_MODE_ANTIALIASED    // Right edge is a boundary of the mesh
        );

    // Create the gradient mesh object from the patch data.
    DX::ThrowIfFailed(
        m_deviceResources->GetD2DDeviceContext()->CreateGradientMesh(
            patchList,
            ARRAYSIZE(patchList),
            &m_gradientMesh
            )
        );
}

// Initialization.
void D2DGradientMeshRenderer::CreateWindowSizeDependentResources()
{
}

// Release all references to resources that depend on the graphics device.
// This method is invoked when the device is lost and resources are no longer usable.
void D2DGradientMeshRenderer::ReleaseDeviceDependentResources()
{
    m_gradientMesh.Reset();
}

// Renders one frame.
void D2DGradientMeshRenderer::Render()
{
    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    // Translate to the center of the window.
    m_deviceResources->GetD2DDeviceContext()->SetTransform(
        D2D1::Matrix3x2F::Translation(logicalSize.Width / 2.0f, logicalSize.Height / 2.0f) *
        m_deviceResources->GetOrientationTransform2D()
        );

    // Draw the gradient mesh.
    m_deviceResources->GetD2DDeviceContext()->DrawGradientMesh(m_gradientMesh.Get());

}

D2DDashLineRenderer::D2DDashLineRenderer(
    const std::shared_ptr<DX::DeviceResources>& deviceResources,
    float dashOffset,
    std::pair<float, float> begin,
    std::pair<float, float> end
):
m_deviceResources(deviceResources),
m_dashOffset(dashOffset),
m_begin(begin),
m_end(end)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void D2DDashLineRenderer::CreateDeviceDependentResources()
{
    m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_solidBrush);

    D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties;
    strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_DASH;
    strokeStyleProperties.dashOffset = m_dashOffset;
    strokeStyleProperties.dashCap = D2D1_CAP_STYLE_FLAT;
    strokeStyleProperties.miterLimit = 10.0f;
    strokeStyleProperties.lineJoin = D2D1_LINE_JOIN_ROUND;
    strokeStyleProperties.startCap = D2D1_CAP_STYLE_FLAT;
    strokeStyleProperties.endCap = D2D1_CAP_STYLE_FLAT;

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DFactory()->CreateStrokeStyle(&strokeStyleProperties, NULL, 0, &m_strokeStyle)
    );
}

void D2DDashLineRenderer::CreateWindowSizeDependentResources()
{

}

void D2DDashLineRenderer::ReleaseDeviceDependentResources()
{
    m_strokeStyle.Reset();
    m_solidBrush.Reset();
}

void D2DDashLineRenderer::Render()
{


    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    // Translate to the center of the window.
    m_deviceResources->GetD2DDeviceContext()->SetTransform(
        D2D1::Matrix3x2F::Translation(logicalSize.Width / 2.0f, logicalSize.Height / 2.0f) *
        m_deviceResources->GetOrientationTransform2D()
    );

    // Draw the gradient mesh.
    m_deviceResources->GetD2DDeviceContext()->DrawLine(
        { m_begin.first,  m_begin.second },
        { m_end.first,  m_end.second },
        *m_solidBrush.GetAddressOf(),
        2.0f,
        *m_strokeStyle.GetAddressOf());


}


D2DDashCircleRenderer::D2DDashCircleRenderer(
    const std::shared_ptr<DX::DeviceResources>& deviceResources,
    std::pair<float, float> center,
    float radius
):
    m_deviceResources(deviceResources),
    m_radius(radius),
    m_center(center)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
void D2DDashCircleRenderer::CreateDeviceDependentResources()
{
    m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_solidBrush);

    D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties;
    strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_DASH;
    strokeStyleProperties.dashOffset = 0.0f;
    strokeStyleProperties.dashCap = D2D1_CAP_STYLE_FLAT;
    strokeStyleProperties.miterLimit = 10.0f;
    strokeStyleProperties.lineJoin = D2D1_LINE_JOIN_ROUND;
    strokeStyleProperties.startCap = D2D1_CAP_STYLE_FLAT;
    strokeStyleProperties.endCap = D2D1_CAP_STYLE_FLAT;

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DFactory()->CreateStrokeStyle(&strokeStyleProperties, NULL, 0, &m_strokeStyle)
    );
}

void D2DDashCircleRenderer::CreateWindowSizeDependentResources()
{
}

void D2DDashCircleRenderer::ReleaseDeviceDependentResources()
{
    m_strokeStyle.Reset();
    m_solidBrush.Reset();
}

void D2DDashCircleRenderer::Render()
{

    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    // Translate to the center of the window.
    m_deviceResources->GetD2DDeviceContext()->SetTransform(
        D2D1::Matrix3x2F::Translation(logicalSize.Width / 2.0f, logicalSize.Height / 2.0f) *
        m_deviceResources->GetOrientationTransform2D()
    );

    // Draw the gradient mesh.
    m_deviceResources->GetD2DDeviceContext()->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(m_center.first, m_center.second), m_radius, m_radius), *m_solidBrush.GetAddressOf(), 2.0f, *m_strokeStyle.GetAddressOf());

}

