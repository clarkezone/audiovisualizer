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

	struct ScaleSection {
		float Start;
		float Length;
		Windows::UI::Color LineColor;
		float LineThickness;
	};

	struct ScaleLabel
	{
		float TickLength;
		Windows::UI::Color TickColor;
		float TickThickness;
		hstring Label;
		float Position;
	};

	std::vector<ScaleSection> _scaleSections = 
	{
		ScaleSection() = { 0.0f,0.66f,Windows::UI::Colors::Black(),3.0f },
		ScaleSection() = { 0.67f,0.33f,Windows::UI::Colors::Red(),6.0f }
	};

	std::vector<ScaleLabel> _scaleLabels = 
	{
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3.0f, L"-20", 0.067f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-10", 0.21f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-7", 0.3f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(),  3, L"-5", 0.37f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-3", 0.47f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(), 3, L"-1", 0.59f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Black(),  3, L"0", 0.66f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Red(),  3, L"+1", 0.75f },
		ScaleLabel() = { 0.05f, Windows::UI::Colors::Red(), 3, L"+3", 0.94f }

	};

	AnalogVUMeter::AnalogVUMeter()
	{
		using namespace Windows::UI;
		using namespace Windows::UI::Xaml;
		using namespace Windows::UI::Xaml::Hosting;
		using namespace Windows::Foundation::Numerics;

		auto elementVisual = ElementCompositionPreview::GetElementVisual(*this);
		_compositor = elementVisual.Compositor();

		auto device = Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();
		_compositionDevice = Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionGraphicsDevice(_compositor, device);

		_meterVisual = _compositor.CreateContainerVisual();
		ElementCompositionPreview::SetElementChildVisual(*this, _meterVisual);

		_backgroundBrush = _compositor.CreateColorBrush(Colors::LightYellow());
		_meterBackgroundVisual = _compositor.CreateSpriteVisual();
		_meterBackgroundVisual.Brush(_backgroundBrush);
		_meterVisual.Children().InsertAtBottom(_meterBackgroundVisual);

		_dialBrush = _compositor.CreateColorBrush(Colors::Red());
		_scaleVisual = _compositor.CreateSpriteVisual();
		_meterVisual.Children().InsertAtTop(_scaleVisual);
		_dialVisual = _compositor.CreateSpriteVisual();
		_dialVisual.Brush(_dialBrush);

		_meterClip = _compositor.CreateInsetClip();
		_meterVisual.Clip(_meterClip);

		_dialShadowColor = Colors::DarkGray();
		_dialShadow = _compositor.CreateDropShadow();
		_dialShadow.Offset(float3(1, 1, -10));
		_dialShadow.BlurRadius(3.0f);
		_dialShadow.Color(_dialShadowColor);
		_dialVisual.Shadow(_dialShadow);

		_meterVisual.Children().InsertAtTop(_dialVisual);
		
		/* TODO: Create a light
		_meterLight = _compositor.CreateSpotLight();
		_meterLight.Targets.Add(_meterVisual);
		_meterLight.CoordinateSpace = _meterVisual;
		_meterLight.InnerConeAngleInDegrees = 30f;
		_meterLight.InnerConeColor = Colors.Blue;
		_meterLight.InnerConeIntensity = 10f;
		_meterLight.OuterConeAngleInDegrees = 60f;
		_meterLight.OuterConeColor = Colors.BlueViolet; */

		SizeChanged(SizeChangedEventHandler(this, &AnalogVUMeter::OnSizeChanged));
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

	uint32_t AnalogVUMeter::ChannelIndex()
	{
		return _channelIndex;
	}

	void AnalogVUMeter::ChannelIndex(uint32_t value)
	{
		_channelIndex = value;
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
		return _scaleArcCenterPoint + relativeLength * _scaleArcRadius * float2(cosf(angle),sinf(angle));
	}

	void AnalogVUMeter::PaintScale(const float2 &size)
	{
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
			//drawingSession.DrawCircle(_scaleArcCenterPoint, _scaleArcRadius,Colors.Red);
			drawingSession.Clear(Windows::UI::Colors::Transparent());
			for(auto section : _scaleSections)
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
			
			for(auto label : _scaleLabels)
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
		float2 newSize = float2((float) args.NewSize().Width, (float)args.NewSize().Height);
		_meterVisual.Size(newSize);
		_meterBackgroundVisual.Size(newSize);
		_scaleVisual.Size(newSize);
		// Recalculate internal metrics
		_scaleArcCenterPoint = newSize * _dialRelativeFixPoint;
		_scaleArcStartPoint = newSize * _scaleStartingPoint;
		_scaleArcRadius = distance(_scaleArcCenterPoint, _scaleArcStartPoint);
		_scaleArcStartAngle = atan2f(_scaleArcStartPoint.y - _scaleArcCenterPoint.y, _scaleArcStartPoint.x - _scaleArcCenterPoint.x);

		// Calculate angle sweep for symmetric meter
		float2 scaleSymmetricEndPoint = float2(newSize.x - _scaleArcStartPoint.x, _scaleArcStartPoint.y);
		float  endPointAngle = atan2f(scaleSymmetricEndPoint.y - _scaleArcCenterPoint.y, scaleSymmetricEndPoint.x - _scaleArcCenterPoint.x);
		_scaleSweepAngle = endPointAngle - _scaleArcStartAngle;
		
		_dialVisual.Size(float2(5, _scaleArcRadius * _dialRelativeLength));

		UpdateDialPosition();
		PaintScale(newSize);
	}
	void AnalogVUMeter::OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs const &)
	{
		Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(Windows::System::Threading::TimerElapsedHandler(this, &AnalogVUMeter::UpdateMeter), Windows::Foundation::TimeSpan(166667));	// Fire 60 times per second
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
