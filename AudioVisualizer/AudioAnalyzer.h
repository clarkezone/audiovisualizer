﻿#pragma once

#include "AudioAnalyzer.g.h"
#include "ring_buffer.h"
#include "ScalarData.h"
#include "SpectrumData.h"


namespace winrt::AudioVisualizer::implementation
{
    struct AudioAnalyzer : AudioAnalyzerT<AudioAnalyzer>
    {
	private:
		bool _bIsClosed;
		bool _asyncProcessing;
		bool _bIsSuspended;
		bool _bIsFlushPending;

		AnalyzerType _analyzerTypes;
		uint32_t _inputChannels;
		uint32_t _sampleRate;
		uint32_t _fftLength;
		uint32_t _fftLengthLog2;

		uint32_t _stepFrames;
		uint32_t _overlapFrames;
		float _fOutSampleRate;
		
		Windows::Foundation::IReference<int64_t> _seedPosition;

		DirectX::XMVECTOR *_pWindow;
		DirectX::XMVECTOR *_pFftReal;
		DirectX::XMVECTOR *_pFftUnityTable;
		DirectX::XMVECTOR *_pFftBuffers;
		float _fFftScale;	// 2/N scale factor for fft output
		::AudioVisualizer::ring_buffer _inputBuffer;
		std::mutex _inputBufferAccess;
		HANDLE _evtProcessingThreadWait;
		Windows::Foundation::IAsyncAction _workThread{ nullptr };
		
		void InitWindow();
		void FreeBuffers();

		winrt::event<Windows::Foundation::TypedEventHandler<AudioVisualizer::AudioAnalyzer, AudioVisualizer::VisualizationDataFrame>> _output;

		inline int64_t time_to_frames(Windows::Foundation::TimeSpan const &time)
		{
			return (int64_t) (time.count() * _sampleRate + 5000000L) / 10000000L;
		}
		inline Windows::Foundation::TimeSpan frames_to_time(int64_t frames)
		{
			return Windows::Foundation::TimeSpan(10000000L * frames / _sampleRate); 
		}
		void AddInput(const winrt::Windows::Media::AudioFrame & frame);
		void ProcessingProc(Windows::Foundation::IAsyncAction const &action);
		void AnalyzeData();
		void Calculate(DirectX::XMVECTOR *pRms, DirectX::XMVECTOR *pPeak, DirectX::XMVECTOR *pSpectrum);
	public:

		AudioAnalyzer() = delete;
		AudioAnalyzer(std::nullptr_t = nullptr) noexcept {}
		~AudioAnalyzer() {
			Close();
		}
		AudioAnalyzer(uint32_t bufferSize, uint32_t inputChannels, uint32_t sampleRate, uint32_t inputStep, uint32_t inputOverlap, uint32_t fftLength,bool asyncProcessing);

		void ProcessInput(Windows::Media::AudioFrame const& frame);
		void ProcessInputRaw(float* pData, size_t sampleCount,int64_t position);

		AudioVisualizer::AnalyzerType AnalyzerTypes();
		void AnalyzerTypes(AudioVisualizer::AnalyzerType const& value);
		float SpectrumStep();
		uint32_t SpectrumElementCount();
		bool IsSuspended();
		void IsSuspended(bool value);
		void Flush();
		void Flush(int64_t seedPosition);
		event_token Output(Windows::Foundation::TypedEventHandler<AudioVisualizer::AudioAnalyzer, AudioVisualizer::VisualizationDataFrame> const& handler);
		void Output(event_token const& token);
		void Close();
	};
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct AudioAnalyzer : AudioAnalyzerT<AudioAnalyzer, implementation::AudioAnalyzer>
    {
    };
}
