using AudioVisualizer.test.MediaFoundation.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Collections;
using Windows.Media;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    namespace MediaFoundation
    {
        namespace Interop
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
                public uint dwStreamID;
                public IMFSample pSample;
                public uint dwStatus;
                public IMFCollection pEvents;
            }
            [StructLayout(LayoutKind.Sequential)]
            internal struct MF_CLOCKPROPERTIES
            {
                Int64 qwCorrelationRate;
                Guid guidClockId;
                UInt32 dwClockFlags;
                Int64 qwClockFrequency;
                UInt32 dwClockTolerance;
                UInt32 dwClockJitter;
            }

            internal class MFPlat
            {
                [DllImport("MFPlat.dll")]
                internal extern static void MFCreatePropertiesFromMediaType([MarshalAs(UnmanagedType.Interface)] IMFMediaType mediaType, ref Guid uuid, [MarshalAs(UnmanagedType.IInspectable)] out Object properties);

                [DllImport("MFPlat.dll")]
                internal extern static void MFCreateMediaTypeFromProperties([MarshalAs(UnmanagedType.Interface)] Object properties, [MarshalAs(UnmanagedType.Interface)] out IMFMediaType mediaType);
                [DllImport("MFPlat.dll")]
                internal extern static void MFCreateSample([MarshalAs(UnmanagedType.Interface)] out IMFSample sample);
            }
            [ComImport()]
            [Guid("2cd2d921-c447-44a7-a13c-4adabfc247e3")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFAttributes
            {
                void GetItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, Out] ref object pValue);
                void GetItemType([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pType);
                void CompareItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref object Value, out int pbResult);
                void Compare([MarshalAs(UnmanagedType.Interface)] IMFAttributes pTheirs, uint MatchType, out int pbResult);
                void GetUINT32([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint punValue);
                void GetUINT64([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out ulong punValue);
                void GetDouble([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out double pfValue);
                void GetGUID([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out Guid pguidValue);
                void GetStringLength([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pcchLength);
                void GetString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out, MarshalAs(UnmanagedType.LPWStr)] string pwszValue, uint cchBufSize, [Out] out uint pcchLength);
                void GetAllocatedString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [MarshalAs(UnmanagedType.LPWStr)] out string ppwszValue, out uint pcchLength);
                void GetBlobSize([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pcbBlobSize);
                void GetBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pBuf, uint cbBufSize, [Out] out uint pcbBlobSize);
                void GetAllocatedBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out] IntPtr ppBuf, out uint pcbSize);
                void GetUnknown([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref Guid riid, out IntPtr ppv);
                void SetItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref object Value);
                void DeleteItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey);
                void DeleteAllItems();
                void SetUINT32([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, uint unValue);
                void SetUINT64([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ulong unValue);
                void SetDouble([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, double fValue);
                void SetGUID([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPStruct)] Guid guidValue);
                void SetString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPWStr)] string wszValue);
                void SetBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] pBuf, uint cbBufSize);
                void SetUnknown([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.IUnknown)] object pUnknown);
                void LockStore();
                void UnlockStore();
                void GetCount(out uint pcItems);
                void GetItemByIndex(uint unIndex, out Guid pguidKey, [In, Out] ref object pValue);
                void CopyAllItems([In, MarshalAs(UnmanagedType.Interface)] IMFAttributes pDest);
            }

            [ComImport()]
            [Guid("44ae0fa8-ea31-4109-8d2e-4cae4997c555")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFMediaType
            {
                void GetItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, Out] ref object pValue);
                void GetItemType([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pType);
                void CompareItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref object Value, out int pbResult);
                void Compare([MarshalAs(UnmanagedType.Interface)] IMFAttributes pTheirs, uint MatchType, out int pbResult);
                void GetUINT32([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint punValue);
                void GetUINT64([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out ulong punValue);
                void GetDouble([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out double pfValue);
                void GetGUID([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out Guid pguidValue);
                void GetStringLength([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pcchLength);
                void GetString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out, MarshalAs(UnmanagedType.LPWStr)] string pwszValue, uint cchBufSize, [Out] out uint pcchLength);
                void GetAllocatedString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [MarshalAs(UnmanagedType.LPWStr)] out string ppwszValue, out uint pcchLength);
                void GetBlobSize([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, out uint pcbBlobSize);
                void GetBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pBuf, uint cbBufSize, [Out] out uint pcbBlobSize);
                void GetAllocatedBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [Out] IntPtr ppBuf, out uint pcbSize);
                void GetUnknown([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref Guid riid, out IntPtr ppv);
                void SetItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ref object Value);
                void DeleteItem([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey);
                void DeleteAllItems();
                void SetUINT32([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, uint unValue);
                void SetUINT64([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, ulong unValue);
                void SetDouble([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, double fValue);
                void SetGUID([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPStruct)] Guid guidValue);
                void SetString([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPWStr)] string wszValue);
                void SetBlob([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] pBuf, uint cbBufSize);
                void SetUnknown([In, MarshalAs(UnmanagedType.LPStruct)] Guid guidKey, [In, MarshalAs(UnmanagedType.IUnknown)] object pUnknown);
                void LockStore();
                void UnlockStore();
                void GetCount(out uint pcItems);
                void GetItemByIndex(uint unIndex, out Guid pguidKey, [In, Out] ref object pValue);
                void CopyAllItems([In, MarshalAs(UnmanagedType.Interface)] IMFAttributes pDest);
                void GetMajorType(out Guid pguidMajorType);
                void IsCompressedFormat([MarshalAs(UnmanagedType.Bool)] out bool pfCompressed);
                void IsEqual([In, MarshalAs(UnmanagedType.Interface)] IMFMediaType pIMediaType, out uint pdwFlags);
                void GetRepresentation([In] Guid guidRepresentation, out IntPtr ppvRepresentation);
                void FreeRepresentation([In] Guid guidRepresentation, [In] IntPtr pvRepresentation);
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

            [ComImport()]
            [Guid("F6696E82-74F7-4f3d-A178-8A5E09C3659F")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFClockStateSink
            {
                void OnClockStart(Int64 systemTime, Int64 clockStartOffset);
                void OnClockStop(Int64 systemTime);
                void OnClockPause(Int64 systemTime);
                void OnClockRestart(Int64 systemTime);
                void OnClockSetRate(Int64 systemTime, float rate);
            }

            [ComImport()]
            [Guid("2eb1e945-18b8-4139-9b1a-d5d584818530")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFClock
            {
                void GetClockCharacteristics(out UInt32 characteristics);
                void GetCorrelatedTime(Int32 reserved, out Int64 clockTime, out Int64 systemTime);
                void GetContinuityKey(out UInt32 continuityKey);
                void GetClockState(out UInt32 clockState);
                void GetClockProperties(out MF_CLOCKPROPERTIES properties);
            }

            [ComImport()]
            [Guid("7FF12CCE-F76F-41c2-863B-1666C8E5E139")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFPresentationTimeSource
            {
                // Empty not used
            }

            [ComImport()]
            [Guid("868CE85C-8EA9-4f55-AB82-B009A910A805")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFPresentationClock
            {
                void SetTimeSource([In, MarshalAs(UnmanagedType.Interface)] IMFPresentationTimeSource timeSource);
                void GetTimeSource([Out, MarshalAs(UnmanagedType.Interface)] out IMFPresentationTimeSource timeSource);
                void GetTime([Out] out Int64 time);
                void AddClockStateSink([In, MarshalAs(UnmanagedType.Interface)] IMFClockStateSink sink);
                void RemoveClockStateSink([In, MarshalAs(UnmanagedType.Interface)] IMFClockStateSink sink);
                void Start(Int64 offset);
                void Stop();
                void Pause();
            }


            [ComImport()]
            [Guid("6ef2a662-47c0-4666-b13d-cbb717f2fa2c")]
            [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
            internal interface IMFClockConsumer
            {
                void SetPresentationClock([In, MarshalAs(UnmanagedType.IUnknown)] object clock);
                void GetPresentationClock([MarshalAs(UnmanagedType.IUnknown)] out object clock);
            }
        }
        
        [ClassInterface(ClassInterfaceType.None)]
        internal class FakePresentationClock : IMFPresentationClock, IMFClock
        {
            public TimeSpan Time = TimeSpan.Zero;
            private List<IMFClockStateSink> _sinks = new List<IMFClockStateSink>();

            public void AddClockStateSink([In, MarshalAs(UnmanagedType.Interface)] IMFClockStateSink sink)
            {
                _sinks.Add(sink);
            }

            public void RemoveClockStateSink([In, MarshalAs(UnmanagedType.Interface)] IMFClockStateSink sink)
            {
                _sinks.Remove(sink);
            }

            public void GetClockCharacteristics(out uint characteristics)
            {
                throw new NotImplementedException();
            }

            public void GetCorrelatedTime(int reserved, out long clockTime, out long systemTime)
            {
                throw new NotImplementedException();
            }

            public void GetContinuityKey(out uint continuityKey)
            {
                throw new NotImplementedException();
            }

            public void GetClockState(out uint clockState)
            {
                throw new NotImplementedException();
            }

            public void GetClockProperties([MarshalAs(UnmanagedType.LPStruct)] out MF_CLOCKPROPERTIES properties)
            {
                throw new NotImplementedException();
            }

            public void SetTimeSource([In, MarshalAs(UnmanagedType.Interface)] IMFPresentationTimeSource timeSource)
            {
                throw new NotImplementedException();
            }

            public void GetTimeSource([MarshalAs(UnmanagedType.Interface), Out] out IMFPresentationTimeSource timeSource)
            {
                throw new NotImplementedException();
            }

            public void GetTime([Out] out long time)
            {
                time = Time.Ticks;
            }

            public void Start(long offset)
            {
                Time = TimeSpan.FromTicks(offset);
                _sinks.ForEach((sink) => { sink.OnClockStart(0, Time.Ticks); });
            }

            public void Stop()
            {
                _sinks.ForEach((sink) => { sink.OnClockStop(0); });
            }

            public void Pause()
            {
                _sinks.ForEach((sink) => { sink.OnClockPause(0); });
            }
        }

        internal class MftWrapper
        {
            private IMFTransform _mft;
            private IMediaEncodingProperties PropertiesFromMediaType(IMFMediaType type)
            {
                Guid iidAudioEncodingProperties = new Guid("B4002AF6-ACD4-4E5A-A24B-5D7498A8B8C4");
                Object properties = null;
                MFPlat.MFCreatePropertiesFromMediaType(type, ref iidAudioEncodingProperties, out properties);
                return properties as IMediaEncodingProperties;
            }

            private IMFMediaType MediaTypeFromProperties(IMediaEncodingProperties properties)
            {
                IMFMediaType mediaType = null;
                MFPlat.MFCreateMediaTypeFromProperties(properties, out mediaType);
                return mediaType;
            }
            private PropertySet PropSetFromAttributes(IMFAttributes attributes)
            {
                if (attributes == null)
                    return null;
                var retValue = new PropertySet();
                uint cCount = 0;
                attributes.GetCount(out cCount);
                for (uint i = 0; i < cCount; i++)
                {
                    Guid key;
                    Object item = null;
                    attributes.GetItemByIndex(i, out key, ref item);
                    retValue.Add(key.ToString(), item);
                }
                return retValue;
            }

            public MftWrapper(Object transform)
            {
                _mft = (IMFTransform) transform;
            }

            public (uint inputStreamCount,uint outStreamCount) GetStreamCount()
            {
                (uint, uint) result = (0,0);
                _mft.GetStreamCount(out result.Item1, out result.Item2);
                return result;
            }
            public void GetStreamIds()
            {
                UInt32 cInput = 0, cOutput = 0;
                UInt32 outStreams = 0, inStreams = 0;
                _mft.GetStreamIDs(cInput, out inStreams, cOutput, out outStreams);
            }

            public (uint minInput,uint maxInput,uint minOutput,uint maxOutput) GetStreamLimits()
            {
                (uint, uint, uint, uint) result;
                _mft.GetStreamLimits(out result.Item1, out result.Item2, out result.Item3, out result.Item4);
                return result;
            }

            public MFT_INPUT_STREAM_INFO GetInputStreamInfo(uint streamIndex)
            {
                MFT_INPUT_STREAM_INFO streamInfo;
                _mft.GetInputStreamInfo(streamIndex, out streamInfo);
                return streamInfo;
            }
            public MFT_OUTPUT_STREAM_INFO GetOutputStreamInfo(uint streamIndex)
            {
                MFT_OUTPUT_STREAM_INFO streamInfo;
                _mft.GetOutputStreamInfo(streamIndex, out streamInfo);
                return streamInfo;
            }
            // No parameters as this is expected to throw
            public uint [] AddInputStreams(uint streamCount)
            {
                uint streamIds = 0;
                _mft.AddInputStreams(streamCount, out streamIds);
                return null;
            }

            public IMediaEncodingProperties GetAvailableInputTypes(uint streamID,uint typeIndex)
            {
                IMFMediaType mediaType = null;
                _mft.GetInputAvailableType(streamID, typeIndex,out mediaType);
                return PropertiesFromMediaType(mediaType);
            }

            internal void DeleteInputStream(uint streamId)
            {
                _mft.DeleteInputStream(streamId);
            }
            internal PropertySet GetAttributes()
            {
                IMFAttributes attributes = null;
                _mft.GetAttributes(out attributes);
                return PropSetFromAttributes(attributes);
            }

            internal PropertySet GetInputStreamAttributes()
            {
                IMFAttributes attributes = null;
                _mft.GetInputStreamAttributes(0, out attributes);
                return null;
            }
            internal PropertySet GetOutputStreamAttributes()
            {
                IMFAttributes attributes = null;
                _mft.GetOutputStreamAttributes(0, out attributes);
                return null;
            }

            internal void SetInputMediaType(uint streamIndex, AudioEncodingProperties encoding,bool bTestOnly)
            {
                IMFMediaType mediaType = MediaTypeFromProperties(encoding);
                _mft.SetInputType(streamIndex, mediaType, bTestOnly ? 1u : 0u);
            }
            internal void SetOutputMediaType(uint streamIndex, AudioEncodingProperties encoding, bool bTestOnly)
            {
                IMFMediaType mediaType = MediaTypeFromProperties(encoding);
                _mft.SetOutputType(streamIndex, mediaType, bTestOnly ? 1u : 0u);
            }

            internal void ProcessInput(IMFSample sample)
            {
                _mft.ProcessInput(0, sample, 0);
            }

            internal IMFSample ProcessOutput()
            {
                MFT_OUTPUT_DATA_BUFFER buffer = new MFT_OUTPUT_DATA_BUFFER() { dwStreamID = 0, pSample = null, dwStatus = 0, pEvents = null };
                uint status = 0;
                _mft.ProcessOutput(0, 1, buffer, out status);
                return buffer.pSample;
            }
        }
    }

}
