using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Media;

namespace VisualizerPlayer
{
    [ComImport]
    [Guid("5B0D3235-4DBA-4D44-865E-8F1D0E4FD04D")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal unsafe interface IMemoryBufferByteAccess
    {
        void GetBuffer(out byte* buffer, out uint capacity);
    }

    static internal class VisualizationExtensions
    {
        public static VisualizationData AsVisualizationData(this Windows.Media.AudioFrame frame)
        {
            return new VisualizationData(frame);
        }
    }

    internal sealed unsafe class VisualizationData : IDisposable
    {
        public void Dispose()
        {
            if (m_Buffer != null)
            {
                m_Buffer.Dispose();
                m_Buffer = null;
            }
            if (m_BufferReference != null)
            {
                m_BufferReference.Dispose();
                m_BufferReference = null;
            }
            GC.SuppressFinalize(this);
        }

        AudioBuffer m_Buffer;
        Windows.Foundation.IMemoryBufferReference m_BufferReference;
        unsafe float* m_pData;
        uint m_DataCapacity;
        uint m_DataStep;
        uint m_RMSOffset;

        internal unsafe VisualizationData(AudioFrame frame)
        {
            m_Buffer = frame.LockBuffer(AudioBufferAccessMode.Read);
            m_BufferReference = m_Buffer.CreateReference();
            byte* pData;
            uint capacity;

            ((IMemoryBufferByteAccess)m_BufferReference).GetBuffer(out pData, out capacity);
            m_pData = (float*)pData;
            m_DataCapacity = m_Buffer.Length / sizeof(float);

            m_DataStep = (uint)frame.ExtendedProperties["{F4D65F78-CF5A-4949-88C1-76DAD605C313}"];
            m_RMSOffset = (uint)frame.ExtendedProperties["{0B82B25D-E6E1-4B6B-92A1-7EEE99D02CFE}"];
        }

        public uint GetChannelOffset(uint channel) { return m_DataStep * channel; }

        public uint Length { get { return m_DataCapacity; } }

        public float GetRMSValue(uint channel)
        {
            return this[m_RMSOffset + channel];
        }

        unsafe public float this[uint index]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
#if DEBUG
                if (index >= Length || index < 0)
                    throw new IndexOutOfRangeException();
#endif
                return m_pData[index];
            }
        }
    }
}
