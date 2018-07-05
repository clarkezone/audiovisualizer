#pragma once

#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.UI.Core.h>
#include <set>

namespace winrt::AudioVisualizer::implementation
{
	template<typename ControlType>
	struct BarVisualizerBase
	{
	private:
		ControlType *derived_this() { return static_cast<ControlType *>(this); }

	protected:
		unsigned _barCount;
		Windows::UI::Xaml::Controls::Orientation _orientation;
		std::vector<MeterBarLevel> _levels;
		UINT32 _channelIndex;
		Windows::UI::Xaml::Thickness _elementMargin;
		Windows::UI::Color _unlitElementColor;
		std::mutex _lock;

		AudioVisualizer::IVisualizationSource _source{ nullptr };
		Windows::UI::Composition::Compositor _compositor{ nullptr };
		Windows::UI::Composition::CompositionGraphicsDevice _compositionDevice{ nullptr };
		Windows::UI::Composition::ContainerVisual _meterVisual{ nullptr };	// Container visual for all other visuals
		Windows::UI::Composition::SpriteVisual _meterBackgroundVisual{ nullptr }; // Backplate for the meter
		Windows::UI::Composition::ContainerVisual _meterElementVisuals{ nullptr };	// Container for individual elements visuals
		Windows::UI::Composition::CompositionColorBrush _backgroundBrush{ nullptr };
		Windows::UI::Composition::CompositionColorBrush _unlitElementBrush{ nullptr };
		Windows::UI::Composition::CompositionColorBrush _auxElementBrush{ nullptr };

		Windows::System::Threading::ThreadPoolTimer _updateTimer{ nullptr };


		std::vector<Windows::UI::Composition::SpriteVisual> _elementVisuals;
		std::vector<int> _barStates;	// Keeps state of the bar elements lit
		std::vector<int> _barAuxStates;	// Keeps state of the auxiliary bar elements lit

		virtual void OnSourceChanged(hstring const &) {};
		winrt::event_token _sourceChangedEvent;

	public:
		BarVisualizerBase()
		{
			using namespace Windows::UI;
			using namespace Windows::UI::Xaml;
			using namespace Windows::UI::Xaml::Hosting;
			using namespace Windows::Foundation::Numerics;
			
			_barCount = 1;
			_channelIndex = 0;
			_orientation = Windows::UI::Xaml::Controls::Orientation::Vertical;
			_elementMargin = Windows::UI::Xaml::ThicknessHelper::FromUniformLength(0);
			_unlitElementColor = Windows::UI::Colors::Transparent();

			auto elementVisual = ElementCompositionPreview::GetElementVisual(*derived_this());
			_compositor = elementVisual.Compositor();

			auto device = Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();
			_compositionDevice = Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionGraphicsDevice(_compositor, device);

			_meterVisual = _compositor.CreateContainerVisual();
			ElementCompositionPreview::SetElementChildVisual(*derived_this(), _meterVisual);

			_backgroundBrush = _compositor.CreateColorBrush(Colors::LightYellow());
			_auxElementBrush = _compositor.CreateColorBrush(Colors::BlueViolet());
			_meterBackgroundVisual = _compositor.CreateSpriteVisual();
			_meterBackgroundVisual.Brush(_backgroundBrush);
			_meterVisual.Children().InsertAtBottom(_meterBackgroundVisual);
			_meterElementVisuals = _compositor.CreateContainerVisual();
			_meterVisual.Children().InsertAtTop(_meterElementVisuals);
			_unlitElementBrush = _compositor.CreateColorBrush(_unlitElementColor);

			InitializeDefaultLevels();
			derived_this()->SizeChanged(SizeChangedEventHandler(this, &BarVisualizerBase::OnSizeChanged));
			derived_this()->Loaded(Windows::UI::Xaml::RoutedEventHandler(this, &BarVisualizerBase::OnLoaded));
			derived_this()->Unloaded(Windows::UI::Xaml::RoutedEventHandler(this, &BarVisualizerBase::OnUnloaded));
		}

		void InitializeDefaultLevels()
		{
			std::vector<MeterBarLevel> defaultLevels;
			defaultLevels.resize(23);
			int level = -60;
			for (size_t i = 0; i < 23; i++, level += 3)
			{
				defaultLevels[i].Level = (float)level;
				if (level < -6)
					defaultLevels[i].Color = Windows::UI::Colors::Lime();
				else if (level <= 0)
					defaultLevels[i].Color = Windows::UI::Colors::Yellow();
				else
					defaultLevels[i].Color = Windows::UI::Colors::Red();
			}
			Levels(array_view<const MeterBarLevel>(defaultLevels));
		}

		void CreateElementVisuals()
		{
			//TODO: Move to ui thread
			_meterElementVisuals.Children().RemoveAll();
			_elementVisuals.clear();
			size_t elementRowCount = _levels.size();
			_elementVisuals.resize(_barCount * elementRowCount, nullptr);
			_barStates.resize(_barCount, -1);
			_barAuxStates.resize(_barCount, -1);

			for (size_t columnIndex = 0, elementIndex = 0; columnIndex < _barCount; columnIndex++)
			{
				for (size_t rowIndex = 0; rowIndex < elementRowCount; rowIndex++, elementIndex++)
				{
					auto elementVisual = _compositor.CreateSpriteVisual();
					auto dropShadow = _compositor.CreateDropShadow();
					dropShadow.BlurRadius(3);
					dropShadow.Color(_unlitElementColor);
					dropShadow.Offset(Windows::Foundation::Numerics::float3(1, 1, -5));
					elementVisual.Shadow(dropShadow);
					elementVisual.Brush(_unlitElementBrush);
					_meterElementVisuals.Children().InsertAtBottom(elementVisual);
					_elementVisuals[elementIndex] = elementVisual;
				}
			}
		}

		void OnSizeChanged(Windows::Foundation::IInspectable const &, Windows::UI::Xaml::SizeChangedEventArgs const &args)
		{
			LayoutVisuals(args.NewSize());
		}

		void OnLoaded(Windows::Foundation::IInspectable const &, Windows::UI::Xaml::RoutedEventArgs const &)
		{
			using namespace Windows::System::Threading;
			_updateTimer = ThreadPoolTimer::CreatePeriodicTimer(TimerElapsedHandler(this, &BarVisualizerBase::OnUpdateTimerElapsed), Windows::Foundation::TimeSpan(166667));
		}

		void OnUnloaded(Windows::Foundation::IInspectable const &, Windows::UI::Xaml::RoutedEventArgs const &)
		{
			if (_updateTimer) {
				_updateTimer.Cancel();
			}
		}

		void OnUpdateTimerElapsed(Windows::System::Threading::ThreadPoolTimer const &)
		{
			VisualizationDataFrame frame{ nullptr };
			if (_source) {
				frame = _source.GetData();
			}
			OnUpdateMeter(frame);
		}

		virtual void OnUpdateMeter(VisualizationDataFrame const &frame) = 0;

		int GetBarElementIndex(float value) {
			if (value > _levels.front().Level) {
				auto firstLevelGTE = std::find_if(
					std::begin(_levels), 
					std::end(_levels), 
					[=](const MeterBarLevel &level) { return level.Level >= value; }
				);
				if (firstLevelGTE != std::end(_levels)) {
					if (firstLevelGTE == std::begin(_levels))
						return -1;

					return (firstLevelGTE - std::begin(_levels)) - 1;
				}
				else {
					return _levels.size();
				}
			}
			return -1;
		}

		void UpdateBarValue(uint32_t barIndex, float mainValue, float auxValue)
		{
			using namespace Windows::UI::Composition;
			int mainValueIndex = GetBarElementIndex(mainValue);
			int auxValueIndex = GetBarElementIndex(auxValue);

			int updateFrom = std::min(mainValueIndex, _barStates[barIndex]);
			int updateTo = std::max(mainValueIndex, _barStates[barIndex]);

			for (int rowIndex = 0; rowIndex < (int)_levels.size(); rowIndex++)
			{
				if (rowIndex >= updateFrom && rowIndex <= updateTo && updateFrom != updateTo ||
					(auxValueIndex != _barAuxStates[barIndex] && (rowIndex == auxValueIndex || rowIndex == _barAuxStates[barIndex]))
					)
				{
					auto visual = _elementVisuals[barIndex * _levels.size() + rowIndex];
					auto shadow = visual.Shadow().as<DropShadow>();
					if (rowIndex <= mainValueIndex || rowIndex == auxValueIndex) {
						Windows::UI::Composition::CompositionColorBrush brush{ nullptr };
						if (rowIndex == auxValueIndex && _auxElementBrush)
						{
							brush = _auxElementBrush;
						}
						else
						{
							auto brushEntry = _elementBrushes.find(_levels[rowIndex].Color);
							assert(brushEntry != std::end(_elementBrushes));	// Should not happen
							brush = brushEntry->second;
						}
						visual.Brush(brush);
						shadow.Color(brush.Color());
					}
					else
					{
						visual.Brush(_unlitElementBrush);
						shadow.Color(_unlitElementColor);
					}
				}		
			}
			_barStates[barIndex] = mainValueIndex;
			_barAuxStates[barIndex] = auxValueIndex;
		}

		void LayoutVisuals()
		{
			LayoutVisuals(winrt::Windows::Foundation::Size((float)derived_this()->ActualWidth(), (float)derived_this()->ActualHeight()));
		}

		void LayoutVisuals(Windows::Foundation::Size size)
		{
			using namespace Windows::Foundation::Numerics;
			using namespace Windows::UI::Xaml::Controls;
			if (_elementVisuals.size() == 0)
				return;

			size_t elementRowCount = _levels.size();

			float2 newSize = float2(size.Width, size.Height);

			float2 cellSize = _orientation == Orientation::Vertical ? // Flip dimensions based on orientation
								float2(newSize.x / _barCount, newSize.y / _levels.size()) : 
								float2(newSize.x / _levels.size(), newSize.y / _barCount);

			float2 elementOffset = float2(float(_elementMargin.Left) * cellSize.x, float(_elementMargin.Top) * cellSize.y);
			float2 relativeElementSize = float2((float)(1.0 - _elementMargin.Left - _elementMargin.Right), (float)(1.0 - _elementMargin.Top - _elementMargin.Bottom));
			float2 elementSize = relativeElementSize * cellSize;

			for (size_t barIndex = 0, elementIndex = 0; barIndex < _barCount; barIndex++)
			{
				float2 elementCell = elementOffset + (_orientation == Orientation::Vertical ?
									float2(barIndex * cellSize.x, newSize.y - cellSize.y) : // If vertical layout, first cell is bottommost to the left
									float2(0, cellSize.y * barIndex));	// else left top

				for (size_t rowIndex = 0; rowIndex < elementRowCount; rowIndex++, elementIndex++)
				{
					auto elementVisual = _elementVisuals[elementIndex];
					elementVisual.Offset(float3(elementCell, 0));
					elementVisual.Size(elementSize);
					if (_orientation == Orientation::Vertical) {
						elementCell.y -= cellSize.y;
					}
					else {
						elementCell.x += cellSize.x;
					}
				}
			}
		}

		AudioVisualizer::IVisualizationSource Source()
		{
			return _source;
		}

		
		void Source(AudioVisualizer::IVisualizationSource const& value)
		{
			if (_source) {
				_source.ConfigurationChanged(_sourceChangedEvent);
			}
			_source = value;
			OnSourceChanged(L"");
			if (_source) {
				_sourceChangedEvent = _source.ConfigurationChanged(
					Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring>(
						[=] (const AudioVisualizer::IVisualizationSource &, const hstring &propertyName) {
							OnSourceChanged(propertyName);
						}
						));
			}
		}

		com_array<AudioVisualizer::MeterBarLevel> Levels()
		{
			return com_array<MeterBarLevel>(_levels);
		}

		struct windows_ui_color_is_less_than
		{
			bool operator()(Windows::UI::Color const &a, Windows::UI::Color const &b) const
			{
				return *((uint32_t*)&a) < *((uint32_t*)&b);
			}
		};
		std::map<Windows::UI::Color, Windows::UI::Composition::CompositionColorBrush, windows_ui_color_is_less_than> _elementBrushes;

		void Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
		{
			if (value.data() == nullptr || value.size() < 1)
				throw winrt::hresult_invalid_argument(L"Value cannot be empty");

			float lastValue = value.at(0).Level;
			for (size_t index = 1; index < value.size(); index++)
			{
				if (value.at(index).Level <= lastValue)
					throw winrt::hresult_invalid_argument(L"Array elements need to be in ascending order");
				lastValue = value.at(index).Level;
			}

			_levels.resize(value.size());
			std::copy(value.begin(), value.end(), _levels.begin());
			// Create element brushes cache, store in map indexing by color
			_elementBrushes.clear();
			for (auto level : _levels) {
				auto existingElement = _elementBrushes.find(level.Color);
				if (existingElement == std::end(_elementBrushes)) {
					auto brush = _compositor.CreateColorBrush(level.Color);
					_elementBrushes.insert(std::make_pair(level.Color, brush));
				}
			}
			// Create new layout on ui thread
			derived_this()->Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, Windows::UI::Core::DispatchedHandler(
				[=] {
				CreateElementVisuals();
				LayoutVisuals();
			}
			));
		}

		Windows::UI::Xaml::Controls::Orientation Orientation()
		{
			return _orientation;
		}

		void Orientation(Windows::UI::Xaml::Controls::Orientation const& value)
		{
			_orientation = value;
			LayoutVisuals();
		}

		int32_t ChannelIndex()
		{
			return _channelIndex;
		}

		void ChannelIndex(int32_t value)
		{
			if (value < 0) {
				throw hresult_invalid_argument(L"Channel index cannot be negative");
			}
			_channelIndex = value;
		}

		Windows::UI::Xaml::Thickness RelativeElementMargin()
		{
			return _elementMargin;
		}

		void RelativeElementMargin(Windows::UI::Xaml::Thickness const& value)
		{
			if (value.Left + value.Right >= 1.0 || value.Top + value.Bottom >= 1.0)
				throw hresult_invalid_argument();

			_elementMargin = value;
			LayoutVisuals();
		}

		Windows::UI::Color UnlitElement()
		{
			return _unlitElementColor;
		}

		void UnlitElement(Windows::UI::Color const& value)
		{
			_unlitElementColor = value;
			_unlitElementBrush = _compositor.CreateColorBrush(_unlitElementColor);
			LayoutVisuals();
		}	
	};
}
