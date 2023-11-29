//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "DeviceResources.h"

namespace D2DGradientMesh
{
    // This sample renderer instantiates a basic rendering pipeline.
    class D2DGradientMeshRenderer
    {
    public:
        D2DGradientMeshRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;

        // Sample-specific resources.
        Microsoft::WRL::ComPtr<ID2D1GradientMesh> m_gradientMesh;
    };

    class D2DDashLineRenderer
    {
    public:
        D2DDashLineRenderer(
            const std::shared_ptr<DX::DeviceResources>& deviceResources,
            float dashOffset,
            std::pair<float, float> begin,
            std::pair<float, float> end
        );
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        float m_dashOffset;
        std::pair<float, float> m_begin;
        std::pair<float, float> m_end;
        // Sample-specific resources.
        Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_strokeStyle;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
    };


    class D2DDashCircleRenderer
    {
    public:
        D2DDashCircleRenderer(
            const std::shared_ptr<DX::DeviceResources>& deviceResources,
            std::pair<float, float> center,
            float radius
        );
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        std::pair<float, float> m_center;
        float m_radius;
        // Sample-specific resources.
        Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_strokeStyle;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
    };

}
