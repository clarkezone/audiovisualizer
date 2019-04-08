#pragma once

#include "TiledViewManager.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct TiledViewManager : TiledViewManagerT<TiledViewManager>
    {
	private:
		float _viewportSize;
		float _viewportOffset;
		double _zoom;
		double _resolution;
		double _conversionFactor;
		double _contentOffset;
		double _contentSize;
		uint32_t _offscreenTiles;
		TileIndexRange _tileRange;
		
		IContentScaleManager _scaleManager;
		float _tileSizeLimit;

		winrt::event<Windows::Foundation::EventHandler<IInspectable>> _scaleChanged;
		winrt::event<Windows::Foundation::EventHandler<IInspectable>> _rangeChanged;

		void updateTiles();
		void updateScale();

		void TileRange(AudioVisualizer::TileIndexRange const& tileRange);
		void ContentScale(ContentScaleRange const &range);
	public:
        TiledViewManager();

        float ViewportSize();
        void ViewportSize(float value);
        float ViewportOffset();
        void ViewportOffset(float value);
        double Zoom();
        void Zoom(double value);
        double Resolution();
        void Resolution(double value);
        double ContentOffset();
        void ContentOffset(double value);
        double ContentSize();
        void ContentSize(double value);
		float TileSizeLimit();
		void TileSizeLimit(float value);
		AudioVisualizer::TileIndexRange TileRange();
		AudioVisualizer::ContentScaleRange ScaleRange();
		double Scale();

        float ContentToView(double value);
        double ViewToContent(float value);
    
		winrt::event_token RangeChanged(Windows::Foundation::EventHandler<IInspectable> const& handler);
		void RangeChanged(winrt::event_token const& token) noexcept;

		winrt::event_token ScaleChanged(Windows::Foundation::EventHandler<IInspectable> const& handler);
		void ScaleChanged(winrt::event_token const& token) noexcept;

	};
}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct TiledViewManager : TiledViewManagerT<TiledViewManager, implementation::TiledViewManager>
	{
	};
}
