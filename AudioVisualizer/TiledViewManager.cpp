#include "pch.h"
#include "TiledViewManager.h"

namespace winrt::AudioVisualizer::implementation
{
	TiledViewManager::TiledViewManager() {
		_viewportSize = { 0,0 };
		_viewportOffset = { 0,0 };
		_zoom = 1.0;
		_resolution = 1.0;
		_conversionFactor = 1.0;
		_contentOffset = { 0,0 };
		_contentSize = { 0,0 };
	}

    Windows::Foundation::Numerics::float2 TiledViewManager::ViewportSize()
    {
		return _viewportSize;
    }

    void TiledViewManager::ViewportSize(Windows::Foundation::Numerics::float2 const& value)
    {
		if (value.x < 0 || value.y < 0) {
			throw hresult_invalid_argument();
		}
		_viewportSize = value;
    }

    Windows::Foundation::Numerics::float2 TiledViewManager::ViewportOffset()
    {
		return _viewportOffset;
    }

    void TiledViewManager::ViewportOffset(Windows::Foundation::Numerics::float2 const& value)
    {
		_viewportOffset = value;
    }

    double TiledViewManager::Zoom()
    {
		return _zoom;
    }

    void TiledViewManager::Zoom(double value)
    {
		if (value <= 0.0) {
			throw hresult_invalid_argument();
		}
		_zoom = value;
		_conversionFactor = _zoom * _resolution;
    }

    double TiledViewManager::Resolution()
    {
		return _resolution;
    }

    void TiledViewManager::Resolution(double value)
    {
		if (value <= 0.0) {
			throw hresult_invalid_argument();
		}
		_resolution = value;
		_conversionFactor = _zoom * _resolution;
    }

    AudioVisualizer::ContentUnit TiledViewManager::ContentOffset()
    {
		return _contentOffset;
    }

    void TiledViewManager::ContentOffset(AudioVisualizer::ContentUnit const& value)
    {
		_contentOffset = value;
    }

    AudioVisualizer::ContentUnit TiledViewManager::ContentSize()
    {
		return _contentSize;
    }

    void TiledViewManager::ContentSize(AudioVisualizer::ContentUnit const& value)
    {
		if (value.X < 0 || value.Y < 0) {
			throw hresult_invalid_argument();
		}
		_contentSize = value;
    }

    Windows::Foundation::Numerics::float2 TiledViewManager::ContentToView(AudioVisualizer::ContentUnit const& value)
    {
		Windows::Foundation::Numerics::float2 valueInViewUnits = { float(value.X * _conversionFactor), float(value.Y * _conversionFactor) };
		return valueInViewUnits;
    }

    AudioVisualizer::ContentUnit TiledViewManager::ViewToContent(Windows::Foundation::Numerics::float2 const& value)
    {
		ContentUnit valueInContentUnits = { double(value.x) / _conversionFactor, double(value.y) / _conversionFactor };
		return valueInContentUnits;
    }
}
