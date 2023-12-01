#pragma once
#include "DeviceResources.h"

namespace D2DTest
{
    class D2DLightLineRenderer
    {
    public:
        D2DLightLineRenderer(
                const std::shared_ptr<DX::DeviceResources>& deviceResources,
                std::pair<float, float> begin,
                std::pair<float, float> end,
                float glowSize
        );
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        Microsoft::WRL::ComPtr<ID2D1Bitmap> m_sourceBitmap;

        std::pair<float, float> m_begin;
        std::pair<float, float> m_end;
        float m_glowSize;
        // Sample-specific resources.
        Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_strokeStyle;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
        Microsoft::WRL::ComPtr<ID2D1Effect> m_glowEffect;
    };
}

