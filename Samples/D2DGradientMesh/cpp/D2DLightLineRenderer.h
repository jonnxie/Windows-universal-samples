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
                float glowSize,
                float strokeWidth
        );
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Render();

    private:
        void CreateEffect();
        void CreateGeometry();
        void CreateTargetBitmap();
    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        Microsoft::WRL::ComPtr<ID2D1Bitmap> m_sourceBitmap;
        Microsoft::WRL::ComPtr<ID2D1Bitmap> m_targetAssociateBitmap;
        Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> m_targetBitmap;

        std::pair<float, float> m_begin;
        std::pair<float, float> m_end;
        float m_glowSize;
        float m_strokeWidth;
        // Sample-specific resources.
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_redSolidBrush;
        Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
        Microsoft::WRL::ComPtr<ID2D1Effect> m_glowEffect;

        Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pathGeometry;

    };
}

