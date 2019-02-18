#pragma once

#include "TiledViewManager.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct TiledViewManager : TiledViewManagerT<TiledViewManager>
    {
	private:
		Windows::Foundation::Numerics::float2 _viewportSize;
		Windows::Foundation::Numerics::float2 _viewportOffset;
		double _zoom;
		double _resolution;
		double _conversionFactor;
		ContentUnit _contentOffset;
		ContentUnit _contentSize;
		
	public:
        TiledViewManager();

        Windows::Foundation::Numerics::float2 ViewportSize();
        void ViewportSize(Windows::Foundation::Numerics::float2 const& value);
        Windows::Foundation::Numerics::float2 ViewportOffset();
        void ViewportOffset(Windows::Foundation::Numerics::float2 const& value);
        double Zoom();
        void Zoom(double value);
        double Resolution();
        void Resolution(double value);
        AudioVisualizer::ContentUnit ContentOffset();
        void ContentOffset(AudioVisualizer::ContentUnit const& value);
        AudioVisualizer::ContentUnit ContentSize();
        void ContentSize(AudioVisualizer::ContentUnit const& value);
        Windows::Foundation::Numerics::float2 ContentToView(AudioVisualizer::ContentUnit const& value);
        AudioVisualizer::ContentUnit ViewToContent(Windows::Foundation::Numerics::float2 const& value);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct TiledViewManager : TiledViewManagerT<TiledViewManager, implementation::TiledViewManager>
	{
	};
}
