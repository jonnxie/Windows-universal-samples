#include "pch.h"
#include "D2DLightLineRenderer.h"

#include "DirectXHelper.h"
#include <windows.ui.xaml.media.dxinterop.h>

using namespace D2DTest;

using namespace DirectX;
using namespace Windows::Foundation;
using namespace D2D1;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

D2DLightLineRenderer::D2DLightLineRenderer(
        const std::shared_ptr<DX::DeviceResources>& deviceResources,
        std::pair<float, float> begin,
        std::pair<float, float> end,
        float glowSize
):
m_deviceResources(deviceResources),
m_begin(begin),
m_end(end),
m_glowSize(glowSize)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void D2DLightLineRenderer::CreateDeviceDependentResources()
{
    D2D1_BITMAP_PROPERTIES bitmapProperties =
        D2D1::BitmapProperties(
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_deviceResources->GetDPI(),
            m_deviceResources->GetDPI()
        );

    DXGI_SWAP_CHAIN_DESC desc;
    DX::ThrowIfFailed(
        m_deviceResources->GetSwapChain()->GetDesc(&desc)
    );

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DDeviceContext()->CreateBitmap(
            D2D1_SIZE_U{ desc.BufferDesc.Width , desc.BufferDesc.Height },
            *(& bitmapProperties),
            &m_sourceBitmap
        )
    );

    ID2D1Effect* effect;
    if (SUCCEEDED(m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1GaussianBlur, &effect)))
    {
        effect->SetInput(0, *m_sourceBitmap.GetAddressOf());
        effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_glowSize);

        ID2D1Effect* compositeEffect;
        if (SUCCEEDED(m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1Composite, &compositeEffect)))
        {
            compositeEffect->SetInputEffect(0, effect);
            compositeEffect->SetInput(1, *m_sourceBitmap.GetAddressOf());

            ID2D1Effect* blendEffect;
            if (SUCCEEDED(m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1Blend, &blendEffect)))
            {
                blendEffect->SetInputEffect(0, compositeEffect);
                blendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_SCREEN);
                blendEffect->SetValue(D2D1_BLEND_PROP_FORCE_DWORD, D2D1_PROPERTY_FORCE_DWORD);

                ID2D1Effect* colorEffect;
                if (SUCCEEDED(m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1ColorManagement, &colorEffect)))
                {
                    colorEffect->SetInputEffect(0, blendEffect);
                    colorEffect->SetValue(D2D1_COLORMANAGEMENT_PROP_SOURCE_COLOR_CONTEXT, nullptr);
                    colorEffect->SetValue(D2D1_COLORMANAGEMENT_PROP_DESTINATION_COLOR_CONTEXT, nullptr);
                    colorEffect->SetValue(D2D1_COLORMANAGEMENT_PROP_FORCE_DWORD, D2D1_PROPERTY_FORCE_DWORD);

                    if (SUCCEEDED(m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1Composite, &m_glowEffect)))
                    {
                        m_glowEffect->SetInputEffect(0, colorEffect);
                        m_glowEffect->SetInput(1, *m_sourceBitmap.GetAddressOf());
                        m_glowEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_OVER);
                    }
                }
            }
        }
    }
}

void D2DLightLineRenderer::CreateWindowSizeDependentResources()
{

}

void D2DLightLineRenderer::ReleaseDeviceDependentResources(){
    m_strokeStyle.Reset();
    m_solidBrush.Reset();
    m_glowEffect.Reset();
}

void D2DLightLineRenderer::Render(){

}





























