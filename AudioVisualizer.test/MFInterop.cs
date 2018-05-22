using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [StructLayout(LayoutKind.Sequential)]
    struct MFT_INPUT_STREAM_INFO
    {
        public long hnsMaxLatency;
        public uint dwFlags;
        public uint cbSize;
        public uint cbMaxLookahead;
        public uint cbAlignment;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct MFT_OUTPUT_STREAM_INFO
    {
        public uint dwFlags;
        public uint cbSize;
        public uint cbAlignment;
    }
    [StructLayout(LayoutKind.Sequential)]
    internal struct MFT_OUTPUT_DATA_BUFFER
    {
        uint dwStreamID;
        IMFSample pSample;
        uint dwStatus;
        IMFCollection pEvents;
    }

    [ComImport()]
    [Guid("2cd2d921-c447-44a7-a13c-4adabfc247e3")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFAttributes
    {
        // Not using the internals
    }

    [ComImport()]
    [Guid("44ae0fa8-ea31-4109-8d2e-4cae4997c555")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFMediaType
    {
        // Not using the internals
    }

    [ComImport()]
    [Guid("DF598932-F10C-4E39-BBA2-C308F101DAA3")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFMediaEvent
    {
        // Not using the internals
    }

    [ComImport()]
    [Guid("c40a00f2-b93a-4d80-ae8c-5a1c634f58e4")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFSample
    {
        // Not using the internals
    }

    [ComImport()]
    [Guid("5BC8A76B-869A-46a3-9B03-FA218A66AEBE")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFCollection
    {
        // Not using the internals
    }



    [ComImport()]
    [Guid("bf94c121-5b05-4e6f-8000-ba598961414d")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IMFTransform
    {
        void GetStreamLimits(out uint pdwInputMinimum, out uint pdwInputMaximum, out uint pdwOutputMinimum, out uint pdwOutputMaximum);

        void GetStreamCount(out uint pcInputStreams, out uint pcOutputStreams);

        void GetStreamIDs(uint dwInputIDArraySize, out uint pdwInputIDs, uint dwOutputIDArraySize, out uint pdwOutputIDs);
        void GetInputStreamInfo(
            uint dwInputStreamID,
            out MFT_INPUT_STREAM_INFO pStreamInfo);

        void GetOutputStreamInfo(
            uint dwOutputStreamID,
            out MFT_OUTPUT_STREAM_INFO pStreamInfo);

        void GetAttributes([MarshalAs(UnmanagedType.Interface)] out IMFAttributes ppAttributes);

        void GetInputStreamAttributes(uint dwInputStreamID, [MarshalAs(UnmanagedType.Interface)] out IMFAttributes ppAttributes);

        void GetOutputStreamAttributes(uint dwOutputStreamID, [MarshalAs(UnmanagedType.Interface)] out IMFAttributes ppAttributes);

        void DeleteInputStream(uint dwStreamID);

        void AddInputStreams(uint cStreams, out uint adwStreamIDs);

        void GetInputAvailableType(uint dwInputStreamID, uint dwTypeIndex, [MarshalAs(UnmanagedType.Interface)] out IMFMediaType ppType);

        void GetOutputAvailableType(uint dwOutputStreamID, uint dwTypeIndex, [MarshalAs(UnmanagedType.Interface)] out IMFMediaType ppType);

        void SetInputType(uint dwInputStreamID, [In, MarshalAs(UnmanagedType.Interface)] IMFMediaType pType, uint dwFlags);

        void SetOutputType(uint dwOutputStreamID, [In, MarshalAs(UnmanagedType.Interface)] IMFMediaType pType, uint dwFlags);

        void GetInputCurrentType(uint dwInputStreamID, [MarshalAs(UnmanagedType.Interface)] out IMFMediaType ppType);

        void GetOutputCurrentType(uint dwOutputStreamID, [MarshalAs(UnmanagedType.Interface)] out IMFMediaType ppType);

        void GetInputStatus(uint dwInputStreamID, out uint pdwFlags);

        void GetOutputStatus(out uint pdwFlags);

        void SetOutputBounds(long hnsLowerBound, long hnsUpperBound);

        void ProcessEvent(uint dwInputStreamID, [In, MarshalAs(UnmanagedType.Interface)] IMFMediaEvent pEvent);

        void ProcessMessage(
            uint eMessage,
            UIntPtr ulParam);

        void ProcessInput(uint dwInputStreamID, [In, MarshalAs(UnmanagedType.Interface)] IMFSample pSample, uint dwFlags);

        void ProcessOutput(
            uint dwFlags,
            uint cOutputBufferCount,
            MFT_OUTPUT_DATA_BUFFER pOutputSamples,
            out uint pdwStatus);
    }

}
