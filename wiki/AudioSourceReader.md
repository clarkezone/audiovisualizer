# AudioSourceReader
AudioSourceReader is a wrapper for IMFSourceReader with specific focus on reading audio frames. It enables to open a media file and read audio frames in sequential order. It also has seek capability but it is not precice.

## Constructor
AudioSourceReader(Windows::Storage::Streams::IRandomAccessStream stream)

Opens the media file and creates reader


## Properties
### static Int32 FirstAudioStreamIndex { get; }

This is the default stream index - points to the first audio stream in the media file.

### AudioSourceReaderFlags Flags { get; }

Returns the current state of the reader. Flags are defined as identical to [MF_SOURCE_READER_FLAG](https://docs.microsoft.com/en-us/windows/desktop/api/mfreadwrite/ne-mfreadwrite-mf_source_reader_flag). 

###	Windows.Media.MediaProperties.IMediaEncodingProperties Format;

Sets the output media format of the reader for the stream that has been set by StreamIndex

### Windows.Foundation.TimeSpan Duration { get; };

Returns the estimated duration of the media file. This is usually accurate for the uncompressed files and can be less than the actual frames in the file. Do not rely on this value to allocate exact size buffers

### Int32 StreamIndex

Sets the stream index on which source reader operates. Stream index has to be valid (i.e not greater than stream count for the media file and needs to point to an audio stream), otherwise exception will be thrown.


## Methods
### Windows::Media::MediaProperties::IMediaEncodingProperties GetNativeFormat(Int32 streamIndex)

Returns native media format for the given stream in the file. If the stream index is invalid returns null, thus it can be used to enumerate streams in the file.

###	void Seek(Windows::Foundation::TimeSpan position)

Set's the stream position to the indicated value. Please note that the seek is almost always not accurate and positions the stream to the position before the position. In some cases this can be even slightly after the position. If you request a position that is past media file end then an exception is thrown.

###	Windows::Media::AudioFrame Read()

Reads the next audio frame from the file. If there are no more frames to read then returns null. If an error occurs, exception is thrown and for other conditions you can check the Flags property.

The number audio samples read can vary and is dependent on media type and other factors.

#### Note:

Media Foundation has few issues that causes inacurate position reporting for the audio frames. This class implements fixes to ensure that the audio frames read have consistent RelativeTime and Duration  properties. This means that Time[n] + Duration[n] = Time[n+1]. 

When doing math with TimeSpan please make a note that internally the value is represented as 64-bit integer in 100ns units. Rounding errors can therefore cause the above equasion to have inequalities if not rounding properly.

### Sample

```csharp

// Create analyzer for 10ms frames, 25% overlap
var analyzer = new AudioAnalyzer(100000, 2, 48000, 480, 120, 1024,false);

var mediaFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///music.mp3"));

var stream = await testFile.OpenAsync(FileAccessMode.Read);
var reader = new AudioSourceReader(stream);

// Set output format to same as analyzer, 48k, 2 channels. Analyzer needs 32bit float samples
var format = AudioEncodingProperties.CreatePcm(48000, 2, 32);
format.Subtype = "Float";
reader.Format = format;

AudioFrame frame = null;

do
{
    frame = reader.Read();
    if (frame == null) {
        break;
    }
    analyzer.ProcessInput(frame);

} while (frame != null);


```
