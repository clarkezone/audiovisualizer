#pragma once

#include "AnalogVUMeter.g.h"
#include <winrt/Windows.System.Threading.h>
#include <vector>

using namespace winrt::Windows::Foundation::Numerics;

namespace winrt::AudioVisualizer::implementation
{
    struct AnalogVUMeter : AnalogVUMeterT<AnalogVUMeter>
    {
	private:
		Windows::UI::Composition::Compositor _compositor{ nullptr };
		Windows::UI::Composition::CompositionGraphicsDevice _compositionDevice{ nullptr };

		Windows::UI::Composition::ContainerVisual _meterVisual{ nullptr };	// Container visual for all other visuals
		Windows::UI::Composition::SpriteVisual _meterBackgroundVisual{ nullptr }; // Backplate for the meter
		Windows::UI::Composition::SpriteVisual _scaleVisual{ nullptr }; // Visual that renders the scale on the meter
		Windows::UI::Composition::SpriteVisual _dialVisual{ nullptr }; // Visual that renders the dial
		Windows::UI::Composition::DropShadow _dialShadow{ nullptr }; // Shadow for the dial
		Windows::UI::Composition::InsetClip _meterClip{ nullptr };// Clip the outside of the meter

		Windows::UI::Composition::CompositionColorBrush _backgroundBrush{ nullptr };
		Windows::UI::Composition::CompositionColorBrush _dialBrush{ nullptr };
		Windows::UI::Color _dialShadowColor;

		std::vector<MeterScaleSection> _scaleSections;
		std::vector<MeterScaleLabel> _scaleLabels;

		float2 _scaleArcCenterPoint = float2::zero();
		float2 _scaleArcStartPoint = float2::zero();
		float _scaleArcRadius = 0.0f;
		float _scaleArcStartAngle = 0.0f;

		// Meter configuration
		float2 _dialRelativeFixPoint = float2(0.5f, 1.5f);
		float2 _scaleStartingPoint = float2(0.1f, 0.6f);
		float _scaleSweepAngle = 1.33f;
		float _dialRelativeLength = 1.08f;
		float _meterValue = 0.0f;

		IVisualizationSource _source;
		uint32_t _channelIndex = 0;

		void UpdateDialPosition();
		void PaintScale(const float2 &size);
		float2 PointOnArc(float relativeValue,float relativeLength = 1.0f);

		void OnSizeChanged(IInspectable sender, Windows::UI::Xaml::SizeChangedEventArgs const &args);
		void UpdateLayout(winrt::Windows::Foundation::Numerics::float2 &newSize);
		void OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const &args);
		void OnBackgroundChanged(Windows::UI::Xaml::DependencyObject const &sender, Windows::UI::Xaml::DependencyProperty const &dp);
		void UpdateMeter(Windows::System::Threading::ThreadPoolTimer const &timer);
	public:
        AnalogVUMeter();
		AudioVisualizer::IVisualizationSource Source();
		void Source(AudioVisualizer::IVisualizationSource const& value);
		int32_t ChannelIndex();
		void ChannelIndex(int32_t value);
		Windows::Foundation::Numerics::float2 DialRelativeFixPoint();
		void DialRelativeFixPoint(Windows::Foundation::Numerics::float2 value);
		Windows::Foundation::Numerics::float2 ScaleRelativeStartingPoint();
		void ScaleRelativeStartingPoint(Windows::Foundation::Numerics::float2 value);
		com_array<AudioVisualizer::MeterScaleSection> ScaleSections();
		void ScaleSections(array_view<AudioVisualizer::MeterScaleSection const> value);
		com_array<AudioVisualizer::MeterScaleLabel> ScaleLabels();
		void ScaleLabels(array_view<AudioVisualizer::MeterScaleLabel const> value);

    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct AnalogVUMeter : AnalogVUMeterT<AnalogVUMeter, implementation::AnalogVUMeter>
    {
    };
}
