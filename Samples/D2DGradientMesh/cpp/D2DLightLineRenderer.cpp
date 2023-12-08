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
        float glowSize,
        float strokeWidth
):
m_deviceResources(deviceResources),
m_begin(begin),
m_end(end),
m_glowSize(glowSize),
m_strokeWidth(strokeWidth)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void D2DLightLineRenderer::CreateDeviceDependentResources()
{
    m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_solidBrush);
    m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_redSolidBrush);

    ComPtr<IWICBitmapDecoder> wicBitmapDecoder;
    DX::ThrowIfFailed(
        m_deviceResources->GetWicImagingFactory()->CreateDecoderFromFilename(
            L"Assets\\storelogo-sdk.png",
            //L"Assets\\girl.png",
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &wicBitmapDecoder
        )
    );

    ComPtr<IWICBitmapFrameDecode> wicBitmapFrame;
    DX::ThrowIfFailed(
        wicBitmapDecoder->GetFrame(0, &wicBitmapFrame)
    );

    ComPtr<IWICFormatConverter> wicFormatConverter;
    DX::ThrowIfFailed(
        m_deviceResources->GetWicImagingFactory()->CreateFormatConverter(&wicFormatConverter)
    );

    DX::ThrowIfFailed(
        wicFormatConverter->Initialize(
            wicBitmapFrame.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom  // the BGRA format has no palette so this value is ignored
        )
    );

    double dpiX = 96.0;
    double dpiY = 96.0;
    DX::ThrowIfFailed(
        wicFormatConverter->GetResolution(&dpiX, &dpiY)
    );

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DDeviceContext()->CreateBitmapFromWicBitmap(
            wicFormatConverter.Get(),
            BitmapProperties(
                PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                static_cast<float>(dpiX),
                static_cast<float>(dpiY)
            ),
            &m_sourceBitmap
        )
    );

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
    );

    //D2D1_BITMAP_PROPERTIES sourceBitmapProperties =
    //    D2D1::BitmapProperties(
    //        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
    //        m_deviceResources->GetDPI(),
    //        m_deviceResources->GetDPI()
    //    );


    //DX::ThrowIfFailed(
    //    m_deviceResources->GetD2DDeviceContext()->CreateBitmap(
    //        D2D1_SIZE_U{ desc.BufferDesc.Width , desc.BufferDesc.Height },
    //        sourceBitmapProperties,
    //        &m_sourceBitmap
    //    )
    //);
    CreateTargetBitmap();
    CreateGeometry();
    CreateEffect();
}

void D2DLightLineRenderer::CreateWindowSizeDependentResources()
{

}

void D2DLightLineRenderer::ReleaseDeviceDependentResources(){
    m_solidBrush.Reset();
    m_glowEffect.Reset();
    m_sourceBitmap.Reset();
    m_targetBitmap.Reset();
    m_targetAssociateBitmap.Reset();
    m_stateBlock.Reset();
    m_pathGeometry.Reset();
}

void D2DLightLineRenderer::Render(){
    //m_deviceResources->GetD2DDeviceContext()->SetTarget(m_sourceBitmap.Get());

    //m_deviceResources->GetD2DDeviceContext()->SetTarget(m_deviceResources->GetD2DTargetBitmap());

    //ComPtr<ID2D1ColorContext> colorContext;
    //m_deviceResources->GetD2DTargetBitmap()->GetColorContext(&colorContext);
    //int size = colorContext->GetProfileSize();
    //std::vector<BYTE> color(size);
    //colorContext->GetProfile(color.data(), size);

    //m_deviceResources->GetD2DDeviceContext()->SaveDrawingState(m_stateBlock.Get());
    auto icoSize = m_sourceBitmap->GetSize();
    auto size = m_targetAssociateBitmap->GetSize();

    m_deviceResources->GetD2DDeviceContext()->BeginDraw();
    //m_deviceResources->GetD2DDeviceContext()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
    m_deviceResources->GetD2DDeviceContext()->SetTransform(m_deviceResources->GetOrientationTransform2D());
 
    //m_deviceResources->GetD2DDeviceContext()->DrawImage(m_glowEffect.Get(),
    //    {0,0},
    //    D2D1::RectF(8, 8, size.width + 8, size.height + 8)
    //);

    //m_deviceResources->GetD2DDeviceContext()->DrawBitmap(
    //    m_targetAssociateBitmap.Get(),
    //    D2D1::RectF(0, 0, size.width, size.height)
    //);

    m_deviceResources->GetD2DDeviceContext()->DrawImage(m_glowEffect.Get(),
     { 0,0 },
     D2D1::RectF(0, 0, size.width , size.height)
    );

    m_deviceResources->GetD2DDeviceContext()->DrawBitmap(
        m_sourceBitmap.Get(),
        D2D1::RectF(8, 8, icoSize.width + 8, icoSize.height + 8)
    );


    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    m_deviceResources->GetD2DDeviceContext()->SetTransform(
                D2D1::Matrix3x2F::Translation(logicalSize.Width / 2.0f, logicalSize.Height / 2.0f) *
        m_deviceResources->GetOrientationTransform2D()
    );

    m_deviceResources->GetD2DDeviceContext()->DrawLine(
     { m_begin.first,  m_begin.second },
     { m_end.first,  m_end.second },
     *m_solidBrush.GetAddressOf());

    m_deviceResources->GetD2DDeviceContext()->FillRectangle(
    { 5, 5, 55, 55 },
    *m_redSolidBrush.GetAddressOf()
    );

    m_deviceResources->GetD2DDeviceContext()->EndDraw();

    //m_deviceResources->GetD2DDeviceContext()->RestoreDrawingState(m_stateBlock.Get());
}

void D2DLightLineRenderer::CreateTargetBitmap()
{
    DXGI_SWAP_CHAIN_DESC desc;
    DX::ThrowIfFailed(
        m_deviceResources->GetSwapChain()->GetDesc(&desc)
    );
    m_deviceResources->GetD2DDeviceContext()->CreateCompatibleRenderTarget(D2D1::SizeF(desc.BufferDesc.Width, desc.BufferDesc.Height),
        &m_targetBitmap);

    //渲染到目标贴图
    {
        m_targetBitmap->BeginDraw();

        m_targetBitmap->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

        Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

        // Translate to the center of the window.
        m_targetBitmap->SetTransform(
                        D2D1::Matrix3x2F::Translation(logicalSize.Width / 2.0f, logicalSize.Height / 2.0f) *
                m_deviceResources->GetOrientationTransform2D()
        );

        m_targetBitmap->DrawLine(
                { m_begin.first,  m_begin.second },
                { m_end.first,  m_end.second },
                *m_redSolidBrush.GetAddressOf()
                );

        //FLOAT left;
        //FLOAT top;
        //FLOAT right;
        //FLOAT bottom;

        m_targetBitmap->FillRectangle(
        {5, 5, 55, 55},
        *m_redSolidBrush.GetAddressOf()
       );



            //,3.0);

 /*       m_targetBitmap->DrawLine(
            { m_begin.first,  m_begin.second },
            { m_end.first,  m_end.second },
            *m_solidBrush.GetAddressOf());*/

        HRESULT hr = m_targetBitmap->EndDraw();
        if (hr != D2DERR_RECREATE_TARGET)
        {
            DX::ThrowIfFailed(hr);
        }
    }

    DX::ThrowIfFailed(m_targetBitmap->GetBitmap(&m_targetAssociateBitmap));
}

void D2DLightLineRenderer::CreateGeometry()
{
    DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreatePathGeometry(&m_pathGeometry));
    Microsoft::WRL::ComPtr <ID2D1GeometrySink> pSink;
    DX::ThrowIfFailed(m_pathGeometry->Open(&pSink));
    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
    pSink->BeginFigure({ m_begin.first,  m_begin.second }, D2D1_FIGURE_BEGIN_FILLED);
     pSink->AddLine({ m_end.first,  m_end.second });
    //pSink->BeginFigure({ m_begin.first,  m_begin.second + 100 }, D2D1_FIGURE_BEGIN_FILLED);
    //pSink->AddLine({ m_end.first,  m_end.second + 100 });
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
    pSink->Close();
    pSink.Reset();
}

void D2DLightLineRenderer::CreateEffect()
{
    //边缘泛光效果
  /*  {
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
    }*/
//实验
    {
        m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1GaussianBlur, &m_glowEffect);

        m_glowEffect->SetInput(0, m_targetAssociateBitmap.Get());
        m_glowEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 3.0f);
    }

    {
        //m_deviceResources->GetD2DDeviceContext()->CreateEffect(CLSID_D2D1ColorMatrix, &m_glowEffect);
        //m_glowEffect->SetInput(0, m_targetAssociateBitmap.Get());
        //D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        //m_glowEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);

    }


}



























