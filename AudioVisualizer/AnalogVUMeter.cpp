#include "pch.h"
#include "AnalogVUMeter.h"
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Windows.Foundation.Numerics.h>

namespace winrt::AudioVisualizer::implementation
{
	const float pi = 3.14159f;




	AnalogVUMeter::AnalogVUMeter()
	{
		using namespace Windows::UI;
		using namespace Windows::UI::Xaml;
		using namespace Windows::UI::Xaml::Hosting;
		using namespace Windows::Foundation::Numerics;

		
		_scaleSections =
		{
			MeterScaleSection() = { 0.0f,0.66f,Windows::UI::Colors::Black(),3.0f },
			MeterScaleSection() = { 0.67f,0.33f,Windows::UI::Colors::Red(),6.0f }
		};

		_scaleLabels =
		{
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3.0f, L"-20", 0.067f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-10", 0.21f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-7", 0.3f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(),  3, L"-5", 0.37f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-3", 0.47f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-1", 0.59f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Black(),  3, L"0", 0.66f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Red(),  3, L"+1", 0.75f },
			MeterScaleLabel() = { 0.05f, Windows::UI::Colors::Red(), 3, L"+3", 0.94f }

		};

		auto elementVisual = ElementCompositionPreview::GetElementVisual(*this);
		_compositor = elementVisual.Compositor();

		auto device = Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();
		_compositionDevice = Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionGraphicsDevice(_compositor, device);

		_meterVisual = _compositor.CreateContainerVisual();
		ElementCompositionPreview::SetElementChildVisual(*this, _meterVisual);

		_meterBackgroundVisual = _compositor.CreateSpriteVisual();
		_meterBackgroundVisual.Brush(util::make_composition_brush(Background(),_compositor));
		_meterVisual.Children().InsertAtBottom(_meterBackgroundVisual);

		_dialBrush = _compositor.CreateColorBrush(Windows::UI::Colors::Red());
		_scaleVisual = _compositor.CreateSpriteVisual();
		_meterVisual.Children().InsertAtTop(_scaleVisual);
		_dialVisual = _compositor.CreateSpriteVisual();
		_dialVisual.Brush(_dialBrush);
		_dialVisual.Size(float2(5.0f, 0.0f));	// Default width 5

		_meterClip = _compositor.CreateInsetClip();
		_meterVisual.Clip(_meterClip);

		_dialShadow = _compositor.CreateDropShadow();
		_dialShadow.Offset(Windows::Foundation::Numerics::float3(-3, 3, -10));
		_dialShadow.BlurRadius(5.0f);
		_dialShadow.Color(Windows::UI::Colors::Black());
		_dialShadow.Opacity(0.5f);
		_dialVisual.Shadow(_dialShadow);

		_meterVisual.Children().InsertAtTop(_dialVisual);

		SizeChanged(SizeChangedEventHandler(this, &AnalogVUMeter::OnSizeChanged));
		RegisterPropertyChangedCallback(Windows::UI::Xaml::Controls::Control::BackgroundProperty(), Windows::UI::Xaml::DependencyPropertyChangedCallback(this, &AnalogVUMeter::OnBackgroundChanged));
		Loaded(Windows::UI::Xaml::RoutedEventHandler(this, &AnalogVUMeter::OnLoaded));
	}

	AudioVisualizer::IVisualizationSource AnalogVUMeter::Source()
	{
		return _source;
	}

	void AnalogVUMeter::Source(AudioVisualizer::IVisualizationSource const & value)
	{
		_source = value;
	}

	int32_t AnalogVUMeter::ChannelIndex()
	{
		return _channelIndex;
	}

	void AnalogVUMeter::ChannelIndex(int32_t value)
	{
		if (value < 0) {
			throw hresult_invalid_argument(L"Channel index cannot be negative");
		}

		_channelIndex = value;
	}

	Windows::Foundation::Numerics::float2 AnalogVUMeter::DialRelativeFixPoint()
	{
		return _dialRelativeFixPoint;
	}

	void AnalogVUMeter::DialRelativeFixPoint(Windows::Foundation::Numerics::float2 value)
	{
		_dialRelativeFixPoint = value;
		float2 size = float2((float)ActualWidth(), (float)ActualHeight());
		UpdateLayout(size);

	}

	Windows::Foundation::Numerics::float2 AnalogVUMeter::ScaleRelativeStartingPoint()
	{
		return _scaleStartingPoint;
	}

	void AnalogVUMeter::ScaleRelativeStartingPoint(Windows::Foundation::Numerics::float2 value)
	{
		_scaleStartingPoint = value;
		float2 size = float2((float)ActualWidth(), (float)ActualHeight());
		UpdateLayout(size);
	}

	com_array<AudioVisualizer::MeterScaleSection> AnalogVUMeter::ScaleSections()
	{
		return com_array<AudioVisualizer::MeterScaleSection>(_scaleSections);
	}

	void AnalogVUMeter::ScaleSections(array_view<AudioVisualizer::MeterScaleSection const> value)
	{
		_scaleSections.resize(value.size());
		auto copy = _scaleSections.begin();
		for (auto item : value) {
			*copy = item;
			copy++;
		}
		float2 size = float2((float)ActualWidth(), (float)ActualHeight());
		PaintScale(size);
	}

	com_array<AudioVisualizer::MeterScaleLabel> AnalogVUMeter::ScaleLabels()
	{
		return com_array<AudioVisualizer::MeterScaleLabel>(_scaleLabels);
	}

	void AnalogVUMeter::ScaleLabels(array_view<AudioVisualizer::MeterScaleLabel const> value)
	{
		_scaleLabels.resize(value.size());
		auto copy = _scaleLabels.begin();
		for (auto item : value) {
			*copy = item;
			copy++;
		}
		//std::copy(std::begin(value), std::end(value), _scaleLabels);
		float2 size = float2((float)ActualWidth(), (float)ActualHeight());
		PaintScale(size);
	}

	float AnalogVUMeter::DialShadowOpacity()
	{
		return _dialShadow.Opacity();
	}

	void AnalogVUMeter::DialShadowOpacity(float value)
	{
		_dialShadow.Opacity(value);
	}

	float AnalogVUMeter::DialShadowBlurRadius()
	{
		return _dialShadow.BlurRadius();
	}

	void AnalogVUMeter::DialShadowBlurRadius(float value)
	{
		_dialShadow.BlurRadius(value);
	}

	Windows::Foundation::Numerics::float3 AnalogVUMeter::DialShadowOffset()
	{
		return _dialShadow.Offset();
	}

	void AnalogVUMeter::DialShadowOffset(Windows::Foundation::Numerics::float3 const& value)
	{
		_dialShadow.Offset(value);
	}

	Windows::UI::Color AnalogVUMeter::DialShadowColor()
	{
		return _dialShadow.Color();
	}

	void AnalogVUMeter::DialShadowColor(Windows::UI::Color const& value)
	{
		_dialShadow.Color(value);
	}

	float AnalogVUMeter::DialRelativeLength()
	{
		return _dialRelativeLength;
	}

	void AnalogVUMeter::DialRelativeLength(float value)
	{
		_dialRelativeLength = value;
		_dialVisual.Size(float2(_dialVisual.Size().x, _scaleArcRadius * _dialRelativeLength));
	}

	Windows::UI::Color AnalogVUMeter::DialColor()
	{
		return _dialBrush.Color();
	}

	void AnalogVUMeter::DialColor(Windows::UI::Color const& value)
	{
		_dialBrush.Color(value);
	}

	float AnalogVUMeter::DialWidth()
	{
		return _dialVisual.Size().x;
	}

	void AnalogVUMeter::DialWidth(float value)
	{
		_dialVisual.Size(float2(value, _scaleArcRadius * _dialRelativeLength));
	}


	void AnalogVUMeter::UpdateDialPosition()
	{
		float clampedValue = _meterValue >= 0.0f && _meterValue <= 1.0f ? _meterValue : _meterValue <= 0.0f ? 0.0f : 1.0f;
		float angle = _scaleSweepAngle * clampedValue + _scaleArcStartAngle - pi / 2;
		auto rotation = make_float4x4_rotation_z(angle);
		rotation.m41 = _scaleArcCenterPoint.x;
		rotation.m42 = _scaleArcCenterPoint.y;
		_dialVisual.TransformMatrix(rotation);
	}

	float2 AnalogVUMeter::PointOnArc(float relativeValue, float relativeLength)
	{
		float angle = _scaleSweepAngle * relativeValue + _scaleArcStartAngle;
		return _scaleArcCenterPoint + relativeLength * _scaleArcRadius * float2(cosf(angle), sinf(angle));
	}

	void AnalogVUMeter::PaintScale(const float2 &size)
	{
		if (size.x == 0 || size.y == 0)
			return;
		using namespace Windows::Graphics::DirectX;
		using namespace Microsoft::Graphics::Canvas::Text;
		using namespace Microsoft::Graphics::Canvas::Geometry;
		using namespace Microsoft::Graphics::Canvas::UI::Composition;

		auto surface = _compositionDevice.CreateDrawingSurface(size, DirectXPixelFormat::R8G8B8A8UIntNormalized, DirectXAlphaMode::Premultiplied);

		CanvasTextFormat textFormat = CanvasTextFormat();
		textFormat.VerticalAlignment(CanvasVerticalAlignment::Center);
		textFormat.HorizontalAlignment(CanvasHorizontalAlignment::Center);

		// Draw scale
		auto drawingSession = CanvasComposition::CreateDrawingSession(surface);
		{
			drawingSession.Clear(Windows::UI::Colors::Transparent());
			for (auto section : _scaleSections)
			{
				auto pathBuilder = CanvasPathBuilder(drawingSession);
				float segmentStartAngle = _scaleSweepAngle * section.Start + _scaleArcStartAngle;
				float2 segmantArcStartPoint = PointOnArc(section.Start);
				pathBuilder.BeginFigure(segmantArcStartPoint);
				pathBuilder.AddArc(_scaleArcCenterPoint, _scaleArcRadius, _scaleArcRadius, segmentStartAngle, _scaleSweepAngle * section.Length);
				pathBuilder.EndFigure(CanvasFigureLoop::Open);
				auto path = CanvasGeometry::CreatePath(pathBuilder);
				drawingSession.DrawGeometry(path, section.LineColor, section.LineThickness);
			}

			for (auto label : _scaleLabels)
			{
				float2 tickStart = PointOnArc(label.Position);
				float2 tickEnd = PointOnArc(label.Position, label.TickLength + 1.0f);
				float2 labelCenterPoint = PointOnArc(label.Position, label.TickLength + 1.0f + 0.05f);
				drawingSession.DrawLine(tickStart, tickEnd, label.TickColor, label.TickThickness);
				auto oldTransform = drawingSession.Transform();
				float labelRotationAngle = _scaleSweepAngle * label.Position + _scaleArcStartAngle + pi / 2.0f;
				drawingSession.Transform(make_float3x2_rotation(labelRotationAngle, labelCenterPoint));
				drawingSession.DrawText(label.Label, labelCenterPoint, label.TickColor, textFormat);
				drawingSession.Transform(oldTransform);
			}
		}
		drawingSession.Close();
		auto scaleBrush = _compositor.CreateSurfaceBrush(surface);
		_scaleVisual.Brush(scaleBrush);
	}

	void AnalogVUMeter::OnSizeChanged(IInspectable sender, Windows::UI::Xaml::SizeChangedEventArgs const & args)
	{
		using namespace Windows::Foundation::Numerics;
		float2 newSize = float2((float)args.NewSize().Width, (float)args.NewSize().Height);
		_meterVisual.Size(newSize);
		_meterBackgroundVisual.Size(newSize);
		_scaleVisual.Size(newSize);
		UpdateLayout(newSize);
	}
	void AnalogVUMeter::UpdateLayout(winrt::Windows::Foundation::Numerics::float2 &size)
	{
		// Recalculate internal metrics
		_scaleArcCenterPoint = size * _dialRelativeFixPoint;
		_scaleArcStartPoint = size * _scaleStartingPoint;
		_scaleArcRadius = distance(_scaleArcCenterPoint, _scaleArcStartPoint);
		_scaleArcStartAngle = atan2f(_scaleArcStartPoint.y - _scaleArcCenterPoint.y, _scaleArcStartPoint.x - _scaleArcCenterPoint.x);

		// Calculate angle sweep for symmetric meter
		float2 scaleSymmetricEndPoint = float2(size.x - _scaleArcStartPoint.x, _scaleArcStartPoint.y);
		float  endPointAngle = atan2f(scaleSymmetricEndPoint.y - _scaleArcCenterPoint.y, scaleSymmetricEndPoint.x - _scaleArcCenterPoint.x);
		_scaleSweepAngle = endPointAngle - _scaleArcStartAngle;
		_dialVisual.Size(float2(_dialVisual.Size().x, _scaleArcRadius * _dialRelativeLength));

		UpdateDialPosition();
		PaintScale(size);
	}

	void AnalogVUMeter::OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const &)
	{
		Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(Windows::System::Threading::TimerElapsedHandler(this, &AnalogVUMeter::UpdateMeter), Windows::Foundation::TimeSpan(166667));	// Fire 60 times per second
	}
	void AnalogVUMeter::OnBackgroundChanged(Windows::UI::Xaml::DependencyObject const &, Windows::UI::Xaml::DependencyProperty const &)
	{
		_meterBackgroundVisual.Brush(util::make_composition_brush(Background(),_compositor));
	}
	void AnalogVUMeter::UpdateMeter(Windows::System::Threading::ThreadPoolTimer const &)
	{
		_meterValue = 0.0f;
		if (_source) {
			auto data = _source.GetData();
			if (data && data.RMS() && data.RMS().Size() > _channelIndex) {
				_meterValue = data.RMS().AmplitudeScale() == ScaleType::Linear ? data.RMS().GetAt(_channelIndex) : powf(10, data.RMS().GetAt(_channelIndex) / 20.0f);
			}
		}
		UpdateDialPosition();
	}
}
