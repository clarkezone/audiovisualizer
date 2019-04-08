#include "pch.h"
#include "TiledViewManager.h"
#include "BinaryScaleManager.h"
#include "BinaryScaleManager.h"

using namespace winrt::Windows::Foundation::Numerics;

namespace winrt::AudioVisualizer::implementation
{

	TiledViewManager::TiledViewManager() {
		_viewportSize = 0;
		_viewportOffset = 0;
		_zoom = 1.0;
		_resolution = 1.0;
		_conversionFactor = 1.0;
		_contentOffset = 0;
		_contentSize = 0;
		_tileSizeLimit = 256;
		_offscreenTiles = 0;
		_scaleManager = AudioVisualizer::BinaryScaleManager();

	}

    float TiledViewManager::ViewportSize()
    {
		return _viewportSize;
    }

    void TiledViewManager::ViewportSize(float value)
    {
		if (value < 0 ) {
			throw hresult_invalid_argument();
		}
		if (_viewportSize != value) {
			_viewportSize = value;
			updateTiles();
		}
    }

    float TiledViewManager::ViewportOffset()
    {
		return _viewportOffset;
    }

    void TiledViewManager::ViewportOffset(float value)
    {
		if (_viewportOffset != value) {
			_viewportOffset = value;
			updateTiles();
		}
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
		if (_zoom != value) {
			_zoom = value;
			_conversionFactor = _zoom * _resolution;
		}
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
		if (_resolution != value) {
			_resolution = value;
			_conversionFactor = _zoom * _resolution;
		}
    }

    double TiledViewManager::ContentOffset()
    {
		return _contentOffset;
    }

    void TiledViewManager::ContentOffset(double value)
    {
		if (_contentOffset != value) {
			_contentOffset = value;
		}
    }

    double TiledViewManager::ContentSize()
    {
		return _contentSize;
    }

    void TiledViewManager::ContentSize(double value)
    {
		if (value < 0) {
			throw hresult_invalid_argument();
		}
		_contentSize = value;
    }

	float TiledViewManager::TileSizeLimit()
	{
		return _tileSizeLimit;
	}

	void TiledViewManager::TileSizeLimit(float value)
	{
		_tileSizeLimit = value;
		updateTiles();
	}

	AudioVisualizer::TileIndexRange TiledViewManager::TileRange()
	{
		return _tileRange;
	}
	AudioVisualizer::ContentScaleRange TiledViewManager::ScaleRange()
	{
		return AudioVisualizer::ContentScaleRange();
	}
	double TiledViewManager::Scale()
	{
		return 0.0;
	}
	void TiledViewManager::TileRange(AudioVisualizer::TileIndexRange const & tileRange)
	{
		if (tileRange != _tileRange) {
			_tileRange = tileRange;
			_rangeChanged(*this, *this);
		}
	}

	void TiledViewManager::ContentScale(ContentScaleRange const & range)
	{
	}

    float TiledViewManager::ContentToView(double value)
    {
		return float(value * _conversionFactor);
    }

    double TiledViewManager::ViewToContent(float value)
    {
		return double(value) / _conversionFactor;
    }

	winrt::event_token TiledViewManager::RangeChanged(Windows::Foundation::EventHandler<IInspectable> const & handler)
	{
		return _rangeChanged.add(handler);
	}

	void TiledViewManager::RangeChanged(winrt::event_token const & token) noexcept
	{
		_rangeChanged.remove(token);
	}

	winrt::event_token TiledViewManager::ScaleChanged(Windows::Foundation::EventHandler<IInspectable> const & handler)
	{
		return _scaleChanged.add(handler);
	}

	void TiledViewManager::ScaleChanged(winrt::event_token const & token) noexcept
	{
		_scaleChanged.remove(token);
	}

	void TiledViewManager::updateTiles()
	{
		auto maximumTiles = ceil(ContentToView(ContentSize()) / _tileSizeLimit);

		TileIndexRange newRange{
			(int32_t)std::clamp<float>(floor(ViewportOffset() / _tileSizeLimit) - (float)_offscreenTiles, 0, maximumTiles),
			(int32_t)std::clamp<float>(ceil((ViewportOffset() + ViewportSize()) / _tileSizeLimit) + (float)_offscreenTiles, 0, maximumTiles)
		};

		TileRange(newRange);
	}

	void TiledViewManager::updateScale()
	{

	}

}
